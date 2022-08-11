// functions borrowed from Arduino source code to help bit manipulation (for addressing 4051s, etc)
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

// include libraries
#include "daisy_seed.h"
#include "daisysp.h"
#include "sr_165.h"

using namespace daisy;
using namespace daisy::seed;

#include "Connection.h" 
#include "Socket.h"

// include modules
#include "Module.h"
#include "VCO.h"
#include "VCF.h"
#include "IO.h"

// debugging stuff
bool useSerial = false;

// define chain of 5 74hc165 shift registers (read many digital inputs)
using My165Chain = ShiftRegister165<5>;

DaisySeed hw;

// patching variables
uint8_t inputReadings[32];
uint8_t prevInputReadings[32];
uint8_t stableCycles[32];
uint8_t stableInputReadings[32];

// analog variables
float analogReadings[16];

// misc variables (tidy up into groups later)
SaiHandle::Config::SampleRate sampleRate = SaiHandle::Config::SampleRate::SAI_48KHZ;
const int MAX_CONNECTIONS = 32;
Connection connections[MAX_CONNECTIONS];
Socket outputSockets[32];
Socket inputSockets[32];
int outputSocketOrder[32];
int inputSocketOrder[32];

// declare modules
VCO vco1;
VCO vco2;
VCF vcf;
IO io;

// declare functions
void calculateProcessOrder();
void setOrder(Socket socket, int order);
void addConnection(uint8_t physicalOutputNum, uint8_t physicalInputNum);
void removeConnection(uint8_t physicalOutputNum, uint8_t physicalInputNum);
void processConnection(uint8_t connectionNum);
uint8_t findFreeConnectionSlot();
void initOutput(int socketNumber, Module *module, int param);
void initInput(int socketNumber, Module *module, int param);

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		for(int j=0; j<32; j++) {
			int outNum = outputSocketOrder[j];
			if(outNum >=0) {
				outputSockets[outNum].process();
			}
			int inNum = inputSocketOrder[j];
			if (inNum >= 0)
			{
				inputSockets[inNum].process();
			}
		}

		// set daisy seed output as final stage (IO module) output
		out[0][i] = *io.inputFloats[IO::MAIN_IN];
		out[1][i] = *io.inputFloats[IO::MAIN_IN];
	}
}

int main(void)
{
	// Daisy Seed config
	hw.Configure();
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(sampleRate);
	hw.StartAudio(AudioCallback);

	// Polymod hardware config
	ShiftRegister595 outputChain;
	My165Chain inputChain;
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

	vco1.freq = 100.08;
	vco2.freq = 150.03;

	// set up sockets
	initOutput(0, &vco1, VCO::SQUARE_OUT);
	initOutput(1, &vco1, VCO::SAW_OUT);
	initOutput(2, &vco2, VCO::SQUARE_OUT);
	initOutput(3, &vco2, VCO::SAW_OUT);
	initOutput(4, &vcf, VCF::FILTER_OUT);

	initInput(0, &io, IO::MAIN_IN);
	initInput(1, &vcf, VCF::FILTER_IN);

	vcf.init();

	//outputSockets[4].pseudoSourceTemp = &inputSockets[1];

	//vco1.tempSocket = &outputSockets[0];

	// inputSockets[0].socketType = Socket::INPUT;
	// io.mainIn = &inputSockets[0].inVal;

	// inputSockets[1].socketType = Socket::INPUT;
	// vcf.filterIn = &inputSockets[1].inVal;

	// start serial log (wait for connection)
	if (useSerial)
	{
		hw.StartLog(true);
		hw.PrintLine("STARTED POLYMOD :)");
	}

	int analogChannel = 0;
	int bitNumber = 0;

	// main loop, everything happens in here
	while (1)
	{
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
						addConnection(inputReadings[i] - 1, i);
						calculateProcessOrder();
					}
					else if (stableInputReadings[i] > 0)
					{
						// disconnection
						if (useSerial)
							hw.Print("%d-x->%d\n", stableInputReadings[i] - 1, i);
						removeConnection(stableInputReadings[i] - 1, i);
						calculateProcessOrder();
					}
					stableInputReadings[i] = inputReadings[i];
				}
				prevInputReadings[i] = inputReadings[i];
			}
		}

		hw.DelayMs(1); // seems to be required for 4051s to function properly - try reducing to microseconds to optimise patching speed
		analogReadings[analogChannel] = hw.adc.GetFloat(0);
		analogReadings[analogChannel + 8] = hw.adc.GetFloat(1);

		analogChannel++; // probably merge analog channel and bitNumber, they're basically the same
		bitNumber = (bitNumber + 1) % 8;
		if (analogChannel == 8)
		{
			analogChannel = 0;
		}
	}
}

