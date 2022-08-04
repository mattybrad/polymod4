#include "VCF.h"

VCF::VCF() {
  _filter.Init(48000.0f);
}

void VCF::process() {
  _filter.SetRes(tempRes);
  _filter.SetFreq(tempCutoff);
  _sockets[1].outVal = _filter.Process(_sockets[0].inVal);
}