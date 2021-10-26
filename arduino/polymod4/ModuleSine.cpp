#include "Arduino.h"
#include "ModuleSine.h"

ModuleSine::ModuleSine() {
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    _sineOsc[i].amplitude(0.1);
    _sineOsc[i].frequency(0.5);
    _sineOscCables[i] = new AudioConnection(_sineOsc[i], 0, audioOut.amplifiers[i], 0);
    //audioOut.hardcodedPoly = true;
  }
}

void ModuleSine::update() {
  
}
