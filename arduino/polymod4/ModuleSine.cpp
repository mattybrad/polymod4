#include "Arduino.h"
#include "ModuleSine.h"

ModuleSine::ModuleSine() {
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    _sineOsc[i].amplitude(0.2);
    _sineOsc[i].frequency(0.1*pow(2,i));
    _sineOscCables[i] = new AudioConnection(_sineOsc[i], 0, audioOut.amplifiers[i], 0);
    //audioOut.hardcodedPoly = true;
  }
}

void ModuleSine::update() {
  
}
