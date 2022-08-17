#ifndef VCF_h
#define VCF_h
#include "Module.h"
#include "daisy_seed.h"
#include "daisysp.h"
using namespace daisysp;
using namespace daisy;

class VCF : public Module {
  public:
    VCF();
    virtual float process(int functionID, int polyChannel);
    MoogLadder filter[MAX_POLYPHONY];
    float tempFloat = 50.0f;
    float badFloat = 999.99f;
    enum {AUDIO_IN, FREQ_IN, LPF_OUT};

  private:
};

#endif