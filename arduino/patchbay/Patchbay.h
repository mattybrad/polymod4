#ifndef Patchbay_h
#define Patchbay_h

#include "Arduino.h"

class Patchbay
{
  public:
    Patchbay();
    void setPins(int outData, int outClock, int outLatch, int inLoad, int inClock, int inClockEnable, int inData);
    void update();
  private:
    unsigned int _read_shift_regs();
    bool _ready = false;
    int _pinOutData;
    int _pinOutClock;
    int _pinOutLatch;
    int _pinInLoad;
    int _pinInClock;
    int _pinInClockEnable;
    int _pinInData; 
    unsigned int _updateDelay = 1; // ms
    unsigned long _nextUpdate = 0; // millis, overflows after 50 days
    unsigned int _numOutputRegisters = 1; // not yet implemented
    unsigned int _numInputRegisters = 1; // not yet implemented
    unsigned int _byteNum;
    byte _inBytes[8];
    byte _prevInBytes[8];
    byte _stableCycles[8];
};

#endif
