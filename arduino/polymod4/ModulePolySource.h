#ifndef ModulePolySource_h
#define ModulePolySource_h
#include "Arduino.h"
#include "Module.h"

class ModulePolySource : public Module {
  public:
    ModulePolySource();
    virtual void update();
    SocketOutput audioOut;
  private:
    AudioSynthWaveformDc _dc[MAX_POLYPHONY];
    AudioConnection* _cables[MAX_POLYPHONY];
    
};

#endif
