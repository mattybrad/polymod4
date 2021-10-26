#include "Arduino.h"
#include "ModulePolySource.h"

ModulePolySource::ModulePolySource() {
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    _dc[i].amplitude(0.1*i);
    _cables[i] = new AudioConnection(_dc[i], 0, audioOut.amplifiers[i], 0);
    audioOut.hardcodedPoly = true;
  }
}

void ModulePolySource::update() {
  
}
