#ifndef ModuleADSR_h
#define ModuleADSR_h
#include "Arduino.h"
#include "Module.h"

class ModuleADSR : public Module {
  public:
    ModuleADSR();
    virtual void update();
    void tempChangeSettings();
    SocketInput gateIn;
    SocketOutput controlOut;
  private:
    bool _gateOpen[MAX_POLYPHONY];
    AudioRecordQueue _queue[MAX_POLYPHONY];
    AudioEffectEnvelope _env[MAX_POLYPHONY];
    AudioSynthWaveformDc _dc;
    AudioConnection* _dcCables[MAX_POLYPHONY];
    AudioConnection* _inputCables[MAX_POLYPHONY];
    AudioConnection* _outputCables[MAX_POLYPHONY];
};

#endif
