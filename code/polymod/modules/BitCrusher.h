#ifndef BitCrusher_h
#define BitCrusher_h
#include "Module.h"
#include "daisy_seed.h"
#include "daisysp.h"
using namespace daisysp;
using namespace daisy;

class BitCrusher : public Module {
  public:
    BitCrusher();
    virtual float process(int functionID, int polyChannel, int sampleNum);
    Bitcrush bc[MAX_POLYPHONY];
    enum {AUDIO_IN, AUDIO_OUT};

  private:
};

#endif