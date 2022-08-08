#ifndef Socket_h
#define Socket_h

#include "Module.h"

class Socket
{
public:
    Socket();
    void process();
    Module *module;
    float inVal = 0.0;
    float outVal = 0.0;

private:
};

#endif