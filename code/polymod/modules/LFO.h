#ifndef LFO_h
#define LFO_h
#include "Module.h"
#include "daisy_seed.h"
#include "daisysp.h"
using namespace daisysp;
using namespace daisy;

class LFO : public Module {
  public:
    LFO();
    virtual float process(int functionID, int polyChannel, int sampleNum);
    float freqControl;
    Oscillator osc[MAX_POLYPHONY];
    enum {FREQ_IN, CONTROL_OUT};

  private:
};

#endif