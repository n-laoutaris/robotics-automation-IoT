/*
  The circuit:
  Warning LED on pin 2
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

#include <SPI.h>
#include <SD.h>
#include "DHT.h"
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const int SD_CS = 5;
int i = 0;
const int LED = 2;
const int DHTPin = 17;
DHT dht(DHTPin, DHT11);
String myfile = "/Comparison_log.txt";

const char* ssid = "Nicklamda";
const char* password = "andronikos";

void setup() {

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH); // Setup starting

  Wire.begin(4, 15);

  // Open serial communications
  Serial.begin(115200);

  // Connect to the Network via WiFi and initialize Wifi Serial
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to WiFi ");   Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("..connected! IP Address: ");  Serial.println(WiFi.localIP());
  // Connect to the NTP client over the internet
  Serial.print("Connecting to NTP client...");
  timeClient.begin();
  timeClient.setTimeOffset(10800);
  Serial.println("connected.");

  // Initialize DHT
  Serial.print("Initializing DHT sensor...");
  dht.begin();
  Serial.println("DHT initialized.");

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  // Initialize the card
  Serial.print("Initializing SD card...");
  SD.begin(SD_CS);
  if (!SD.begin(SD_CS)) {
    Serial.println("initialization failed.");
  }
  else {
    Serial.println("card initialized.");
  }

  digitalWrite(LED, LOW); // Setup complete

}

void loop() {
  delay(5000);
  digitalWrite(LED, HIGH); // data logging starting
  i++;
  // Get the Date and time. The date will be the file name
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }
  String formattedDate = timeClient.getFormattedDate();
  String timeStamp = extractTime(formattedDate);      // We need to extract date and time
  String dataString = "ID: " + format3Digits(i) + " at " + timeStamp + " || ";

  File dataFile = SD.open(myfile, FILE_APPEND);
  float t1 = dht.readTemperature();
  dataString += "DHT-11 measurement: " + String(t1);
  float t2 = bmp.readTemperature();
  dataString += " | BMP180 measurement: " + String(t2);

  if (dataFile) {                  // if the file is available, write to it:
    dataFile.println(dataString);
    dataFile.close();
    Serial.println("Wrote in " + myfile + " -> " + dataString); // print to the serial port
    digitalWrite(LED, LOW); // data logging complete
  }
  else {                           // if the file isn't open, pop up an error:
    Serial.println("Error opening " + myfile);
  }
}

String extractTime(String formattedDate) {
  int splitT = formattedDate.indexOf("T");
  return formattedDate.substring(splitT + 1, formattedDate.length() - 1);
}

String format3Digits(int i) {
  if (i < 10) {
    return "00" + String(i);
  }
  else if (i < 100) {
    return "0" + String(i);
  }
  else {
    return String(i);
  }
}
