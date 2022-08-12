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
    virtual float process(int functionID, int polyChannel);
    float freq = 300.0;
    Oscillator square;
    Oscillator saw;
    Oscillator sine;
    enum {SQUARE_OUT, SAW_OUT, SINE_OUT};

  private:
};

#endif