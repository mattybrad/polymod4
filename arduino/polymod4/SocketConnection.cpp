#include "Arduino.h"
#include "SocketConnection.h"

SocketConnection::SocketConnection() {
  
}

void SocketConnection::connect(unsigned int outNum, SocketOutput& src, unsigned int inNum, SocketInput& dest, int connNum) {
  outputSocketNum = outNum;
  inputSocketNum = inNum;
  _src = &src;
  _dest = &dest;
  _src->connNum = connNum;
  _dest->connNum = connNum;
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    _patchCables[i] = new AudioConnection(src.amplifiers[i], 0, dest.amplifiers[i], 0);
  }
  inUse = true;
}

void SocketConnection::disconnect() {
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    delete _patchCables[i];
    //_patchCables[i]->disconnect();
    // was using built-in disconnect method, but it caused a memory leak
    // look on teensy forums for info on this, here's a thread i started a while back: https://forum.pjrc.com/archive/index.php/t-54760.html
  }
  _src->connNum = -1;
  _dest->connNum = -1;
  _src = NULL;
  _dest = NULL;
  checkNum = 0;
  poly = false;
  confirmed = false;
  inUse = false;
}
