#ifndef Patchbay_h
#define Patchbay_h

#include "Arduino.h"

using ConnectionCallback = void (*)(unsigned int outNum, unsigned int inNum);

class Patchbay
{
  public:
    Patchbay();
    void setPins(int outData, int outClock, int outLatch, int inLoad, int inClock, int inClockEnable, int inData);
    void setCallbacks(ConnectionCallback connectionCallback, ConnectionCallback disconnectionCallback);
    void update();
  private:
    ConnectionCallback _connectionCallback;
    ConnectionCallback _disconnectionCallback;
    unsigned int _read_shift_regs();
    bool _ready = false;
    unsigned int _numOutputRegisters = 2;
    unsigned int _numInputRegisters = 2;
    int _pinOutData;
    int _pinOutClock;
    int _pinOutLatch;
    int _pinInLoad;
    int _pinInClock;
    int _pinInClockEnable;
    int _pinInData; 
    unsigned int _updateDelay = 1; // ms
    unsigned long _nextUpdate = 0; // millis, overflows after 50 days
    unsigned int _bitPosition;
    byte _inBytes[16];
    byte _prevInBytes[16];
    byte _stableCycles[16];
    byte _stableBytes[16];
};

#endif
