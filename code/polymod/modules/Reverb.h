#ifndef Reverb_h
#define Reverb_h
#include "Module.h"
#include "daisy_seed.h"
#include "daisysp.h"
using namespace daisysp;
using namespace daisy;

class Reverb : public Module {
  public:
    Reverb();
    ReverbSc *rvPointer;
    virtual float process(int functionID, int polyChannel, int sampleNum);
    // ReverbSc DSY_SDRAM_BSS rv[MAX_POLYPHONY];
    // daisy reverb class needs to use external SRAM, but can't be done inside a class like this, needs to be global - figure out using pointers etc?
    // https://forum.electro-smith.com/t/sdram-section-attribute/2459/2
    enum {AUDIO_IN, AUDIO_OUT};

  private:
};

#endif