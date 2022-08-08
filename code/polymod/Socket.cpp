#include "Socket.h"

Socket::Socket() {
  
}

void Socket::process() {
    if(socketType == OUTPUT) {
        outVal = processFunction(*module, inVal);
    } else if(socketType == INPUT) {
        inVal = sourceSocket->outVal;
    }
}