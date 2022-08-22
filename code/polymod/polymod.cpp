// functions borrowed from Arduino source code to help bit manipulation (for addressing 4051s, etc)
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

// include libraries
#include "daisy_seed.h"
#include "daisysp.h"
#include "core_cm7.h"
#include "sr_165.h"

//using namespace daisy;
using namespace daisy::seed;

#include "Connection.h" 
#include "Socket.h"

// include modules
#include "Module.h"
#include "VCO.h"
#include "VCF.h"
#include "LFO.h"
#include "BitCrusher.h"
#include "IO.h"

// socket stuff
Socket sockets[64];
Socket *socketOrder[64];
const int MAX_CONNECTIONS = 32;
Connection connections[MAX_CONNECTIONS];

// debugging stuff
bool useSerial = false;

// define chain of 5 74hc165 shift registers (read many digital inputs)
using My165Chain = ShiftRegister165<5>;
ShiftRegister595 outputChain;
My165Chain inputChain;

// patching variables
uint8_t inputReadings[32];
uint8_t prevInputReadings[32];
uint8_t stableCycles[32];
uint8_t stableInputReadings[32];
int bitNumber = 0;

// analog variables
float analogReadings[16];
int analogChannel = 0;

DaisySeed hw;

// declare modules
VCO vco1;
VCO vco2;
LFO lfo;
VCF vcf;
BitCrusher crusher;
IO io;

// declare functions
void addConnection(uint8_t physicalOutputNum, uint8_t physicalInputNum);
void removeConnection(uint8_t physicalOutputNum, uint8_t physicalInputNum);
void calculateSocketOrder();
void setSocketOrder(Socket *socket, int order);
void initOutput(int socketNumber, Module *module, int param);
void initInput(int socketNumber, Module *module, int param);
int getSystemPinNum(int userPinNum);
void handlePhysicalConnections();

float dummyFloat;
void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		bool nullFound = false;
		for (int j = 0; j < 64 && !nullFound; j++)
		{
			if(socketOrder[j] == nullptr) {
				nullFound = true;
			} else {
				socketOrder[j]->process(i);
			}
		}

		// set daisy seed output as final stage (IO module) output
		float finalOutput = 0.0f;
		for(int k=0; k<Module::MAX_POLYPHONY; k++) {
			finalOutput += io.sockets[IO::MAIN_OUTPUT_IN]->value[k];
		}
		finalOutput = 0.1 * finalOutput;
		out[0][i] = finalOutput;
		out[1][i] = finalOutput;
	}
}

int main(void)
{
	// Daisy Seed config
	hw.Configure();
	hw.Init();
	hw.SetAudioBlockSize(64); // can increase this if having performance issues
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	// Polymod hardware config
	My165Chain::Config inputChainConfig;
	inputChainConfig.clk = D5;
	inputChainConfig.latch = D6;
	inputChainConfig.data[0] = D3;
	inputChain.Init(inputChainConfig);
	dsy_gpio_pin outputChainPins[3] = {D1, D2, D7};
	outputChain.Init(outputChainPins, 5);
	GPIO inputChainClockEnable;
	inputChainClockEnable.Init(D4, GPIO::Mode::OUTPUT);
	inputChainClockEnable.Write(false);
	AdcChannelConfig analogInputs[2];
	analogInputs[0].InitSingle(A0);
	analogInputs[1].InitSingle(A1);
	hw.adc.Init(analogInputs, 2);
	hw.adc.Start();

	// start serial log (wait for connection)
	if (useSerial)
	{
		hw.StartLog(true);
		hw.PrintLine("STARTED POLYMOD :)");
	}

	hw.StartAudio(AudioCallback);

	// set up sockets
	for(int i=0; i<64; i++) {
		sockets[i].socketNum = i;
	}
	initOutput(0, &vco1, VCO::AUDIO_OUT);
	initOutput(1, &vcf, VCF::LPF_OUT);
	initOutput(2, &lfo, LFO::AUDIO_OUT);
	initOutput(3, &crusher, BitCrusher::AUDIO_OUT);
	initInput(32, &io, IO::MAIN_OUTPUT_IN);
	initInput(33, &vcf, VCF::AUDIO_IN);
	initInput(34, &vcf, VCF::FREQ_IN);
	initInput(35, &vco1, VCO::FREQ_IN);
	initInput(36, &crusher, BitCrusher::AUDIO_IN);

	/*addConnection(0, 33);
	addConnection(1, 32);
	addConnection(2, 34);*/

	// main loop, everything happens in here
	while (1)
	{
		// do nothing here for now, this is where all the shift reg stuff goes
		handlePhysicalConnections();
	}
}

uint8_t findFreeConnectionSlot()
{
	bool foundSlot = false;
	uint8_t slotNum = 0;
	for (uint8_t i = 0; i < MAX_CONNECTIONS && !foundSlot; i++)
	{
		if (!connections[i].isConnected)
		{
			slotNum = i;
			foundSlot = true;
		}
	}
	return slotNum; // should probably check all slots aren't full...
}

void addConnection(uint8_t physicalOutputNum, uint8_t physicalInputNum)
{
	uint8_t slotNum = findFreeConnectionSlot();
	if (useSerial)
	{
		hw.PrintLine("Connection slot %d", slotNum);
	}
	connections[slotNum].isConnected = true;
	connections[slotNum].physicalOutputNum = physicalOutputNum;
	connections[slotNum].physicalInputNum = physicalInputNum;
	sockets[physicalOutputNum].destSocket = &sockets[physicalInputNum];
	sockets[physicalInputNum].sourceSocket = &sockets[physicalOutputNum];
	calculateSocketOrder();
}

