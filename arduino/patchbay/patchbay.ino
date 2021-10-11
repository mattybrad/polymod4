#include "Patchbay.h"

Patchbay p;

void setup() {
  Serial.begin(9600);
  p.setPins(2,3,4,5,6,7,8);
  p.setCallbacks(handleConnection, handleDisconnection);
}

void loop() {
  p.update();
  
  // handle any connections or disconnections
  
}

void handleConnection(unsigned int outNum, unsigned int inNum) {
  Serial.print("con ");
  Serial.print(outNum);
  Serial.print("---");
  Serial.println(inNum);
}

void handleDisconnection(unsigned int outNum, unsigned int inNum) {
  Serial.print("dis ");
  Serial.print(outNum);
  Serial.print("-x-");
  Serial.println(inNum);
}
