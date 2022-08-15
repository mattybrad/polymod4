#include "Socket.h"

Socket::Socket() {
  
}

void Socket::process() {
    if(socketType == OUTPUT) {
        for (int i = 0; i < Module::MAX_POLYPHONY; i++)
        {
            value[i] = module->process(param, i);
        }
    } else if (socketType == INPUT) {
        for (int i = 0; i < Module::MAX_POLYPHONY; i++)
        {
            value[i] = sourceSocket->value[i];
            module->inputFloats[param][i] = value[i];
        }
    }
}