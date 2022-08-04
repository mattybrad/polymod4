#include "daisy_seed.h"
#include "daisysp.h"
#include "TestLib.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;
TestLib testLib;

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
	hw.StartLog(true);
	hw.PrintLine("this is a test %d", testLib.myNum);
	while(1) {}
}
