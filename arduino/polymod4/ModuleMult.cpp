#include "Arduino.h"
#include "ModuleMult.h"

ModuleMult::ModuleMult() {
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    _out1Cables[i] = new AudioConnection(in.amplifiers[i], 0, out1.amplifiers[i], 0);
    _out2Cables[i] = new AudioConnection(in.amplifiers[i], 0, out2.amplifiers[i], 0);
  }
  out1.socketInputs[0] = &in;
  out2.socketInputs[0] = &in;
}

void ModuleMult::update() {

}
