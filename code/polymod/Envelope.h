#ifndef Envelope_h
#define Envelope_h
#include "Module.h"
#include "daisy_seed.h"
#include "daisysp.h"
using namespace daisysp;
using namespace daisy;

class Envelope : public Module {
  public:
    Envelope();
    virtual float process(int functionID, int polyChannel, int sampleNum);
    Adsr adsr[MAX_POLYPHONY];
    enum {GATE_IN, CONTROL_OUT};

  private:
};

#endif