#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, 4, 15, GEOMETRY_128_64);

const int trimmer = 36;
int thisValue;
int prevValue = -255;
int x;

void setup() {

  pinMode(16, OUTPUT); digitalWrite(16, LOW); delay(50); digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);

  display.drawString(64, 12, "Start");
  display.drawProgressBar(14, 44, 100, 10, 100);
  display.display();
}

void loop() {

  thisValue = analogRead(trimmer);
  if (abs(thisValue - prevValue) >= 10) {  //προσπάθεια για να μην κάνει fluctuate
    x = map(thisValue, 0, 4095, 100, 0);
  }
  display.clear();
  display.drawString(64, 12, String(x));
  display.drawProgressBar(14, 44, 100, 10, x);
  //display.drawString(104, 12, String(thisValue));  //τεστ
  display.display();

  prevValue = thisValue;
  delay(30);
}
