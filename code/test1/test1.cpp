#include "daisy_seed.h"
#include "daisysp.h"
#include "TestLib.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

struct StorageTestData
{
	StorageTestData() : a(0xdeadbeee) {}

	uint32_t a;

	bool operator==(const StorageTestData &rhs) { return a == rhs.a; }
	bool operator!=(const StorageTestData &rhs) { return !operator==(rhs); }
};

using StorageTestClass = PersistentStorage<StorageTestData>;

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
	hw.PrintLine("QSPI test...");

	daisy::QSPIHandle::Status s = hw.qspi.GetStatus();
	hw.PrintLine("qspi status: %d", s);

	StorageTestClass storage(hw.qspi);
	StorageTestData defaults;
	storage.Init(defaults);
	//auto &data = storage.GetSettings();
	//data.a = 123;
	//storage.Save();
	auto state = storage.GetState();
	auto val = storage.GetSettings().a;

	hw.PrintLine("got to here...");
	hw.PrintLine("storage stage: %d", state);
	hw.PrintLine("storage value: %d", val);
}
