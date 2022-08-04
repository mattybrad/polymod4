#ifndef VCO_h
#define VCO_h
#include "Module.h"
#include "daisy_seed.h"
#include "daisysp.h"
using namespace daisysp;
using namespace daisy;

class VCO : public Module {
  public:
    VCO();
    //virtual void prepare();
    virtual void process();
    float tempFreq = 100.0f;
    //SocketOutput audioOut;
    //SocketInput freqModIn;
  private:
    Oscillator _osc;
    //static Oscillator _osc[4];
    //AnalogControl _freqControl;
    //AudioSynthWaveformModulated _osc[MAX_POLYPHONY];
    //AudioMixer4 _modMixer[MAX_POLYPHONY];
};

#endif