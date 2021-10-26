#include "Arduino.h"
#include "ModuleMixer.h"

ModuleMixer::ModuleMixer() {
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    _audioMixer[i].gain(0, 1);
    _audioMixer[i].gain(1, 1);
    _mixerCables1[i] = new AudioConnection(audioIn1.amplifiers[i], 0, _audioMixer[i], 0);
    _mixerCables2[i] = new AudioConnection(audioIn2.amplifiers[i], 0, _audioMixer[i], 1);
    _mixerCables3[i] = new AudioConnection(_audioMixer[i], 0, audioOut.amplifiers[i], 0);
  }
}

void ModuleMixer::update() {
  
}
