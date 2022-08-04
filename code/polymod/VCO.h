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
    virtual void process();
    float tempFreq = 100.0f;
  private:
    Oscillator _osc;
};

#endif