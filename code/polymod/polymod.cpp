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

// define chain of 5 74hc165 shift registers (read many digital inputs)
using My165Chain = ShiftRegister165<5>;

DaisySeed hw;

// some variables
uint8_t inputReadings[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t prevInputReadings[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = in[0][i];
		out[1][i] = in[1][i];
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

	// start serial log (wait for connection)
    hw.StartLog(true);
	hw.PrintLine("STARTED");

	int analogChannel = 0;
	int bitNumber = 0;
	bool flashToggle = false;

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

		// set 5x LEDs
		outputChain.Set(35,flashToggle);
		outputChain.Set(36,!flashToggle);
		outputChain.Set(37,flashToggle);
		outputChain.Set(38,!flashToggle);
		outputChain.Set(39,flashToggle);

		outputChain.Write();
		inputChain.Update();

		// temp testing stuff
		if(bitNumber == 0) {
			//hw.PrintLine("Input readings:");
			for(int i=0; i<40; i++) {
				//hw.Print(inputChain.State(i)?"1":"0");
			}
			//hw.Print("\n");
		}

		for(int i=0; i<32; i++) {
			bitWrite(inputReadings[i], bitNumber, inputChain.State(i+8));
		}
		if(bitNumber == 7) {
			for(int i=0; i<32; i++) {
				if(inputReadings[i] != prevInputReadings[i]) {
					// change detected
					if(inputReadings[i]>0) {
						// connection
						hw.Print("%d--->%d\n", inputReadings[i]-1, i);
					} else {
						// disconnection
						hw.Print("%d-x->%d\n", prevInputReadings[i]-1, i);
					}
				}
				prevInputReadings[i] = inputReadings[i];
			}
		}

		hw.DelayMs(1); // seems to be required for 4051s to function properly
		float analogReading1 = hw.adc.GetFloat(0);
		float analogReading2 = hw.adc.GetFloat(1);
		FixedCapStr<16> str1("");
		str1.AppendFloat(analogReading1);
		FixedCapStr<16> str2("");
		str2.AppendFloat(analogReading2);
		//hw.Print("%d ",analogChannel);
		//hw.Print(str1);
		//hw.Print(" ");
		//hw.PrintLine(str2);
		
		analogChannel ++; // probably merge analog channel and bitNumber, they're basically the same
		bitNumber = (bitNumber + 1) % 8;
		if(analogChannel == 8) {
			analogChannel = 0;
			//hw.DelayMs(500);
			flashToggle = !flashToggle;
		}
	}
}
