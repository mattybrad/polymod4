#ifndef SocketOutput_h
#define SocketOutput_h
#include "Arduino.h"
#include "Globals.h"
#include <Audio.h>
 
class SocketOutput {
  public:
    SocketOutput();
    AudioAmplifier amplifiers[MAX_POLYPHONY];
    
  private:
    
};

#endif
