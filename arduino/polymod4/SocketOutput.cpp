#include "Arduino.h"
#include "SocketOutput.h"

SocketOutput::SocketOutput() {
  for(unsigned int i=0; i<8; i++) {
    socketInputs[i] = NULL;
  }
}
