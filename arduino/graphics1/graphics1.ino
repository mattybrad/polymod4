#include <Arduino.h>
#include <U8g2lib.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <synth_simple_drum.h>

// GUItool: begin automatically generated code
AudioSynthSimpleDrum     drum2;          //xy=399,244
AudioSynthSimpleDrum     drum3;          //xy=424,310
AudioSynthSimpleDrum     drum1;          //xy=431,197
AudioSynthSimpleDrum     drum4;          //xy=464,374
AudioMixer4              mixer1;         //xy=737,265
AudioOutputI2S           i2s1;           //xy=979,214
AudioConnection          patchCord1(drum2, 0, mixer1, 1);
AudioConnection          patchCord2(drum3, 0, mixer1, 2);
AudioConnection          patchCord3(drum1, 0, mixer1, 0);
AudioConnection          patchCord4(drum4, 0, mixer1, 3);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=930,518
// GUItool: end automatically generated code

static uint32_t next;

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup(void) {

  Serial.begin(115200);

  // audio library init
  AudioMemory(15);

  next = millis() + 1000;

  AudioNoInterrupts();

  drum1.frequency(60);
  drum1.length(1500);
  drum1.secondMix(0.0);
  drum1.pitchMod(0.55);
  
  drum2.frequency(60);
  drum2.length(300);
  drum2.secondMix(0.0);
  drum2.pitchMod(1.0);
  
  drum3.frequency(550);
  drum3.length(400);
  drum3.secondMix(1.0);
  drum3.pitchMod(0.5);

  drum4.frequency(1200);
  drum4.length(150);
  drum4.secondMix(0.0);
  drum4.pitchMod(0.0);
  
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  AudioInterrupts();

  u8g2.begin();

  int lineHeight = 15;
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_9x15_tf);  // choose a suitable font
  u8g2.drawStr(0,1*lineHeight,"Polymod 4"); // write something to the internal memory
  u8g2.drawStr(0,2*lineHeight,"System test");  // write something to the internal memory
  u8g2.drawStr(0,3*lineHeight,"Audio!");  // write something to the internal memory
  u8g2.drawStr(0,4*lineHeight,"Display!");  // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  //delay(1000);
}

void loop(void) {
  static uint32_t num = 0;

  if(millis() == next)
  {
    next = millis() + 1000;

    switch(num % 4)
    {
      case 0:
        drum1.noteOn();
        break;
      case 1:
        drum2.noteOn();
        break;
      case 2:
        drum3.noteOn();
        break;
      case 3:
        drum4.noteOn();
        break;
    }
    num++;

    Serial.print("Diagnostics: ");
    Serial.print(AudioProcessorUsageMax());
    Serial.print(" ");
    Serial.println(AudioMemoryUsageMax());
    AudioProcessorUsageMaxReset();
  }
}
