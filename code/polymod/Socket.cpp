#include "Socket.h"

Socket::Socket() {
  
}

void Socket::process() {
    if(socketType == OUTPUT) {
        outVal = module->process(functionID);
    } else if(socketType == INPUT) {
        inVal = sourceSocket->outVal;
    }
}