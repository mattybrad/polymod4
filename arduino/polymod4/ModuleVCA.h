#ifndef ModuleVCA_h
#define ModuleVCA_h
#include "Arduino.h"
#include "Module.h"

class ModuleVCA : public Module {
  public:
    ModuleVCA();
    virtual void update();
    SocketInput audioIn;
    SocketInput controlIn;
    SocketOutput audioOut;
  private:
    AudioEffectMultiply _multiply[MAX_POLYPHONY];
    AudioConnection* _inputCables[MAX_POLYPHONY];
    AudioConnection* _controlCables[MAX_POLYPHONY];
    AudioConnection* _outputCables[MAX_POLYPHONY];
};

#endif
