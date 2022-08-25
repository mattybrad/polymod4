#ifndef Noise_h
#define Noise_h
#include "Module.h"
#include "daisy_seed.h"
#include "daisysp.h"
using namespace daisysp;
using namespace daisy;

class Noise : public Module {
  public:
    Noise();
    virtual float process(int functionID, int polyChannel, int sampleNum);
    WhiteNoise whiteNoise[MAX_POLYPHONY];
    enum {WHITE_NOISE_OUT};

  private:
};

#endif