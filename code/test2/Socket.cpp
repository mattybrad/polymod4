#include "Socket.h"

Socket::Socket() {
  
}

void Socket::process()
{
    outVal = moduleFunction(*module, inVal);
}