void removeConnection(uint8_t physicalOutputNum, uint8_t physicalInputNum)
{
	for (uint8_t i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (connections[i].isConnected && connections[i].physicalOutputNum == physicalOutputNum && connections[i].physicalInputNum == physicalInputNum)
		{
			connections[i].isConnected = false;
			sockets[physicalOutputNum].destSocket = nullptr;
			sockets[physicalInputNum].sourceSocket = nullptr;
			if (useSerial)
				hw.PrintLine("Removed connection %d", i);
		}
	}
	calculateSocketOrder();
}

void calculateSocketOrder() {

	// reset order numbers
	for (int i = 0; i < 64; i++)
	{
		sockets[i].order = -1;
		sockets[i].orderSet = false;
	}

	// start at IO output (input socket)
	setSocketOrder(io.sockets[IO::MAIN_OUTPUT_IN], 0); // recursive function, sets order number for all sockets
	// after this, sockets should have higher order numbers as they get further away from end of signal chain

	// reset socket order
	for (int i = 0; i < 64; i++)
	{
		socketOrder[i] = nullptr;
	}

	int orderIndex = 0; // this number is HIGHEST at end of signal chain (opposite of above)

	// start at 64, maximum possible order number (?)
	for (int i = 64; i >= 0; i--)
	{
		// for each socket...
		for (int j = 0; j < 64; j++)
		{
			if (sockets[j].order == i)
			{
				socketOrder[orderIndex] = &sockets[j];
				orderIndex++;
			}
		}
	}

	if (useSerial)
	{
		hw.Print("Socket order: ");
		bool nullFound = false;
		for (int i = 0; i < 64 && !nullFound; i++)
		{
			if(socketOrder[i] == nullptr) nullFound = true;
			else {
				hw.Print("%d ", socketOrder[i]->socketNum);
			}
		}
		hw.Print("\n");
	}
}

void setSocketOrder(Socket *socket, int order)
{
	if (!socket->orderSet)
	{
		socket->order = order;
		socket->orderSet = true;
		if (socket->socketType == Socket::INPUT)
		{
			if (socket->sourceSocket != nullptr)
			{
				setSocketOrder(socket->sourceSocket, order + 1);
			}
		}
		else if (socket->socketType == Socket::OUTPUT)
		{
			for (int i = 0; i < socket->module->numPseudoSources; i++)
			{
				if (socket->module->pseudoSources[i][1] == socket->param)
				{
					// pseudo socket destination matches output
					setSocketOrder(socket->module->sockets[socket->module->pseudoSources[i][0]], order + 1);
				}
			}
		}
	}
}

int getSystemPinNum(int userPinNum)
{
	return 8 * (userPinNum / 8) + 7 - (userPinNum % 8);
}

void initOutput(int socketNumber, Module *module, int param)
{
	int systemSocketNumber = getSystemPinNum(socketNumber); // remap
	sockets[systemSocketNumber].socketType = Socket::OUTPUT;
	sockets[systemSocketNumber].module = module;
	sockets[systemSocketNumber].param = param;
	module->sockets[param] = &sockets[systemSocketNumber];
}

void initInput(int socketNumber, Module *module, int param)
{
	int systemSocketNumber = getSystemPinNum(socketNumber); // remap
	sockets[systemSocketNumber].socketType = Socket::INPUT;
	sockets[systemSocketNumber].module = module;
	sockets[systemSocketNumber].param = param;
	module->sockets[param] = &sockets[systemSocketNumber];
}

void handlePhysicalConnections() {
	// set output bits for patching output channels
	for (int i = 0; i < 32; i++)
	{
		outputChain.Set(i, bitRead(i + 1, bitNumber));
	}

	// set bits to address 4051s
	outputChain.Set(32, bitRead(analogChannel, 0));
	outputChain.Set(33, bitRead(analogChannel, 1));
	outputChain.Set(34, bitRead(analogChannel, 2));

	// set LEDs
	for (int i = 0; i < 5; i++)
	{
		outputChain.Set(35 + i, analogReadings[0] > ((float)i + 0.5) / 5.0);
	}

	// read/write from/to shift registers
	outputChain.Write();
	inputChain.Update();

	for (int i = 0; i < 32; i++)
	{
		bitWrite(inputReadings[i], bitNumber, inputChain.State(i + 8));
	}
	if (bitNumber == 7)
	{
		for (int i = 0; i < 32; i++)
		{
			if (inputReadings[i] != prevInputReadings[i])
			{
				// change detected
				stableCycles[i] = 0;
			}
			else
			{
				if (stableCycles[i] < 3)
					stableCycles[i]++;
			}
			if (stableCycles[i] == 2)
			{
				if (inputReadings[i] > 0)
				{
					// connection
					if (useSerial)
						hw.Print("%d--->%d\n", inputReadings[i] - 1, i);
					addConnection(inputReadings[i] - 1, i + 32);
				}
				else if (stableInputReadings[i] > 0)
				{
					// disconnection
					if (useSerial)
						hw.Print("%d-x->%d\n", stableInputReadings[i] - 1, i);
					removeConnection(stableInputReadings[i] - 1, i + 32);
				}
				stableInputReadings[i] = inputReadings[i];
			}
			prevInputReadings[i] = inputReadings[i];
		}
	}

	System::DelayUs(250); // seems to be required for 4051s to work properly - ideally replace blocking delay with callback
	analogReadings[analogChannel] = hw.adc.GetFloat(0);
	analogReadings[analogChannel + 8] = hw.adc.GetFloat(1);

	//tempLfo.freq = 0.01 + 100.0f * analogReadings[0];

	analogChannel++; // probably merge analog channel and bitNumber, they're basically the same
	bitNumber = (bitNumber + 1) % 8;
	if (analogChannel == 8)
	{
		analogChannel = 0;
	}
}