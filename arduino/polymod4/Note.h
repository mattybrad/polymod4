#ifndef Note_h
#define Note_h
#include "Arduino.h"

class Note {
  public:
    Note();
    unsigned long timeStarted = 0;
    unsigned long timeEnded = 0;
    byte noteNum = 0;
    bool noteDown = false;
  private:

};

#endif
