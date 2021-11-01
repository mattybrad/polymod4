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

  // find free note slot
  unsigned long oldestNoteTime = 4294967295;
  byte noteToUse = 0;
  bool foundFreeNote = false;
  byte i;
  for(i=0; i<MAX_POLYPHONY; i++) {
    if(!_notes[i].noteDown && _notes[i].timeEnded < oldestNoteTime) {
      oldestNoteTime = _notes[i].timeEnded;
      noteToUse = i;
      foundFreeNote = true;
    }
  }
  if(!foundFreeNote) {
    // if no free note slot, use lowest note
    byte lowestNoteNum = 255;
    for(i=0; i<MAX_POLYPHONY; i++) {
      if(_notes[i].noteNum <= lowestNoteNum) {
        lowestNoteNum = _notes[i].noteNum;
        noteToUse = i;
      }
    }
  }
  _notes[noteToUse].noteDown = true;
  _notes[noteToUse].noteNum = note;
  _notes[noteToUse].timeStarted = millis();
  _notes[noteToUse].timeEnded = 4294967295;
  _freqDC[noteToUse].amplitude((note-69)/12.0/8.0);
}

void ModuleMidi::noteOff(byte channel, byte note, byte velocity) {
  Serial.println("MIDI MODULE NOTE OFF");

  // find any active notes matching note off note
  byte i;
  for(i=0; i<MAX_POLYPHONY; i++) {
    if(_notes[i].noteDown && _notes[i].noteNum == note) {
      _notes[i].noteDown = false;
      _notes[i].timeEnded = millis();
    }
  }
}
