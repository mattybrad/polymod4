#include "Module.h"

Module::Module() {
  
}

void Module::prepare() {
    for(uint8_t i=0; i<8; i++) {
        _sockets[i].outVal = _sockets[i].newOutVal;
    }
}

void Module::process() {
    
}