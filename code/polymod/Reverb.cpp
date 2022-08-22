#include "Reverb.h"

// Module setup:
// SOCKETS
// 0 (in) Audio in
// 2 (in) Freq mod in
// 3 (out) Low pass filter out

Reverb::Reverb()
{
  for(int i=0; i<MAX_POLYPHONY; i++) {
    //rv[i].Init(_sampleRate);
    //rv[i].SetFeedback(0.9f);
    //rv[i].SetLpFreq(18000.0f);
  }
  addPseudoConnection(AUDIO_IN, AUDIO_OUT);
}

float Reverb::process(int functionID, int polyChannel, int sampleNum)
{
  float returnVal = 0.0f;
  switch (functionID)
  {
  case AUDIO_OUT:
    if(polyChannel==0) {
      //*rvPointer.Process(inputFloats[AUDIO_IN][polyChannel], 0.0f, &returnVal, &dummyValTemp);
    }
    //rv[polyChannel].Process(inputFloats[AUDIO_IN][polyChannel], 0.0f, &returnVal, &dummyValTemp);
    break;
  }
  return returnVal;
}