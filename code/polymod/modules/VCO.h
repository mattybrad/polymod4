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
    virtual float process(int functionID, int polyChannel, int sampleNum);
    float freqControl;
    Oscillator square[MAX_POLYPHONY];
    enum {FREQ_IN, AUDIO_OUT};

  private:
};

#endif