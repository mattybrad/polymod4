#ifndef Socket_h
#define Socket_h
#include <stdint.h>
#include "Module.h"

class Socket {
  public:
    Socket();
    Module *module;
    float (*processFunction)(Module module, float inVal) = NULL;
    void process();
    Socket* destSocket = NULL;
    Socket* sourceSocket = NULL;
    float inVal;
    float outVal;
    int socketType;
    static const int INPUT = 0;
    static const int OUTPUT = 1;
     
  private:
    
};

#endif