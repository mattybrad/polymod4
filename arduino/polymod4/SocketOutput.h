#ifndef SocketOutput_h
#define SocketOutput_h
#include "Arduino.h"
#include "Globals.h"
#include "SocketInput.h"
#include <Audio.h>
 
class SocketOutput {
  public:
    SocketOutput();
    AudioAmplifier amplifiers[MAX_POLYPHONY];
    SocketInput *socketInputs[8];
    int connNum = -1;
  private:
    
};

#endif
