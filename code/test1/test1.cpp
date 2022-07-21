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

	bool led_state = false;

	// Start the log, and wait for connection
    hw.StartLog(true);
	hw.PrintLine("STARTED");
    // Print "Hello World" to the Serial Monitor

	while(1) {
		hw.SetLed(led_state);
		hw.DelayMs(200);
		//led_state = !led_state;
		for(int i=0; i<40; i++) {
			if(i<37) outputChain.Set(i,true);
			else outputChain.Set(i,led_state);
		}
		outputChain.Write();
		inputChain.Update();
		bool anyHigh = false;
		hw.PrintLine("Input readings:");
		for(int i=0; i<40; i++) {
			//hw.Print("\t");
			hw.Print(inputChain.State(i)?"1":"0");
		}
		led_state = anyHigh;
		hw.DelayMs(500);
	}
}
