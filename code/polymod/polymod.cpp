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

using namespace daisy;
using namespace daisy::seed;

#include "Connection.h" 
#include "Socket.h"

// include modules
#include "Module.h"
#include "VCO.h"
#include "VCF.h"
#include "IO.h"

// socket stuff
Socket sockets[64];
Socket *socketOrder[64];
const int MAX_CONNECTIONS = 32;
Connection connections[MAX_CONNECTIONS];

// debugging stuff
bool useSerial = true;

// define chain of 5 74hc165 shift registers (read many digital inputs)
using My165Chain = ShiftRegister165<5>;

DaisySeed hw;

// declare modules
VCO vco1;
VCO vco2;
VCO tempLfo;
VCF vcf;
IO io;

// declare functions
void addConnection(uint8_t physicalOutputNum, uint8_t physicalInputNum);
void removeConnection(uint8_t physicalOutputNum, uint8_t physicalInputNum);
void calculateSocketOrder();
void setSocketOrder(Socket *socket, int order);
void initOutput(int socketNumber, Module *module, int param);
void initInput(int socketNumber, Module *module, int param);

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
				socketOrder[j]->process();
			}
		}

		//socketOrder[2]->module->inputFloats[VCF::AUDIO_IN] = 0.11;

		// set daisy seed output as final stage (IO module) output
		float finalOutput = 0.0f;
		for(int i=0; i<Module::MAX_POLYPHONY; i++) {
			finalOutput += 0.1 * io.sockets[IO::MAIN_OUTPUT_IN]->value[i];
		}
		out[0][i] = finalOutput;
		out[1][i] = finalOutput;
	}
}

int main(void)
{
	// Daisy Seed config
	hw.Configure();
	hw.Init();
	hw.SetAudioBlockSize(16); // can increase this if having performance issues
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

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
	initInput(32, &io, IO::MAIN_OUTPUT_IN);
	initInput(33, &vcf, VCF::AUDIO_IN);

	//addConnection(0,32);
	addConnection(0, 33);
	addConnection(1, 32);
	/*hw.DelayMs(2000);
	removeConnection(0, 33);
	removeConnection(1, 32);
	addConnection(0,32);*/

	// main loop, everything happens in here
	while (1)
	{
		// do nothing here for now, this is where all the shift reg stuff goes
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

void initOutput(int socketNumber, Module *module, int param)
{
	//int systemSocketNumber = getSystemPinNum(socketNumber); // remap
	sockets[socketNumber].socketType = Socket::OUTPUT;
	sockets[socketNumber].module = module;
	sockets[socketNumber].param = param;
	module->sockets[param] = &sockets[socketNumber];
}

void initInput(int socketNumber, Module *module, int param)
{
	//int systemSocketNumber = getSystemPinNum(socketNumber); // remap
	sockets[socketNumber].socketType = Socket::INPUT;
	sockets[socketNumber].module = module;
	sockets[socketNumber].param = param;
	//module->inputFloats[param] = &inputSockets[socketNumber].inVal;
	module->sockets[param] = &sockets[socketNumber];
}