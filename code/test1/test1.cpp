#include "daisy_seed.h"
#include "daisysp.h"
#include "TestLib.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;
MidiUartHandler midi;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = in[0][i];
		out[1][i] = in[1][i];
	}
}

// Typical Switch case for Message Type.
void HandleMidiMessage(MidiEvent m)
{
	switch (m.type)
	{
	case NoteOn:
	{
		hw.PrintLine("note on");
		NoteOnEvent p = m.AsNoteOn();
		// This is to avoid Max/MSP Note outs for now..
		if (m.data[1] != 0)
		{
			p = m.AsNoteOn();
			hw.PrintLine("note: %d", p.note);
		}
	}
	break;
	default:
		hw.PrintLine("something else");
		break;
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);

	hw.StartLog(true);
	hw.PrintLine("Attempting MIDI input...");

	MidiUartHandler::Config midiConfig;
	midiConfig.transport_config.rx = daisy::seed::D14;
	midiConfig.transport_config.tx = daisy::seed::D13;
	midi.Init(midiConfig);
	midi.StartReceive();

	for (;;)
	{
		midi.Listen();
		// Handle MIDI Events
		while (midi.HasEvents())
		{
			HandleMidiMessage(midi.PopEvent());
		}
	}
}
