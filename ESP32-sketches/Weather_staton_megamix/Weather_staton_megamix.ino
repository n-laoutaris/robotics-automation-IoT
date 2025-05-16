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
#include <SPI.h>
#include <SD.h>
#include <WiFi.h>
#include "time.h"
//#include <Arduino.h>
//#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

//WiFi settings
AsyncWebServer server(80);
const char* ssid = "Mia_Skatoula";
const char* password = "6972129095Chatzi!";

//Global constants
const int PRG = 0;
const int LDR = 36;
const int LED = 2;
const int DHTPin = 17;
String log_file = "/Weather_Station_Log.csv";
const int data_collection_interval = 6000;  // Data is collected every how many miliseconds?

// OLED Setting
SSD1306Wire display(0x3c, 4, 15, GEOMETRY_128_64);

//DHT-11 setting
DHT dht(DHTPin, DHT11);

// BMP180 setting
Adafruit_BMP085 bmp;

//Global variables
int disp = 0;
unsigned long previousMillis = 0;
float tempDHT;
float tempBMP;
float humidity;
float feelsLike;
float pressure;
int luminosity;
String dateStamp;
String timeStamp;
String lastLog = "Never";
File dataFile;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);  // Setup starting indicator

  // Open serial communications
  Serial.begin(115200);

  // Setup OLED
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  delay(50);
  digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);

  // Connect to the Network via WiFi and initialize Wifi Serial
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to WiFi ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("..connected! IP Address: ");
  Serial.println(WiFi.localIP());

  // Setup WebSerial
  WebSerial.begin(&server);
  // WebSerial.msgCallback(recvMsg);  // Not implemented
  server.begin();
  Serial.print("WebSerial is accessible at ");
  Serial.print(WiFi.localIP());
  Serial.println("/webserial in browser.");

  // Setup the Time Client
  Serial.print("Connecting to NTP client...");
  configTime(3600 * 3, 0, "pool.ntp.org");  // GMT+3 in seconds, No daylight savings, NTP server address
  Serial.println("connected.");

  // Initialize DHT-11
  Serial.print("Initializing DHT-11 sensor...");
  dht.begin();
  Serial.println("DHT-11 initialized.");

  // Initialize BMP180
  Serial.print("Initializing BMP180 sensor...");
  if (!bmp.begin()) {
    Serial.println("Could not find a  BMP sensor.");
  } else {
    Serial.println("BMP180 initialized.");
  }

  // Initialize the SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin()) {
    Serial.println("initialization failed.");
    delay(2000);
  } else {
    Serial.println("card initialized.");
  }

  digitalWrite(LED, LOW);  // Setup complete indicator
  Serial.println("Setup Complete!");
  WebSerial.println("Setup Complete!");
}

void loop() {

  // Check if button is pressed to change the display
  while (digitalRead(PRG) == LOW) {
    disp++;
    displayCurrentData();
    delay(250);
  }

  // Check if it's time to collect data again
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= data_collection_interval) {
    previousMillis = currentMillis;

    // ==================== Data collection Phase ====================

    digitalWrite(LED, HIGH);  // Data collection and logging starting indicator

    // Get the Date and time
    getDateAndTimeStrings(dateStamp, timeStamp);

    // Get sensor readings
    tempDHT = dht.readTemperature();
    tempBMP = bmp.readTemperature();
    humidity = dht.readHumidity();
    feelsLike = dht.computeHeatIndex(tempDHT, humidity, false);
    pressure = bmp.readPressure() / 100;
    luminosity = map(analogRead(LDR), 0, 4095, 0, 100);

    // ==================== Data Reporting Phase ====================

    // make a string for assembling the data to log:
    String dataString = dateStamp + "," + timeStamp + "," + String(tempDHT) + "," + String(tempBMP) + "," + String(humidity) + "," + String(feelsLike) + "," + String(pressure) + "," + String(luminosity);

    // open the file. only one file can be open at a time, so close this one before opening another.
    dataFile = SD.open(log_file, FILE_APPEND);
    if (dataFile) {  // if the file is available, write to it:
      dataFile.println(dataString);
      dataFile.close();
      lastLog = timeStamp;
      Serial.println("Wrote in " + log_file + " -> " + dataString);  // print to the serial port
      WebSerial.println("Wrote in " + log_file + " -> " + dataString);
    } else {  // if the file isn't open, pop up an error:
      Serial.println("Failed to write in " + log_file + " -> " + dataString);  // print to the serial port
      WebSerial.println("Failed to write in " + log_file + " -> " + dataString);
    }
    digitalWrite(LED, LOW);  // Data collection and logging complete indicator
    displayCurrentData();
  }
  delay(25);  // Never hurt anyone
}

//--------------- FUNCTIONS --------------------------------------

void getDateAndTimeStrings(String& dateStr, String& timeStr) {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    dateStr = "Failed to obtain date.";
    timeStr = "Failed to obtain time.";
  } else {
    // Format date (dd/mm/yyyy)
    char dateBuf[11];  // "dd/mm/yyyy\0"
    snprintf(dateBuf, sizeof(dateBuf), "%02d/%02d/%04d", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
    dateStr = String(dateBuf);

    // Format time (hh:mm:ss)
    char timeBuf[9];  // "hh:mm:ss\0"
    snprintf(timeBuf, sizeof(timeBuf), "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    timeStr = String(timeBuf);
  }
}

void displayCurrentData() {
  display.clear();
  switch (disp % 6) {
    case 0:
      display.drawString(64, 5, "Temperature");
      display.drawString(64, 30, String((tempDHT + tempBMP) / 2) + "*C");
      break;
    case 1:
      display.drawString(64, 5, "Humidity");
      display.drawString(64, 30, String(humidity) + "%");
      break;
    case 2:
      display.drawString(64, 5, "Feels like");
      display.drawString(64, 30, String(feelsLike) + "*C");
      break;
    case 3:
      display.drawString(64, 5, "Pressure");
      display.drawString(64, 30, String(pressure) + " hPa");
      break;
    case 4:
      display.drawString(64, 5, "Luminosity");
      display.drawString(64, 30, String(luminosity) + "%");
      break;
    case 5:
      display.drawString(64, 5, "Last log");
      display.drawString(64, 30, lastLog);
      break;
  }
  display.display();
}

// For added functionality: Sending messages to the station. Not yet implemented.
void recvMsg(uint8_t* data, size_t len) {
  WebSerial.println("Received Data...");
  String d = "";
  for (int c = 0; c < len; c++) {
    d += char(data[c]);
  }
  WebSerial.println(d);
  // Do something else here?
}
