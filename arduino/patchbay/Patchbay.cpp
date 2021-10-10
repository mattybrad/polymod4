#include "Arduino.h"
#include "Patchbay.h"

// this stuff below is copy-pasted, clean up later...

/* How many shift register chips are daisy-chained.
*/
#define NUMBER_OF_SHIFT_CHIPS   1

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

void Patchbay::update()
{
  if(_ready) {
    if(millis()>=_nextUpdate) {
      _nextUpdate += _updateDelay;

      // ADD MORE COMMENTS HERE, THIS WILL MAKE NO SENSE TO ANYONE

      // calculate byte value
      byte b = B00000000;
      for(int j=0; j<8; j++) {
        // for each channel number
        bitWrite(b, j, bitRead(j+1, _byteNum));
      }
  
      // shift out byte b
      digitalWrite(_pinOutLatch, LOW);
      shiftOut(_pinOutData, _pinOutClock, MSBFIRST, b);
      digitalWrite(_pinOutLatch, HIGH);
      delay(1);
  
      // shift in read from shift out
      byte testByte = _read_shift_regs();
      for(int j=0; j<8; j++) {
        // for each channel number
        bitWrite(_inBytes[j], _byteNum, bitRead(testByte, j));
        if(_byteNum==7) {
          if(_prevInBytes[j] != _inBytes[j]) {
            _stableCycles[j] = 0;
          } else if(_stableCycles[j]<3) {
            _stableCycles[j] ++;
          }
          if(_stableCycles[j]==2) {
            if(_inBytes[j]) {
              // connection
              Serial.print(_inBytes[j]-1);
              Serial.print("->");
              Serial.println(j);
            } else {
              Serial.println("disconnection");
            }
          }
          _prevInBytes[j] = _inBytes[j];
        }
      }
      
      _byteNum = (_byteNum + 1) % 8;
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
