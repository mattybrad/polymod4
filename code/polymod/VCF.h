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
    //virtual void prepare();
    virtual void process();
    float tempFreq = 100.0f;
  private:
    MoogLadder _filter;
};

#endif