uint8_t findFreeConnectionSlot()
{
	bool foundSlot = false;
	uint8_t slotNum = 0;
	for (uint8_t i = 0; i < MAX_CONNECTIONS && !foundSlot; i++)
	{
		if (!connections[i].isConnected())
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
		hw.PrintLine("Connection slot %d", slotNum);
	//outputSocketMappings[physicalOutputNum]->destSocket = inputSocketMappings[physicalInputNum];
	connections[slotNum]._isConnected = true; // temp
	connections[slotNum].physicalOutputNum = physicalOutputNum;
	connections[slotNum].physicalInputNum = physicalInputNum;
	outputSockets[physicalOutputNum].destSocket = &inputSockets[physicalInputNum];
	inputSockets[physicalInputNum].sourceSocket = &outputSockets[physicalOutputNum];
}

void removeConnection(uint8_t physicalOutputNum, uint8_t physicalInputNum)
{
	for (uint8_t i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (connections[i].isConnected() && connections[i].physicalOutputNum == physicalOutputNum && connections[i].physicalInputNum == physicalInputNum)
		{
			connections[i]._isConnected = false; // temp
			outputSockets[physicalOutputNum].destSocket = nullptr;
			inputSockets[physicalInputNum].sourceSocket = nullptr;
			if (useSerial)
				hw.PrintLine("Removed connection %d", i);
		}
	}
}

void processConnection(uint8_t connectionNum)
{
	if (connections[connectionNum].isConnected())
	{
		//inputSocketMappings[connections[connectionNum].physicalInputNum]->inVal = outputSocketMappings[connections[connectionNum].physicalOutputNum]->outVal;
	}
}

void setOrder(Socket *socket, int order)
{
	if(!socket->orderSet) {
		socket->order = order;
		socket->orderSet = true;
		if(socket->socketType == Socket::INPUT) {
			if(socket->sourceSocket != nullptr) {
				setOrder(socket->sourceSocket, order + 1);
			}
		} else if(socket->socketType == Socket::OUTPUT) {
			// if(socket->pseudoSourceTemp != nullptr) {
			// 	setOrder(socket->pseudoSourceTemp, order + 1);
			// }
			for(int i=0; i<8; i++) {
				int pseudoConnNum = socket->module->pseudoSources[socket->param];
				if(pseudoConnNum != -1) {
					Socket *thisSocket = socket->module->sockets[pseudoConnNum];
					if (thisSocket != nullptr) {
						setOrder(thisSocket, order + 1);
					}
				}
			}
		}
	}
}

void calculateProcessOrder() {
	// reset order numbers
	for(int i=0; i<32; i++) {
		outputSockets[i].order = 999;
		outputSockets[i].orderSet = false;
		inputSockets[i].order = 999;
		inputSockets[i].orderSet = false;
	}

	// start at IO output (input socket)
	setOrder(&inputSockets[0], 0); // recursive function, sets order number for all sockets

	// reset socket order
	for (int i = 0; i < 32; i++)
	{
		outputSocketOrder[i] = -1;
		inputSocketOrder[i] = -1;
	}

	int orderIndex = 0;
	
	// start at 64, maximum possible order number (?)
	for(int i=64; i>=0; i--) {
		for(int j=0; j<32; j++) {
			if(outputSockets[j].order == i) {
				outputSocketOrder[orderIndex] = j;
				orderIndex ++;
			}
		}
	}

	// do the same for inputs
	orderIndex = 0;
	for (int i = 64; i >= 0; i--)
	{
		for (int j = 0; j < 32; j++)
		{
			if (inputSockets[j].order == i)
			{
				inputSocketOrder[orderIndex] = j;
				orderIndex++;
			}
		}
	}

	if(useSerial) {
		for (int i = 0; i < 8; i++)
		{
			hw.Print("%d %d ", outputSocketOrder[i], inputSocketOrder[i]);
		}
		hw.PrintLine("...");
	}
}

void initOutput(int socketNumber, Module *module, int param) {
	outputSockets[socketNumber].socketType = Socket::OUTPUT;
	outputSockets[socketNumber].module = module;
	outputSockets[socketNumber].param = param;
	module->sockets[param] = &outputSockets[socketNumber];
}

void initInput(int socketNumber, Module *module, int param)
{
	inputSockets[socketNumber].socketType = Socket::INPUT;
	module->inputFloats[param] = &inputSockets[socketNumber].inVal;
	module->sockets[param] = &inputSockets[socketNumber];
}