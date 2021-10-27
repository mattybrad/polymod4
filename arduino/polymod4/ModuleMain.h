#ifndef ModuleMain_h
#define ModuleMain_h
#include "Arduino.h"
#include "Module.h"

class ModuleMain : public Module {
  public:
    ModuleMain();
    virtual void update();
    SocketInput audioIn;
  private:
    AudioOutputI2S _i2s;
    AudioMixer4 _primaryMixers[4];
    AudioMixer4 _secondaryMixer;
    AudioConnection* _primaryMixerCables[16];
    AudioConnection* _secondaryMixerCables[4];
    AudioConnection* _outputCables[2];
    
};

#endif
