#ifndef VCA_h
#define VCA_h
#include "Module.h"
#include "daisy_seed.h"
#include "daisysp.h"
using namespace daisysp;
using namespace daisy;

class VCA : public Module {
  public:
    VCA();
    virtual float process(int functionID, int polyChannel, int sampleNum);
    enum {AUDIO_IN, CONTROL_IN, AUDIO_OUT};

  private:
};

#endif