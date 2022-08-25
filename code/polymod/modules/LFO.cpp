#include "LFO.h"

// Module setup:
// SOCKETS
// 0 (in) Freq mod in
// 1 (out) Square wave out
// 2 (out) Saw wave out

LFO::LFO()
{
  for(int i=0; i<MAX_POLYPHONY; i++) {
    osc[i].Init(_sampleRate);
    osc[i].SetWaveform(Oscillator::WAVE_SIN);
    osc[i].SetFreq(0.25f);
    osc[i].SetAmp(0.2);
  }
}

float LFO::process(int functionID, int polyChannel, int sampleNum)
{
  float returnVal = 0.0f;
  switch(functionID) {
    case CONTROL_OUT:
      if(sampleNum == 0) {
        osc[polyChannel].SetFreq(0.1+100*freqControl*freqControl);
      }
      returnVal = osc[polyChannel].Process();
      break;
  }
  return returnVal;
}