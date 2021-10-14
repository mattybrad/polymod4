#include "Arduino.h"
#include "Patchbay.h"

// this stuff below is copy-pasted, clean up later...

/* How many shift register chips are daisy-chained.
*/
#define NUMBER_OF_SHIFT_CHIPS   2

/* Width of data (how many ext lines).
*/
#define DATA_WIDTH   NUMBER_OF_SHIFT_CHIPS * 8

/* Width of pulse to trigger the shift register to read and latch.
*/
#define PULSE_WIDTH_USEC   5

/* Optional delay between shift register reads.
*/
#define POLL_DELAY_MSEC   1

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

      byte b[_numOutputRegisters];
      for(int i=0; i<_numOutputRegisters; i++) {
        b[i] = B00000000;
        for(int j=0; j<8; j++) {
          // for each channel number
          bitWrite(b[i], j, bitRead(j+1+8*i, _bitPosition));
        }
      }
      for(int i=0; i<_numOutputRegisters; i++) {
        // shift out byte b[i]
        digitalWrite(_pinOutLatch, LOW);
        shiftOut(_pinOutData, _pinOutClock, MSBFIRST, b[_numOutputRegisters-i-1]);
        digitalWrite(_pinOutLatch, HIGH);
      }

      delayMicroseconds(100); // could maybe eliminate or reduce this? shouldn't have blocking delay if possible
  
      // shift in read from shift out
      //byte testByte = _read_shift_regs();

      byte bb[_numInputRegisters];
      digitalWrite(_pinInClockEnable, HIGH);
      digitalWrite(_pinInLoad, LOW);
      delayMicroseconds(PULSE_WIDTH_USEC);
      digitalWrite(_pinInLoad, HIGH);
      digitalWrite(_pinInClockEnable, LOW);
      for(int i=0; i<_numInputRegisters; i++) {
        bb[i] = B00000000;

        for(int j=0; j<8; j++) {
          bool thisBit = digitalRead(_pinInData);
          bitWrite(bb[i], 7-j, thisBit);

          digitalWrite(_pinInClock, HIGH);
          delayMicroseconds(PULSE_WIDTH_USEC);
          digitalWrite(_pinInClock, LOW);
        }
      }

      for(int j=0; j<16; j++) {
        // for each channel number
        byte test = j/8;
        bitWrite(_inBytes[j], _bitPosition, bitRead(bb[j/8], j%8));
        if(_bitPosition==7) {
          //Serial.print(j);
          //Serial.print(" ");
          //Serial.println(_inBytes[j]);
          if(_prevInBytes[j] != _inBytes[j]) {
            _stableCycles[j] = 0;
          } else if(_stableCycles[j]<3) {
            _stableCycles[j] ++;
          }
          if(_stableCycles[j]==2) {
            if(_inBytes[j]) {
              // connection
              if(_inBytes[j] != _stableBytes[j]) {
                _connectionCallback(_inBytes[j]-1,j);
                _stableBytes[j] = _inBytes[j];
              }
            } else {
              if(_stableBytes[j] != 0) {
                _disconnectionCallback(_stableBytes[j]-1,j);
                _stableBytes[j] = 0;
              }
            }
          }
          _prevInBytes[j] = _inBytes[j];
        }
      }
      
      _bitPosition = (_bitPosition + 1) % 8;
    }
  }
}

// copy-pasted function, maybe tidy up...
unsigned int Patchbay::_read_shift_regs()
{
    long bitVal;
    unsigned int bytesVal = 0;

    /* Trigger a parallel Load to latch the state of the data lines,
    */
    digitalWrite(_pinInClockEnable, HIGH);
    digitalWrite(_pinInLoad, LOW);
    delayMicroseconds(PULSE_WIDTH_USEC);
    digitalWrite(_pinInLoad, HIGH);
    digitalWrite(_pinInClockEnable, LOW);

    /* Loop to read each bit value from the serial out line
     * of the SN74HC165N.
    */
    for(int i = 0; i < DATA_WIDTH; i++)
    {
        bitVal = digitalRead(_pinInData);

        /* Set the corresponding bit in bytesVal.
        */
        bytesVal |= (bitVal << ((DATA_WIDTH-1) - i));

        /* Pulse the Clock (rising edge shifts the next bit).
        */
        digitalWrite(_pinInClock, HIGH);
        delayMicroseconds(PULSE_WIDTH_USEC);
        digitalWrite(_pinInClock, LOW);
    }

    return(bytesVal);
}
