#ifndef Socket_h
#define Socket_h
#include <stdint.h>
#include "Module.h"

class Socket {
  public:
    Socket();
    void initInput();
    void initOutput();
    void (*processFunction)() = NULL;
    Socket* destSocket = NULL;
    Socket* sourceSocket = NULL;
    float inVal;
    float outVal;
     
  private:
    
};

#endif