// prototype monophonic implementation of polymod system
// 2 output registers, 2 input registers, 4 potentiometers

#include "Patchbay.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <MIDI.h>

AudioControlSGTL5000 sgtl5000_1;

#include "ModuleSine.h"
#include "ModuleVCO.h"
#include "ModuleMain.h"
#include "SocketConnection.h"
#include "Globals.h"

#define NUM_OUTPUT_REGISTERS 2
#define NUM_INPUT_REGISTERS 2
const byte NUM_OUTPUT_CHANNELS = 8 * NUM_OUTPUT_REGISTERS;
const byte NUM_INPUT_CHANNELS = 8 * NUM_INPUT_REGISTERS;

Patchbay p(NUM_OUTPUT_REGISTERS,NUM_INPUT_REGISTERS,2,3,4,9,10,11,12);

ModuleSine moduleSine;
ModuleVCO moduleVCO;
ModuleMain moduleMain;
SocketConnection connections[MAX_CONNECTIONS];
byte connectionIndex = 0;

SocketOutput *socketOutputs[NUM_OUTPUT_CHANNELS];
SocketInput *socketInputs[NUM_INPUT_CHANNELS];


void setup() {  
  Serial.begin(9600);
  Serial.println("polymod 4 v1");

  AudioMemory(50);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.3);
  
  for(byte i=0; i<NUM_OUTPUT_CHANNELS; i++) {
    socketOutputs[i] = NULL;
  }
  for(byte i=0; i<NUM_INPUT_CHANNELS; i++) {
    socketInputs[i] = NULL;
  }  
  socketOutputs[0] = &moduleSine.audioOut;
  socketOutputs[1] = &moduleVCO.audioOut;
  socketInputs[0] = &moduleVCO.freqModIn;
  socketInputs[1] = &moduleMain.audioIn;
  
  p.begin();
  p.setCallbacks(handleConnection, handleDisconnection);

  connections[0].connect(*socketOutputs[1], *socketInputs[1]);
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
