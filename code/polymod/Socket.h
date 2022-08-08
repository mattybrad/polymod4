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
    int functionID = -1;
    void process();
    Socket* destSocket = nullptr;
    Socket *sourceSocket = nullptr;
    Socket *pseudoSourceTemp = nullptr;
    float inVal;
    float outVal;
    static const int UNUSED = 0;
    static const int INPUT = 1;
    static const int OUTPUT = 2;
    int socketType = UNUSED;
     
  private:
    
};

#endif