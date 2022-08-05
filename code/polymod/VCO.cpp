#include "VCO.h"

// Module setup:
// SOCKETS
// 0 (in) Freq mod in
// 1 (out) Square wave out
// 2 (out) Saw wave out

VCO::VCO() {
  //sockets[0].initInput();
  //sockets[1].initOutput(); // should init with ref/pointer to processSquareOut
  //sockets[2].initOutput(); // should init with ref/pointer to processSawOut

  // define link between inputs and outputs, to allow ordering calculations and poly stuff
  addPseudoConnection(0,1);
  addPseudoConnection(0,2);

  _saw.Init(48000.0f);
  _saw.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
  _saw.SetFreq(100);
  _saw.SetAmp(0.25);

  _square.Init(48000.0f);
  _square.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
  _square.SetFreq(100);
  _square.SetAmp(0.25);
}

void VCO::processSquareOut() {
  //sockets[0].outVal = _square.Process();
}

void VCO::processSawOut()
{
  //sockets[1].outVal = _saw.Process();
}