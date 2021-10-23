#ifndef AnalogControl_h
#define AnalogControl_h
#include "Arduino.h"
#include "SocketOutput.h"
#include <Audio.h>

class AnalogControl {
  public:
    AnalogControl();
    byte pin = 15; // temp
    virtual void update();
    AudioSynthWaveformDc dc;
  private:
    AudioConnection* _cables[MAX_POLYPHONY];
    
};

#endif
