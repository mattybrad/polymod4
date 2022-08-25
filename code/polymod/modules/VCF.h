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
    virtual float process(int functionID, int polyChannel, int sampleNum);
    MoogLadder filter[MAX_POLYPHONY];
    enum {AUDIO_IN, FREQ_IN, LPF_OUT};

  private:
};

#endif