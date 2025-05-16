#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, 4, 15, GEOMETRY_128_64);

const int PRG = 0;

long x;

void setup() {

  pinMode(16, OUTPUT); digitalWrite(16, LOW); delay(50); digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);
}

void loop() {

  while (digitalRead(PRG) == LOW){
    x = random(1, 100);
    display.clear();
    display.drawString(64, 22, String(x));
    display.display();
    delay(30);
  }
}
