#include "VCF.h"

VCF::VCF()
{
  filter.Init(48000.0f);
  filter.SetRes(0.5);
  filter.SetFreq(500.0);
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