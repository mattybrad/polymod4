#include "Arduino.h"
#include "Patchbay.h"

Patchbay::Patchbay()
{
  
}

void Patchbay::setPins(int outData, int outClock, int outLatch, int inLoad, int inClock, int inClockEnable, int inData)
{
  _pinOutData = outData;
  _pinOutClock = outClock;
  _pinOutLatch = outLatch;
  _pinInLoad = inLoad;
  _pinInClock = inClock;
  _pinInClockEnable = inClockEnable;
  _pinInData = inData;
  pinMode(_pinOutData,OUTPUT);
  pinMode(_pinOutClock,OUTPUT);
  pinMode(_pinOutLatch,OUTPUT);
  pinMode(_pinInLoad,OUTPUT);
  pinMode(_pinInClock,OUTPUT);
  pinMode(_pinInClockEnable,OUTPUT);
  pinMode(_pinInData,INPUT);
  _ready = true;
}

void Patchbay::setCallbacks(ConnectionCallback connectionCallback, ConnectionCallback disconnectionCallback)
{
  _connectionCallback = connectionCallback;
  _disconnectionCallback = disconnectionCallback;
}

void Patchbay::update()
{
  if(_ready) {
    if(millis()>=_nextUpdate) {
      _nextUpdate += _updateDelay;

      // ADD MORE COMMENTS HERE, THIS WILL MAKE NO SENSE TO ANYONE

      byte b_send[_numOutputRegisters];
      for(int i=0; i<_numOutputRegisters; i++) {
        b_send[i] = B00000000;
        for(int j=0; j<8; j++) {
          // for each channel number
          bitWrite(b_send[i], j, bitRead(j+1+8*i, _bitPosition));
        }
      }
      for(int i=0; i<_numOutputRegisters; i++) {
        // shift out byte b[i]
        digitalWrite(_pinOutLatch, LOW);
        shiftOut(_pinOutData, _pinOutClock, MSBFIRST, b_send[_numOutputRegisters-i-1]);
        digitalWrite(_pinOutLatch, HIGH);
      }

      delayMicroseconds(100); // could maybe eliminate or reduce this? shouldn't have blocking delay if possible
  
      // shift in read from shift out

      byte b_recv[_numInputRegisters];
      digitalWrite(_pinInClockEnable, HIGH);
      digitalWrite(_pinInLoad, LOW);
      delayMicroseconds(5);
      digitalWrite(_pinInLoad, HIGH);
      digitalWrite(_pinInClockEnable, LOW);
      for(int i=0; i<_numInputRegisters; i++) {
        b_recv[i] = B00000000;

        for(int j=0; j<8; j++) {
          bool thisBit = digitalRead(_pinInData);
          bitWrite(b_recv[i], 7-j, thisBit);

          digitalWrite(_pinInClock, HIGH);
          delayMicroseconds(5);
          digitalWrite(_pinInClock, LOW);
        }
      }

      for(int i=0; i<16; i++) {
        // for each channel number
        bitWrite(_inBytes[i], _bitPosition, bitRead(b_recv[i/8], i%8));
        if(_bitPosition==7) {
          if(_prevInBytes[i] != _inBytes[i]) {
            _stableCycles[i] = 0;
          } else if(_stableCycles[i]<3) {
            _stableCycles[i] ++;
          }
          if(_stableCycles[i]==2) {
            if(_inBytes[i]) {
              // connection
              if(_inBytes[i] != _stableBytes[i]) {
                _connectionCallback(_inBytes[i]-1,i);
                _stableBytes[i] = _inBytes[i];
              }
            } else {
              if(_stableBytes[i] != 0) {
                _disconnectionCallback(_stableBytes[i]-1,i);
                _stableBytes[i] = 0;
              }
            }
          }
          _prevInBytes[i] = _inBytes[i];
        }
      }
      
      _bitPosition = (_bitPosition + 1) % 8;
    }
  }
}
