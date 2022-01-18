#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <MIDI.h>

AudioControlSGTL5000 sgtl5000_1;

AudioSynthWaveformSine   sine1;
AudioAmplifier           amp1;
AudioOutputI2S           i2s1;
AudioConnection          patchCord2(amp1, 0, i2s1, 0);
AudioConnection          patchCord3(amp1, 0, i2s1, 1);
AudioConnection* conns[10];

void setup() {
  Serial.begin(9600);
  Serial.println("polymod disconnection test");

  // initialise teensy audio hardware
  AudioMemory(1000); // tweak this number for performance
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  sine1.amplitude(0.5);
  sine1.frequency(200);
}

void loop() {
  conns[0] = new AudioConnection(sine1,0,amp1,0);
  delay(10);
  delete conns[0];
  //conns[0]->disconnect();
  delay(10);
}
