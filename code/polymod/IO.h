#ifndef IO_h
#define IO_h
#include "Module.h"
#include "daisy_seed.h"
#include "daisysp.h"
using namespace daisysp;
using namespace daisy;

class IO : public Module {
  public:
    IO();
    virtual float process(int functionID, int polyChannel, int sampleNum);
    void handleMidiMessage(MidiEvent m);
    void handleNoteOn(int noteNum);
    void handleNoteOff(int noteNum);
    enum { MAIN_OUTPUT_IN,MAIN_INPUT_OUT,MIDI_PITCH,MIDI_GATE };
    float tempFreqs[MAX_POLYPHONY] = {-0.2, -0.1, 0, 0.0587};
    float tempGates[MAX_POLYPHONY] = {0,0,0,0};
    enum { NOTE_NUM, NOTE_ON_TIME, NOTE_OFF_TIME };
    int midiNotes[MAX_POLYPHONY][3]; // note num, note on time, note off time

  private:
};

#endif