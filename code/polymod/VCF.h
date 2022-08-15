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
    Oscillator tempOsc;
    enum {AUDIO_IN, LPF_OUT};

  private:
};

#endif