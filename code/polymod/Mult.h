#ifndef Mult_h
#define Mult_h
#include "Module.h"
#include "daisy_seed.h"
#include "daisysp.h"
using namespace daisysp;
using namespace daisy;

class Mult : public Module {
  public:
    Mult();
    virtual float process(int functionID, int polyChannel, int sampleNum);
    enum {INPUT, OUTPUT_1, OUTPUT_2};

  private:
};

#endif