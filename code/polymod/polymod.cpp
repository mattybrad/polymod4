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
#include "VCO.h"
#include "IO.h"

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
VCO vco;
IO io;

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
	hw.PrintLine("Connection slot %d", slotNum);
	connections[slotNum]._isConnected = true;
	connections[slotNum].physicalOutputNum = physicalOutputNum;
	connections[slotNum].physicalInputNum = physicalInputNum;
}

void processConnection(uint8_t connectionNum) {
	if (connections[connectionNum].isConnected()) {
		inputSocketMappings[connections[connectionNum].physicalInputNum]->inVal = outputSocketMappings[connections[connectionNum].physicalOutputNum]->outVal;
	}
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		for(uint8_t j=0; j<MAX_CONNECTIONS; j++) {
			processConnection(j);
		}
		vco.process();
		//VCF.process();
		io.process();
		out[0][i] = io.getOutput();
		out[1][i] = io.getOutput();
		//out[0][i] = in[0][i];
		//out[1][i] = in[1][i];
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

	// set up modules
	//vco.mapOutput(0, 0);
	outputSocketMappings[0] = &vco._sockets[0];
	inputSocketMappings[0] = &io._sockets[0];

	// start serial log (wait for connection)
    hw.StartLog(true);
	hw.PrintLine("STARTED");

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
						hw.Print("%d--->%d\n", inputReadings[i]-1, i);
						addConnection(inputReadings[i]-1, i);
					} else if(stableInputReadings[i]>0) {
						// disconnection
						hw.Print("%d-x->%d\n", stableInputReadings[i]-1, i);
					}
					stableInputReadings[i] = inputReadings[i];
				}
				prevInputReadings[i] = inputReadings[i];
			}
		}

		hw.DelayMs(1); // seems to be required for 4051s to function properly
		//float analogReading1 = hw.adc.GetFloat(0);
		//float analogReading2 = hw.adc.GetFloat(1);
		analogReadings[analogChannel] = hw.adc.GetFloat(0);
		analogReadings[analogChannel+8] = hw.adc.GetFloat(1);
		/*FixedCapStr<16> str1("");
		str1.AppendFloat(analogReading1);
		FixedCapStr<16> str2("");
		str2.AppendFloat(analogReading2);
		hw.Print("%d ",analogChannel);
		hw.Print(str1);
		hw.Print(" ");
		hw.PrintLine(str2);*/
		
		analogChannel ++; // probably merge analog channel and bitNumber, they're basically the same
		bitNumber = (bitNumber + 1) % 8;
		if(analogChannel == 8) {
			analogChannel = 0;
			//hw.DelayMs(500);
			//flashToggle = !flashToggle;
		}
	}
}