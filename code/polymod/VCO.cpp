#include "VCO.h"

VCO::VCO() {
  _osc.Init(44100); // temp sample rate
  _osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
  _osc.SetFreq(100);
  _osc.SetAmp(0.25);
}

void VCO::process() {
  _sockets[0].outVal = _osc.Process();
}