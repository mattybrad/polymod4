#include "Socket.h"

Socket::Socket() {
  
}

void Socket::process()
{
    outVal = module->process(inVal);
}