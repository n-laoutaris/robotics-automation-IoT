#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, 4, 15);

const int LED = 32;

void setup() {
  pinMode(16, OUTPUT); digitalWrite(16, LOW); delay(50); digitalWrite(16, HIGH);
  Serial.begin(115200);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);

  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  display.drawString(64, 12, "Blink!");   display.display();
  Serial.println("Blink!");
  delay(1000);                       // wait for a second

  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  display.clear(); display.display();
  delay(1000);                       // wait for a second
}
