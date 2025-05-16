/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

//#include <Wire.h>
#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, 4, 15, GEOMETRY_128_64);


void setup() {
  //Wire.begin(4, 15);
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");

  pinMode(16, OUTPUT); digitalWrite(16, LOW); delay(50); digitalWrite(16, HIGH);
  display.init();  // απο οτι φαινεται εχει μεσα του το wire.begin(4, 15)
}

void loop() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for (address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      nDevices++;
    }
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(2000);
}
