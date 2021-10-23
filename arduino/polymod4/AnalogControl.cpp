#include "Arduino.h"
#include "AnalogControl.h"

AnalogControl::AnalogControl() {
  
}

void AnalogControl::update() {
  float analogVal = analogRead(pin) / 1023.0;
  dc.amplitude(analogVal);
}
