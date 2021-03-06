#include "Arduino.h"
#include "SocketConnection.h"

SocketConnection::SocketConnection() {

}

void SocketConnection::connect(unsigned int outNum, SocketOutput& src, unsigned int inNum, SocketInput& dest, int connNum) {
  if(&src==NULL || &dest==NULL) {
    // not the cleanest way of detecting bad connections, but good enough to stop everything crashing for now
    Serial.print(" (BAD CONNECTION!) ");
  } else {
    outputSocketNum = outNum;
    inputSocketNum = inNum;
    _src = &src;
    _dest = &dest;
    _src->connNum = connNum;
    _dest->connNum = connNum;
    for(byte i=0; i<MAX_POLYPHONY; i++) {
      _patchCables[i] = new AudioConnection(src.amplifiers[i], 0, dest.amplifiers[i], 0);
    }
    _routingConnected = true;
    inUse = true;
  }
}

void SocketConnection::updateRouting() {
  if(!_routingConnected || (poly != _routingIsPoly)) {
    unsigned int i;
    if(_routingConnected) {
      // remove existing routing
      for(i=0; i<MAX_POLYPHONY; i++) {
        delete _patchCables[i];
      }
    }
    if(poly) {
      _routingIsPoly = true;
      for(byte i=0; i<MAX_POLYPHONY; i++) {
        _patchCables[i] = new AudioConnection(_src->amplifiers[i], 0, _dest->amplifiers[i], 0);
      }
    } else {
      _routingIsPoly = false;
      for(byte i=0; i<MAX_POLYPHONY; i++) {
        _patchCables[i] = new AudioConnection(_src->amplifiers[0], 0, _dest->amplifiers[i], 0);
      }
    }
    _routingConnected = true;
  }
}

void SocketConnection::disconnect() {
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    delete _patchCables[i];
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
  _routingConnected = false;
  _routingIsPoly = false;
}
