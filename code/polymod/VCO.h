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
    virtual float process(int functionID);
    Oscillator square;
    Oscillator saw;
    static const int SAW_OUT = 0;
    static const int SQUARE_OUT = 1;

  private:
};

#endif