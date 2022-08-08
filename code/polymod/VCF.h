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
    virtual float process(int functionID);
    MoogLadder filter;
    static const int FILTER_IN = 0;
    static const int FILTER_OUT = 1;
    float *filterIn = nullptr;

  private:
};

#endif