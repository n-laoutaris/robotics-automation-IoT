/*
  The circuit:
  Warning LED on pin 2
  LDR on pin 36 with ~ 1000Ω pulldown resistor
  DHT-11 on pin 17
  SD card attached to SPI bus as follows:
  ** MOSI - pin 23
  ** MISO - pin 19
  ** CLK - pin 18
  ** CS - pin 5 (for MKRZero SD: SDCARD_SS_PIN)
  BMP180 attached to I2C as follows:
  ** SDA - pin 4
  ** SCL - pin 15
*/

#include "DHT.h"
#include "SSD1306Wire.h"
#include <Adafruit_BMP085.h>

int disp = 0;
unsigned long previousMillis = 0;

const int PRG = 0;
const int LDR = 36;
const int LED = 2;
const int DHTPin = 17;

SSD1306Wire display(0x3c, 4, 15, GEOMETRY_128_64);
DHT dht(DHTPin, DHT11);
Adafruit_BMP085 bmp;

float temp;
float hum;
float hic;
float pres;
int lum;

void setup() {

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH); // Setup starting

  // Open serial communications
  Serial.begin(115200);

  // Setup OLED
  pinMode(16, OUTPUT); digitalWrite(16, LOW); delay(50); digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);

  // Initialize DHT
  Serial.print("Initializing DHT sensor...");
  dht.begin();
  Serial.println("DHT initialized.");

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }

  digitalWrite(LED, LOW); // Setup complete
}

void loop() {
  while (digitalRead(PRG) == LOW) {
    disp++;
    displayCurrentData();
    delay(250);
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 5000) {
    previousMillis = currentMillis;
    digitalWrite(LED, HIGH); // data gathering starting

    temp = (dht.readTemperature() + bmp.readTemperature()) / 2;
    hum = dht.readHumidity();
    hic = dht.computeHeatIndex(temp, hum, false);
    pres = bmp.readPressure() / 100;
    int l = analogRead(LDR);
    lum = map(l, 0, 4095, 0, 100);

    digitalWrite(LED, LOW); // data reporting complete
    displayCurrentData();
  }
}


void displayCurrentData() {
  display.clear();
  switch (disp % 5) {
    case 0:
      display.drawString(64, 5, "Temperature");
      display.drawString(64, 30, String(temp) + "*C");
      break;
    case 1:
      display.drawString(64, 5, "Humidity");
      display.drawString(64, 30, String(hum) + "%");
      break;
    case 2:
      display.drawString(64, 5, "Feels like");
      display.drawString(64, 30, String(hic) + "*C");
      break;
    case 3:
      display.drawString(64, 5, "Pressure");
      display.drawString(64, 30, String(pres) + " hPa");
      break;
    case 4:
      display.drawString(64, 5, "Luminosity");
      display.drawString(64, 30, String(lum) + "%");
      break;
  }
  display.display();
}
