#include "SSD1306Wire.h"
#include "DHT.h"
SSD1306Wire display(0x3c, 4, 15);

const int DHTPin = 17;
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.
DHT dht(DHTPin, DHT11);

void setup() {
  pinMode(16, OUTPUT); digitalWrite(16, LOW); delay(50); digitalWrite(16, HIGH);
  Serial.begin(115200);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);

  dht.begin();
  display.drawString(64, 5, "DHT Initialised.");
}

void loop() {
  delay(1000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    display.drawString(64, 5, "Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  String T = String(t); T.remove(4);
  String H = String((int) h);
  String HIC = String(hic);

  display.clear();
  display.drawString(64, 0, "Temp: " + T);
  display.drawString(64, 22, "Hum: " + H + "%");
  display.drawString(64, 42, "HI: " + HIC);
  display.display();
}
