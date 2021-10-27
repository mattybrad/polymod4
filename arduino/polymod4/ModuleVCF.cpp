#include "Arduino.h"
#include "ModuleVCF.h"

ModuleVCF::ModuleVCF() {
  //float niceFreqs[8] = {220.0, 277.2, 329.6, 440.0, 220.0, 277.2, 329.6, 440.0};
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    _filter[i].frequency(800);
    _filter[i].resonance(0.6);
    _filter[i].octaveControl(8);
    _modCables1[i] = new AudioConnection(freqModIn.amplifiers[i], 0, _modMixer[i], 0);
    _modCables2[i] = new AudioConnection(_freqControl.dc, 0, _modMixer[i], 1);
    _modCables3[i] = new AudioConnection(_modMixer[i], 0, _filter[i], 1);
    _inputCables[i] = new AudioConnection(audioIn.amplifiers[i], 0, _filter[i], 0);
    _outputCables[i] = new AudioConnection(_filter[i], 0, audioOut.amplifiers[i], 0);
  }
  audioOut.socketInputs[0] = &audioIn;
  audioOut.socketInputs[1] = &freqModIn;
  //audioOut.hardcodedPoly = true;
}

void ModuleVCF::update() {
  _freqControl.pin = analogPin; // temp
  _freqControl.update();
}
