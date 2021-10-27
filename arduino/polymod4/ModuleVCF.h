#ifndef ModuleVCF_h
#define ModuleVCF_h
#include "Arduino.h"
#include "Module.h"
#include "AnalogControl.h"

class ModuleVCF : public Module {
  public:
    ModuleVCF();
    virtual void update();
    SocketInput audioIn;
    SocketInput freqModIn;
    SocketOutput audioOut;
    byte analogPin = 14;
  private:
    AnalogControl _freqControl;
    AudioFilterStateVariable _filter[MAX_POLYPHONY];
    AudioMixer4 _modMixer[MAX_POLYPHONY];
    AudioConnection* _inputCables[MAX_POLYPHONY];
    AudioConnection* _outputCables[MAX_POLYPHONY];
    AudioConnection* _modCables1[MAX_POLYPHONY];
    AudioConnection* _modCables2[MAX_POLYPHONY];
    AudioConnection* _modCables3[MAX_POLYPHONY];
};

#endif
