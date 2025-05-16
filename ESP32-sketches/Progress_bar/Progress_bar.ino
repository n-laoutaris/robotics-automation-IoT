#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, 4, 15, GEOMETRY_128_64);

const int LB = 27;
const int RB = 26;

int x = 50;

void setup() {

  pinMode(LB, INPUT_PULLUP); pinMode(RB, INPUT_PULLUP);

  pinMode(16, OUTPUT); digitalWrite(16, LOW); delay(50); digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);
 // display.drawString(64, 22, "Press any button to start");

}

void loop() {

  while (digitalRead(RB) == LOW) {
    if (x < 100) {
      x = x + 1;
      display.clear();
      display.drawString(64, 12, String(x));
      display.drawProgressBar(14, 44, 100, 10, x);
      display.display();
      delay(30);
    }
  }
  while (digitalRead(LB) == LOW) {
    if (x > 0) {
      x = x - 1;
      display.clear();
      display.drawString(64, 12, String(x));
      display.drawProgressBar(14, 44, 100, 10, x);
      display.display();
      delay(30);
    }
  }
}
