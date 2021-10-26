#ifndef ModuleMixer_h
#define ModuleMixer_h
#include "Arduino.h"
#include "Module.h"
#include "AnalogControl.h"

// two-channel mixer for now

class ModuleMixer : public Module {
  public:
    ModuleMixer();
    virtual void update();
    SocketOutput audioOut;
    SocketInput audioIn1;
    SocketInput audioIn2;
  private:
    AudioMixer4 _audioMixer[MAX_POLYPHONY];
    AudioConnection* _mixerCables1[MAX_POLYPHONY];
    AudioConnection* _mixerCables2[MAX_POLYPHONY];
    AudioConnection* _mixerCables3[MAX_POLYPHONY];
};

#endif
