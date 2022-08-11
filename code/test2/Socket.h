#ifndef Socket_h
#define Socket_h

#include "Module.h"

class Socket
{
public:
    Socket();
    void process();
    Module *module;
    float (*moduleFunction)(Module module, float inVal);
    float inVal = 1.0;
    float outVal = 1.0;

private:
};

#endif