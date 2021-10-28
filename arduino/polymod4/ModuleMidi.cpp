#include "Arduino.h"
#include "ModuleMidi.h"

ModuleMidi::ModuleMidi() {;
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    _freqDC[i].amplitude(0);
    _freqCables[i] = new AudioConnection(_freqDC[i], 0, freqOut.amplifiers[i], 0);
  }
  freqOut.hardcodedPoly = true;
}

void ModuleMidi::update() {

}

void ModuleMidi::noteOn(byte channel, byte note, byte velocity) {
  Serial.println("MIDI MODULE NOTE ON");
  float temp = (note - 69) / 30.0;
  Serial.println(temp);
  _freqDC[0].amplitude(temp); // very much temp
}

void ModuleMidi::noteOff(byte channel, byte note, byte velocity) {
  Serial.println("MIDI MODULE NOTE OFF");
}
