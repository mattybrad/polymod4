#include "LFO.h"

// Module setup:
// SOCKETS
// 0 (in) Freq mod in
// 1 (out) Square wave out
// 2 (out) Saw wave out

LFO::LFO()
{
  for(int i=0; i<MAX_POLYPHONY; i++) {
    square[i].Init(_sampleRate);
    square[i].SetWaveform(Oscillator::WAVE_SIN);
    square[i].SetFreq(0.1f*(i+1));
    square[i].SetAmp(1.0);
  }
}

float LFO::process(int functionID, int polyChannel) {
  float returnVal = 0.0f;
  switch(functionID) {
    case AUDIO_OUT:
      returnVal = square[polyChannel].Process();
      break;
  }
  return returnVal;
}