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
    static float processSquareOut(Module module, float inVal);
    static float processSawOut(Module module, float inVal);

  private:
    Oscillator _square;
    Oscillator _saw;
};

#endif