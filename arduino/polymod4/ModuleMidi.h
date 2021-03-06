#ifndef ModuleMidi_h
#define ModuleMidi_h
#include "Arduino.h"
#include "Module.h"
#include "Note.h"

class ModuleMidi : public Module {
  public:
    ModuleMidi();
    virtual void update();
    void noteOn(byte channel, byte note, byte velocity);
    void noteOff(byte channel, byte note, byte velocity);
    SocketOutput freqOut;
    SocketOutput gateOut;
    SocketOutput velOut;
  private:
    AudioSynthWaveformDc _freqDC[MAX_POLYPHONY];
    AudioSynthWaveformDc _gateDC[MAX_POLYPHONY];
    AudioSynthWaveformDc _velDC[MAX_POLYPHONY];
    AudioConnection* _freqCables[MAX_POLYPHONY];
    AudioConnection* _gateCables[MAX_POLYPHONY];
    Note _notes[MAX_POLYPHONY];

};

#endif
