#include "VCF.h"

// Module setup:
// SOCKETS
// 0 (in) Audio in
// 2 (in) Freq mod in
// 3 (out) Low pass filter out

VCF::VCF()
{
  filter.Init(48000.0f);
  filter.SetRes(0.5);
  filter.SetFreq(500.0);
}

void VCF::init() {
  addPseudoConnection(FILTER_IN, FILTER_OUT);
}

float VCF::process(int functionID)
{
  float returnVal = 0.0f;
  switch (functionID)
  {
  case FILTER_OUT:
    returnVal = filter.Process(*inputFloats[FILTER_IN]);
    break;
  }
  return returnVal;
}