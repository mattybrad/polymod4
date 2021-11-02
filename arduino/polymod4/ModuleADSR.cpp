#include "Arduino.h"
#include "ModuleADSR.h"

ModuleADSR::ModuleADSR() {
  _dc.amplitude(1.0);
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    _env[i].attack(2);
    _env[i].decay(100);
    _env[i].sustain(0.4);
    _env[i].release(500);
    _queue[i].begin();
    _dcCables[i] = new AudioConnection(_dc, 0, _env[i], 0);
    _inputCables[i] = new AudioConnection(gateIn.amplifiers[i], 0, _queue[i], 0);
    _outputCables[i] = new AudioConnection(_env[i], 0, controlOut.amplifiers[i], 0);
  }
  controlOut.socketInputs[0] = &gateIn;
}

void ModuleADSR::tempChangeSettings() {
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    _env[i].attack(500);
    _env[i].decay(1000);
    _env[i].sustain(0.2);
    _env[i].release(2000);
  }
}

void ModuleADSR::update() {
  // probably won't work properly if audio block size changes..? temp code
  for(byte i=0; i<MAX_POLYPHONY; i++) {
    if(_queue[i].available() > 0) {
      int16_t test[128];
      memcpy(test, _queue[i].readBuffer(), 256);
      _queue[i].freeBuffer();
      int16_t x = test[0];
      boolean newGateReading = (x > 0);
      if(newGateReading != _gateOpen[i]) {
        _gateOpen[i] = newGateReading;
        if(_gateOpen[i]) _env[i].noteOn();
        else _env[i].noteOff();
      }
      _queue[i].clear();
    }
  }
}
