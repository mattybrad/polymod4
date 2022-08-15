#include "VCF.h"

// Module setup:
// SOCKETS
// 0 (in) Audio in
// 2 (in) Freq mod in
// 3 (out) Low pass filter out

VCF::VCF()
{
  for(int i=0; i<MAX_POLYPHONY; i++) {
    filter[i].Init(_sampleRate);
    filter[i].SetRes(0.5);
    filter[i].SetFreq(500.0);
  }
  addPseudoConnection(AUDIO_IN, LPF_OUT);
  addPseudoConnection(FREQ_IN, LPF_OUT);
}

float VCF::process(int functionID)
{
  float returnVal = 0.0f;
  switch (functionID)
  {
  case LPF_OUT:
    //filter.SetFreq(500.0f + 400.0f * *inputFloats[FREQ_IN]);
    //returnVal = filter.Process(*inputFloats[AUDIO_IN]);
    break;
  }
  return 2.0f * returnVal;
}