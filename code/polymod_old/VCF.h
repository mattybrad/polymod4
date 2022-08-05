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
    virtual void process();
    float tempCutoff = 100.0f;
    float tempRes = 0.5f;
  private:
    MoogLadder _filter;
};

#endif