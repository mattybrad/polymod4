#include "Noise.h"

Noise::Noise()
{
  for(int i=0; i<MAX_POLYPHONY; i++) {
    whiteNoise[i].Init();
    whiteNoise[i].SetAmp(0.5);
  }
}

float Noise::process(int functionID, int polyChannel, int sampleNum)
{
  float returnVal = 0.0f;
  switch(functionID) {
    case WHITE_NOISE_OUT:
      returnVal = whiteNoise[polyChannel].Process();
      break;
  }
  return returnVal;
}