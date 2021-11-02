#ifndef ModuleMult_h
#define ModuleMult_h
#include "Arduino.h"
#include "Module.h"

class ModuleMult : public Module {
  public:
    ModuleMult();
    virtual void update();
    SocketInput in;
    SocketOutput out1;
    SocketOutput out2;
  private:
    AudioConnection* _out1Cables[MAX_POLYPHONY];
    AudioConnection* _out2Cables[MAX_POLYPHONY];
};

#endif
