#ifndef Socket_h
#define Socket_h
#include <stdint.h>
#include "Module.h"

class Socket {
  public:
    Socket();
    int order = 999; // temp
    bool orderSet = false;
    Module *module;
    int param = -1;
    void process();
    Socket *destSocket = nullptr;
    Socket *sourceSocket = nullptr;
    float value[Module::MAX_POLYPHONY];
    enum {UNUSED,INPUT,OUTPUT};
    int socketType = UNUSED;
     
  private:
    
};

#endif