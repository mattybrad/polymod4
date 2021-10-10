#include "Patchbay.h"

Patchbay p;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  p.setPins(2,3,4,5,6,7,8);
}

void loop() {
  // put your main code here, to run repeatedly:
  p.update();
}
