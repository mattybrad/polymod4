#ifndef Mixer_h
#define Mixer_h
#include "Module.h"
#include "daisy_seed.h"
#include "daisysp.h"
using namespace daisysp;
using namespace daisy;

class Mixer : public Module {
  public:
    Mixer();
    virtual float process(int functionID, int polyChannel, int sampleNum);
    enum {INPUT_1, INPUT_2, OUTPUT};

  private:
};

#endif