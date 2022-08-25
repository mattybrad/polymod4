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
    square[i].SetFreq(261.6256f);
    square[i].SetAmp(1.0);
  }
  addPseudoConnection(FREQ_IN, AUDIO_OUT);
}

float VCO::process(int functionID, int polyChannel, int sampleNum)
{
  float returnVal = 0.0f;
  switch(functionID) {
    case AUDIO_OUT:
      if(sampleNum == 0) {
        //float newFreq = 100.0f + 33.333f * polyChannel + 50.0f * inputFloats[FREQ_IN][polyChannel];
        float newFreq = 261.6256f * pow(2.0, 10.0 * inputFloats[FREQ_IN][polyChannel]);
        square[polyChannel].SetFreq(newFreq);
      }
      returnVal = square[polyChannel].Process();
      break;
  }
  return returnVal;
}