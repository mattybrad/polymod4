#include "VCO.h"

// Module setup:
// SOCKETS
// 0 (in) Freq mod in
// 1 (out) Square wave out
// 2 (out) Saw wave out

VCO::VCO()
{
  saw.Init(_sampleRate);
  saw.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
  saw.SetFreq(100);
  saw.SetAmp(1.0);

  square.Init(_sampleRate);
  square.SetWaveform(Oscillator::WAVE_POLYBLEP_SQUARE);
  square.SetFreq(100);
  square.SetAmp(1.0);

  sine.Init(_sampleRate);
  sine.SetWaveform(Oscillator::WAVE_SIN);
  sine.SetFreq(100);
  sine.SetAmp(1.0);
}

float VCO::process(int functionID, int polyChannel)
{
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
    case SINE_OUT:
      sine.SetFreq(freq);
      returnVal = sine.Process();
      break;
  }
  return returnVal;
}