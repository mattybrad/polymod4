#include "Arduino.h"
#include "ModuleMain.h"

ModuleMain::ModuleMain() {
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    _mixerCables[i] = new AudioConnection(audioIn.amplifiers[i], 0, _mixer, i);
  }
  _outputCables[0] = new AudioConnection(_mixer, 0, _i2s, 0);
  _outputCables[0] = new AudioConnection(_mixer, 0, _i2s, 1);
}

void ModuleMain::update() {
  
}
