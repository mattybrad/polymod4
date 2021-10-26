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
#include "ModuleMixer.h"
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
ModuleVCO moduleVCO2;
ModuleMixer moduleMixer;
ModuleMain moduleMain;
SocketConnection connections[MAX_CONNECTIONS];
byte connectionIndex = 0;
SocketConnection *mainConnection = NULL;

SocketOutput *socketOutputs[NUM_OUTPUT_CHANNELS];
SocketInput *socketInputs[NUM_INPUT_CHANNELS];

void setup() {
  Serial.begin(9600);
  Serial.println("polymod 4 v1");

  // initialise teensy audio hardware
  AudioMemory(500); // tweak this number for performance
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.3);

  // initialise all socket pointers as null
  for(byte i=0; i<NUM_OUTPUT_CHANNELS; i++) {
    socketOutputs[i] = NULL;
  }
  for(byte i=0; i<NUM_INPUT_CHANNELS; i++) {
    socketInputs[i] = NULL;
  }

  // set references to individual sockets
  socketOutputs[0] = &moduleSine.audioOut;
  socketOutputs[1] = &moduleVCO.audioOut;
  socketOutputs[2] = &moduleVCO2.audioOut;
  socketOutputs[3] = &moduleMixer.audioOut;
  
  socketInputs[0] = &moduleMain.audioIn;
  socketInputs[1] = &moduleVCO.freqModIn;
  socketInputs[2] = &moduleVCO2.freqModIn;
  socketInputs[3] = &moduleMixer.audioIn1;
  socketInputs[4] = &moduleMixer.audioIn2;

  // set analog pins
  moduleVCO.analogPin = 17;
  moduleVCO2.analogPin = 16;
  
  // initialise patchbay
  p.begin();
  p.setCallbacks(handleConnection, handleDisconnection);

  // optional stress test for connections/disconnections:
  if(false) {
    for(unsigned int i=0; i<50000; i++) {
      handleConnection(1,0);
      if(i%1000 == 0) delay(100);
      handleDisconnection(1,0);
      if(i%1000 == 0) delay(100);
    }
    Serial.println("stress test success");
  }
}

void loop() {
  p.update(); // need to remove delays from this function to speed up loop
  moduleVCO.update();
  moduleVCO2.update();
}

void handleConnection(unsigned int outNum, unsigned int inNum) {
  Serial.print("output ");
  Serial.print(outNum);
  Serial.print(" connected to input ");
  Serial.print(inNum);

  unsigned int i;
  bool foundAvailableConnection = false;
  // naive/slow - should probably start at last disconnection index..?
  for(i=0; i<MAX_CONNECTIONS && !foundAvailableConnection; i++) {
    if(!connections[i].inUse) {
      connections[i].connect(outNum, *socketOutputs[outNum], inNum, *socketInputs[inNum], i);
      if(inNum==0) {
        mainConnection = &connections[i];
      }
      Serial.print(", connection ");
      Serial.print(i);
      foundAvailableConnection = true;
    }
  }
  Serial.print("\n");
  calculatePolyStatuses();
}

void handleDisconnection(unsigned int outNum, unsigned int inNum) {
  Serial.print("output ");
  Serial.print(outNum);
  Serial.print(" disconnected from input ");
  Serial.print(inNum);

  // find cable using outNum and inNum (or just use polymod3 method, which is faster)
  unsigned int i;
  for(i=0; i<MAX_CONNECTIONS; i++) {
    if(connections[i].outputSocketNum==outNum && connections[i].inputSocketNum==inNum && connections[i].inUse) {
      connections[i].disconnect();
      if(inNum==0) {
        mainConnection = NULL;
      }
      Serial.print(", connection ");
      Serial.print(i);
    }
  }
  Serial.print("\n");
  calculatePolyStatuses();
}

unsigned int checkNum = 0;
void calculatePolyStatuses() {
  Serial.println("Calculating poly statuses...");
  if(mainConnection != NULL) {
    checkNum = 0;
    resetConnection(*mainConnection);
    while(checkNum < 2) {
      Serial.print("Check num = ");
      Serial.println(checkNum);
      checkConnection(*mainConnection);
      checkNum ++;
    }
  }
  Serial.println("Done with poly stuff");
}

void resetConnection(SocketConnection &c) {
  Serial.println("Reset connection...");
  c.checkNum = 0;
  c.poly = false;
  c.confirmed = false;
  for(unsigned int i=0; i<8; i++) {
    if(c._src->socketInputs[i] != NULL) {
      int connNum = c._src->socketInputs[i]->connNum;
      if(connNum >= 0) {
        if(connections[connNum].confirmed) {
          resetConnection(connections[connNum]);
        }
      }
    }
  }
}

void checkConnection(SocketConnection &c) {
  Serial.println("Check connection...");
  bool prevConfirmed = c.confirmed;
  c.checkNum ++;
  if(c._src->hardcodedPoly) {
    c.poly = true;
    c.confirmed = true;
  }
  bool allMono = true;
  bool allInputsConfirmed = true;
  for(unsigned int i=0; i<8; i++) {
    if(c._src->socketInputs[i] != NULL) {
      int connNum = c._src->socketInputs[i]->connNum;
      if(connNum >= 0) {
        if(connections[connNum].checkNum == checkNum) {
          checkConnection(connections[connNum]);
        }
        if(connections[connNum].poly) allMono = false;
        if(!connections[connNum].confirmed) allInputsConfirmed = false;
      }
    }
  }
  
  if(!allMono) {
    c.poly = true;
    c.confirmed = true;
  }
  if(allInputsConfirmed) {
    c.confirmed = true;
  }
  if(!prevConfirmed && c.confirmed) {
    Serial.print("Connection is ");
    Serial.println(c.poly ? "poly" : "mono");
    // this is where poly routing would be updated for this connection
  }
}

/*void calculatePolyStatuses() {
  int checkNum = 0; // number of times the whole tree has been checked(?)
  resetSocketInput(moduleMain.audioIn);
  while(checkNum < 2) {
    checkSocketInput(moduleMain.audioIn);
    checkNum ++;
  }
}


void checkSocket(Socket &s) {
  bool prevConfirmed = s.confirmed;
  s.checkNum ++;
  if(s.hardcodedPoly) {
    s.poly = true;
    s.confirmed = true;
  }
  bool allMono = true;
  bool allInputsConfirmed = true;
  for(byte i=0; i<MAX_MODULE_INPUTS; i++) {
    if(s.inputs[i].checkNum == checkNum) checkSocket(s); // also should check here that input[i] != socket, but not sure how to do that in C++ yet
    if(s.inputs[i].poly) allMono = false;
    if(!s.inputs[i].confirmed) allInputsConfirmed = false;
  }
  if(!allMono) {
    s.poly = true;
    s.confirmed = true;
  }
  if(allInputsConfirmed) {
    s.confirmed = true;
  }
  if(!prevConfirmed && s.confirmed) {
    s.updatePolyRouting();
  }
}

void resetSocketInput(SocketInput &s) {
  s.checkNum = 0;
  s.poly = false;
  s.confirmed = false;
  for(byte i=0; i<MAX_MODULE_INPUTS; i++) {
    if(s.inputs[i].confirmed) resetSocket(s.inputs[i]);
  }
}*/
