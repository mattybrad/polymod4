#include "Socket.h"

Socket::Socket() {
  
}

void Socket::process(int sampleNum) {
    if(socketType == OUTPUT) {
        for (int i = 0; i < Module::MAX_POLYPHONY; i++)
        {
            value[i] = module->process(param, i, sampleNum);
            //module->inputFloats[param][i] = value[i]; // ???
        }
    } else if (socketType == INPUT) {
        for (int i = 0; i < Module::MAX_POLYPHONY; i++)
        {
            value[i] = sourceSocket->value[i];
            module->inputFloats[param][i] = value[i];
        }
    }
}