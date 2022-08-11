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
const int MAX_CONNECTIONS = 32;
Connection connections[MAX_CONNECTIONS];
std::vector<Socket*> outputSocketMappings(32);
std::vector<Socket*> inputSocketMappings(32);

// declare modules
std::vector<Module *> modules(16);
VCO vco1;
VCO vco2;
VCF vcf;
IO io;


void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		// this is the old way of doing things...
		for(uint8_t j=0; j<MAX_CONNECTIONS; j++) {
			//processConnection(j);
		}
		for(uint8_t j=0; j<16; j++) {
			//if(modules[i] != NULL) modules[i]->process();
		}

		// new way of doing things...


		// set daisy seed output as final stage (IO module) output
		out[0][i] = io.getOutput();
		out[1][i] = io.getOutput();
	}
}

int main(void)
{
	// Daisy Seed config
	hw.Configure();
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);

	ShiftRegister595 outputChain;
	My165Chain inputChain;
	My165Chain::Config inputChainConfig;
	inputChainConfig.clk = D5;
	inputChainConfig.latch = D6;
	inputChainConfig.data[0] = D3;
	inputChain.Init(inputChainConfig);
	dsy_gpio_pin outputChainPins[3] = {D1,D2,D7};
	outputChain.Init(outputChainPins, 5);
	GPIO inputChainClockEnable;
	inputChainClockEnable.Init(D4, GPIO::Mode::OUTPUT);
	inputChainClockEnable.Write(false);
	AdcChannelConfig analogInputs[2];
	analogInputs[0].InitSingle(A0);
	analogInputs[1].InitSingle(A1);
	hw.adc.Init(analogInputs, 2);
	hw.adc.Start();

	// initialise vectors as null
	for (uint8_t i = 0; i < 16; i++)
	{
		modules[i] = NULL;
	}

	// set up modules
	modules[0] = &vco1;
	modules[1] = &vco2;
	modules[2] = &vcf;
	modules[3] = &io;
	outputSocketMappings[0] = &vco1._sockets[0];
	outputSocketMappings[1] = &vco2._sockets[0];
	outputSocketMappings[2] = &vcf._sockets[1];
	inputSocketMappings[0] = &io._sockets[0];
	inputSocketMappings[1] = &vcf._sockets[0];
	vco1.tempFreq = 80.0f;
	vco2.tempFreq = 240.0f;

	// start serial log (wait for connection)
	if(useSerial) {
		hw.StartLog(true);
		hw.PrintLine("STARTED");
	}

	int analogChannel = 0;
	int bitNumber = 0;

	// main loop, everything happens in here
	while(1) {
		// set output bits for patching output channels
		for(int i=0; i<32; i++) {
			outputChain.Set(i, bitRead(i+1,bitNumber));
		}

		// set bits to address 4051s
		outputChain.Set(32,bitRead(analogChannel,0));
		outputChain.Set(33,bitRead(analogChannel,1));
		outputChain.Set(34,bitRead(analogChannel,2));

		// set LEDs
		for(int i=0;i<5;i++) {
			outputChain.Set(35+i, analogReadings[0] > ((float) i + 0.5) / 5.0);
		}

		// read/write from/to shift registers
		outputChain.Write();
		inputChain.Update();

		for(int i=0; i<32; i++) {
			bitWrite(inputReadings[i], bitNumber, inputChain.State(i+8));
		}
		if(bitNumber == 7) {
			for(int i=0; i<32; i++) {
				if(inputReadings[i] != prevInputReadings[i]) {
					// change detected
					stableCycles[i] = 0;
				} else {
					if(stableCycles[i]<3) stableCycles[i] ++;
				}
				if(stableCycles[i]==2) {
					if(inputReadings[i]>0) {
						// connection
						if (useSerial)
							hw.Print("%d--->%d\n", inputReadings[i] - 1, i);
						addConnection(inputReadings[i]-1, i);
					} else if(stableInputReadings[i]>0) {
						// disconnection
						if (useSerial)
							hw.Print("%d-x->%d\n", stableInputReadings[i] - 1, i);
						removeConnection(stableInputReadings[i] - 1, i);
					}
					stableInputReadings[i] = inputReadings[i];
				}
				prevInputReadings[i] = inputReadings[i];
			}
		}

		hw.DelayMs(1); // seems to be required for 4051s to function properly - try reducing to microseconds to optimise patching speed
		analogReadings[analogChannel] = hw.adc.GetFloat(0);
		analogReadings[analogChannel+8] = hw.adc.GetFloat(1);

		// temp analog testing stuff
		vco1.tempFreq = 25.0f + 1000.0f * analogReadings[0];
		vco2.tempFreq = 25.0f + 1000.0f * analogReadings[1];
		vcf.tempRes = analogReadings[2];
		vcf.tempCutoff = 25.0f + 1000.0f * analogReadings[3];

		analogChannel ++; // probably merge analog channel and bitNumber, they're basically the same
		bitNumber = (bitNumber + 1) % 8;
		if(analogChannel == 8) {
			analogChannel = 0;
		}
	}
}

uint8_t findFreeConnectionSlot() {
	bool foundSlot = false;
	uint8_t slotNum = 0;
	for(uint8_t i=0; i<MAX_CONNECTIONS && !foundSlot; i++) {
		if(!connections[i].isConnected()) {
			slotNum = i;
			foundSlot = true;
		}
	}
	return slotNum; // should probably check all slots aren't full...
}

void addConnection(uint8_t physicalOutputNum, uint8_t physicalInputNum) {
	uint8_t slotNum = findFreeConnectionSlot();
	if(useSerial) hw.PrintLine("Connection slot %d", slotNum);
	connections[slotNum]._isConnected = true; // temp
	connections[slotNum].physicalOutputNum = physicalOutputNum;
	connections[slotNum].physicalInputNum = physicalInputNum;
}

void removeConnection(uint8_t physicalOutputNum, uint8_t physicalInputNum)
{
	for(uint8_t i=0; i<MAX_CONNECTIONS; i++) {
		if(connections[i].isConnected() && connections[i].physicalOutputNum == physicalOutputNum && connections[i].physicalInputNum == physicalInputNum) {
			connections[i]._isConnected = false; // temp
			if(useSerial) hw.PrintLine("Removed connection %d", i);
		}
	}
}

void processConnection(uint8_t connectionNum) {
	if (connections[connectionNum].isConnected()) {
		inputSocketMappings[connections[connectionNum].physicalInputNum]->inVal = outputSocketMappings[connections[connectionNum].physicalOutputNum]->outVal;
	}
}