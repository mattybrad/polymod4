#include "VCO.h"

VCO::VCO() {
  _osc.Init(48000.0f); // temp sample rate, might be wrong...
  _osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
  _osc.SetFreq(100);
  _osc.SetAmp(0.25);
}

void VCO::process() {
  _osc.SetFreq(tempFreq);
  _sockets[0].outVal = _osc.Process();
}