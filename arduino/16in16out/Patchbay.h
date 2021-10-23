#ifndef Patchbay_h
#define Patchbay_h

#include "Arduino.h"

// could almost go up to 31 or maybe 32 of each type of register (max channel number is 254 because 0 means no connection)
// but using 8 as a sensible big number for now, would probably be risky to daisy chain more
#define MAX_OUTPUT_REGISTERS 8
#define MAX_INPUT_REGISTERS 8

using ConnectionCallback = void (*)(unsigned int outNum, unsigned int inNum);

class Patchbay
{
  public:
    Patchbay(int numOutputRegisters, int numInputRegisters, int outData, int outClock, int outLatch, int inLoad, int inClock, int inClockEnable, int inData);
    void begin();
    void setCallbacks(ConnectionCallback connectionCallback, ConnectionCallback disconnectionCallback);
    void update();
  private:
    ConnectionCallback _connectionCallback;
    ConnectionCallback _disconnectionCallback;
    bool _ready = false;
    unsigned int _numOutputRegisters = 1;
    unsigned int _numInputRegisters = 1;
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
    byte _inBytes[8*MAX_INPUT_REGISTERS];
    byte _prevInBytes[8*MAX_INPUT_REGISTERS];
    byte _stableCycles[8*MAX_INPUT_REGISTERS];
    byte _stableBytes[8*MAX_INPUT_REGISTERS];
};

#endif
