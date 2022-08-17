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

float VCF::process(int functionID, int polyChannel, int sampleNum)
{
  float returnVal = 0.0f;
  switch (functionID)
  {
  case LPF_OUT:
    if(sampleNum == 0) {
      tempFloat = 500.0f + (490.0f * inputFloats[FREQ_IN][polyChannel]);
      float tempTempFloat = tempFloat;
      if (tempTempFloat > 0.1f)
        filter[polyChannel].SetFreq(tempTempFloat);
    }
    returnVal = filter[polyChannel].Process(inputFloats[AUDIO_IN][polyChannel]);
    break;
  }
  return returnVal;
}