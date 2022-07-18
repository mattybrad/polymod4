#ifndef VCO_h
#define VCO_h
#include "Module.h"

class VCO : public Module {
  public:
    VCO();
    virtual void update();
    //SocketOutput audioOut;
    //SocketInput freqModIn;
  private:
    static Oscillator _osc[4];
    //AnalogControl _freqControl;
    //AudioSynthWaveformModulated _osc[MAX_POLYPHONY];
    //AudioMixer4 _modMixer[MAX_POLYPHONY];
};

#endif