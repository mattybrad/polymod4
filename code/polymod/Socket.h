#ifndef Socket_h
#define Socket_h
#include <stdint.h>
#include "Module.h"

class Socket {
  public:
    Socket();
    int socketNum = -1;
    int order = -1;
    bool orderSet = false;
    Module *module;
    int param = -1;
    void process();
    Socket *destSocket = nullptr;
    Socket *sourceSocket = nullptr;
    float inVal;
    float outVal;
    enum {UNUSED,INPUT,OUTPUT};
    int socketType = UNUSED;
     
  private:
    
};

#endif