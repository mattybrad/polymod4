#include "VCO.h"

// Module setup:
// SOCKETS
// 0 (in) Freq mod in
// 1 (out) Square wave out
// 2 (out) Saw wave out

VCO::VCO()
{
  for(int i=0; i<MAX_POLYPHONY; i++) {
    square[i].Init(_sampleRate);
    square[i].SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
    square[i].SetFreq(100+33.333f*i);
    square[i].SetAmp(1.0);
  }
}

float VCO::process(int functionID, int polyChannel, int sampleNum)
{
  float returnVal = 0.0f;
  switch(functionID) {
    case AUDIO_OUT:
      returnVal = square[polyChannel].Process();
      break;
  }
  return returnVal;
}