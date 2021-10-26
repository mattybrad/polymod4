#include "Arduino.h"
#include "ModuleVCO.h"

ModuleVCO::ModuleVCO() {
  //float niceFreqs[8] = {220.0, 277.2, 329.6, 440.0, 220.0, 277.2, 329.6, 440.0};
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    _osc[i].begin(0.1, 110, WAVEFORM_SAWTOOTH);
    _modCables1[i] = new AudioConnection(freqModIn.amplifiers[i], 0, _modMixer[i], 0);
    _modCables2[i] = new AudioConnection(_freqControl.dc, 0, _modMixer[i], 1);
    _modCables3[i] = new AudioConnection(_modMixer[i], 0, _osc[i], 0);
    _oscCables[i] = new AudioConnection(_osc[i], 0, audioOut.amplifiers[i], 0);
  }
  audioOut.socketInputs[0] = &freqModIn;
  //audioOut.hardcodedPoly = true;
}

void ModuleVCO::update() {
  _freqControl.pin = analogPin; // temp
  _freqControl.update();
}
