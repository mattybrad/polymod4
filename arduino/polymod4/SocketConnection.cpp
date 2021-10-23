#include "Arduino.h"
#include "SocketConnection.h"

SocketConnection::SocketConnection() {
  
}

void SocketConnection::connect(SocketOutput& src, SocketInput& dest) {
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    _patchCables[i] = new AudioConnection(src.amplifiers[i], 0, dest.amplifiers[i], 0);
  }
  inUse = true;
}

void SocketConnection::disconnect() {
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    _patchCables[i]->disconnect();
  }
  inUse = false;
}
