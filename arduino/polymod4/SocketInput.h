#ifndef SocketInput_h
#define SocketInput_h
#include "Arduino.h"
#include "Globals.h"
#include <Audio.h>
 
class SocketInput {
  public:
    SocketInput();
    AudioAmplifier amplifiers[MAX_POLYPHONY];
    
  private:
    
};

#endif
