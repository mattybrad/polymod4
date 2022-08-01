// took these from Arduino source code, need to check this works okay!
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#include "daisy_seed.h"
#include "daisysp.h"
#include "sr_165.h"

using namespace daisy;
using namespace daisy::seed;

using My165Chain = ShiftRegister165<5>;

DaisySeed hw;

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

	bool led_state = false;

	// Start the log, and wait for connection
    hw.StartLog(true);
	hw.PrintLine("STARTED");
    // Print "Hello World" to the Serial Monitor
	int analogChannel = 0;

	while(1) {
		//led_state = !led_state;
		for(int i=0; i<32; i++) {
			outputChain.Set(i,true);
		}
		outputChain.Set(32,bitRead(analogChannel,0));
		outputChain.Set(33,bitRead(analogChannel,1));
		outputChain.Set(34,bitRead(analogChannel,2));
		outputChain.Set(35,bitRead(analogChannel,0));
		outputChain.Set(36,bitRead(analogChannel,1));
		outputChain.Set(37,bitRead(analogChannel,2));
		outputChain.Set(38,true);
		outputChain.Set(39,true);
		outputChain.Write();
		inputChain.Update();
		bool anyHigh = false;
		if(analogChannel == 0) {
			hw.PrintLine("Input readings:");
			for(int i=0; i<40; i++) {
				//hw.Print("\t");
				hw.Print(inputChain.State(i)?"1":"0");
			}
			hw.Print("\n");
		}
		float analogReading1 = hw.adc.GetFloat(0);
		float analogReading2 = hw.adc.GetFloat(1);
		FixedCapStr<16> str1("");
		str1.AppendFloat(analogReading1);
		FixedCapStr<16> str2("");
		str2.AppendFloat(analogReading2);
		hw.Print("%d ",analogChannel);
		hw.Print(str1);
		hw.Print(" ");
		hw.PrintLine(str2);
		/*int ana1 = hw.adc.Get(0);
		int ana2 = hw.adc.Get(1);
		hw.Print("%d %d %d\n", analogChannel, ana1, ana2);*/
		
		led_state = anyHigh;
		analogChannel ++;
		if(analogChannel == 8) {
			analogChannel = 0;
			hw.DelayMs(20);
		}
	}
}
