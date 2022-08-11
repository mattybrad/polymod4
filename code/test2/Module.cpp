#include "Module.h"

Module::Module() {
  
}

float Module::processA(Module module, float inVal)
{
    return inVal + module.freq;
}

float Module::processB(Module module, float inVal)
{
    return inVal * module.freq;
}