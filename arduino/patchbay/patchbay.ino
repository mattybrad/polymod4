#include "Patchbay.h"

Patchbay p(2,2,2,3,4,9,10,11,12);

void setup() {
  Serial.begin(9600);
  p.begin();
  p.setCallbacks(handleConnection, handleDisconnection);
}

void loop() {
  p.update();
}

void handleConnection(unsigned int outNum, unsigned int inNum) {
  Serial.print("output ");
  Serial.print(outNum);
  Serial.print(" connected to input ");
  Serial.println(inNum);
}

void handleDisconnection(unsigned int outNum, unsigned int inNum) {
  Serial.print("output ");
  Serial.print(outNum);
  Serial.print(" disconnected from input ");
  Serial.println(inNum);
}
