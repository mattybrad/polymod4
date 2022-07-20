#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisy::seed;

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
	dsy_gpio_pin outputChainPins[3] = {D1,D2,D7};
	outputChain.Init(outputChainPins, 5);

	bool led_state = true;
	int counter = 0;
	int ledPins[] = {39,38,37,36,35};
	while(1) {
		hw.SetLed(led_state);
		hw.DelayMs(200);
		led_state = !led_state;
		for(int i=0; i<5; i++) {
			outputChain.Set(ledPins[i],(counter%5)==i);
		}
		outputChain.Write();
		counter ++;
	}
}
