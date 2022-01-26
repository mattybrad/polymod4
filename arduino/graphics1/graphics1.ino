#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup(void) {
  u8g2.begin();
}

void loop(void) {
  int lineHeight = 15;
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_9x15_tf);	// choose a suitable font
  u8g2.drawStr(0,1*lineHeight,"Polymod 4");	// write something to the internal memory
  u8g2.drawStr(0,2*lineHeight,"Menu system");  // write something to the internal memory
  u8g2.drawStr(0,3*lineHeight,"Hello");  // write something to the internal memory
  u8g2.drawStr(0,4*lineHeight,"Something");  // write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(1000);  
}
