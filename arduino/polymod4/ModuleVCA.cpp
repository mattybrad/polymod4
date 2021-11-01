#include "Arduino.h"
#include "ModuleVCA.h"

ModuleVCA::ModuleVCA() {
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    _inputCables[i] = new AudioConnection(audioIn.amplifiers[i], 0, _multiply[i], 0);
    _controlCables[i] = new AudioConnection(controlIn.amplifiers[i], 0, _multiply[i], 1);
    _outputCables[i] = new AudioConnection(_multiply[i], 0, audioOut.amplifiers[i], 0);
  }
  audioOut.socketInputs[0] = &audioIn;
  audioOut.socketInputs[1] = &controlIn;
}

void ModuleVCA::update() {

}
