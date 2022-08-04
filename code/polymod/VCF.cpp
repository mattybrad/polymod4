#include "VCF.h"

VCF::VCF() {
  _filter.Init(48000.0f);
  _filter.SetRes(0.7f);
  _filter.SetFreq(300.0f);
}

void VCF::process() {
  _sockets[1].outVal = _filter.Process(_sockets[0].inVal);
}