#include "Arduino.h"
#include "ModuleMain.h"

ModuleMain::ModuleMain() {
  for(unsigned int i=0; i<4; i++) {
    for(unsigned int j=0; j<4; j++) {
      unsigned int channelNum = 4*i + j;
      if(channelNum < MAX_POLYPHONY) {
        _primaryMixerCables[4*i+j] = new AudioConnection(audioIn.amplifiers[4*i+j], 0, _primaryMixers[i], j);
      }
    }
    _secondaryMixerCables[i] = new AudioConnection(_primaryMixers[i], 0, _secondaryMixer, i);
    _secondaryMixer.gain(i,1.0/MAX_POLYPHONY);
  }
  _outputCables[0] = new AudioConnection(_secondaryMixer, 0, _i2s, 0);
  _outputCables[0] = new AudioConnection(_secondaryMixer, 0, _i2s, 1);
  /*for(byte i=0; i<MAX_POLYPHONY; i++) {
    _mixerCables[i] = new AudioConnection(audioIn.amplifiers[i], 0, _mixer, i);
  }
  _outputCables[0] = new AudioConnection(_mixer, 0, _i2s, 0);
  _outputCables[0] = new AudioConnection(_mixer, 0, _i2s, 1);*/
}

void ModuleMain::update() {

}
