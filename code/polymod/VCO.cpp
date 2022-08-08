#include "VCO.h"

// Module setup:
// SOCKETS
// 0 (in) Freq mod in
// 1 (out) Square wave out
// 2 (out) Saw wave out

VCO::VCO() {
  saw.Init(48000.0f);
  saw.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
  saw.SetFreq(100);
  saw.SetAmp(0.25);

  square.Init(48000.0f);
  square.SetWaveform(Oscillator::WAVE_POLYBLEP_SQUARE);
  square.SetFreq(100);
  square.SetAmp(0.25);
}

float VCO::process(int functionID) {
  float returnVal = 0.0f;
  switch(functionID) {
    case SQUARE_OUT:
      square.SetFreq(freq);
      returnVal = square.Process();
      break;
    case SAW_OUT:
      saw.SetFreq(freq);
      returnVal = saw.Process();
      break;
  }
  return returnVal;
}