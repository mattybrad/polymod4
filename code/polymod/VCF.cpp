#include "VCF.h"

// Module setup:
// SOCKETS
// 0 (in) Audio in
// 2 (in) Freq mod in
// 3 (out) Low pass filter out

VCF::VCF()
{
  filter.Init(_sampleRate);
  filter.SetRes(0.5);
  filter.SetFreq(500.0);
  addPseudoConnection(AUDIO_IN, LPF_OUT);
  addPseudoConnection(FREQ_IN, LPF_OUT);
}

float VCF::process(int functionID)
{
  float returnVal = 0.0f;
  switch (functionID)
  {
  case LPF_OUT:
    filter.SetFreq(500.0f + 400.0f * *inputFloats[FREQ_IN]);
    returnVal = filter.Process(*inputFloats[AUDIO_IN]);
    break;
  }
  return returnVal;
}