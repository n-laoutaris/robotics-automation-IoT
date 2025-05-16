/*

  The circuit:
   analog sensors on analog ins 0, 1, and 2
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

*/

#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;
const int ThermistorPin = A0;
const float R1 = 10000; //Value of the divider resistor for the Thermistor circuit
String myfile = "datalog.txt";

int i = 0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) { // see if the card is present and can be initialized
    Serial.println("Card failed, or not present");
    while (1); // don't do anything more
  }
  Serial.println("card initialized.");
  // Some sound or visual effect here?
}

void loop() {
  // make a string for assembling the data to log:
  i++;
  String dataString = "Measurement " + String(i) + ": ";

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(myfile, FILE_WRITE);

  String temp = String(getThermistorTemp());
  dataString += "Temp = " + temp + " C. ";
  dataString += "Time elapsed = " + expressTime((millis());

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);     // print to the serial port too
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening " + myfile);
  }
  delay(5000);
}

float getThermistorTemp() {
  int Vo;
  float logR2, R2, T;
  float c1 = 0.9104334644e-03, c2 = 2.147972021e-04, c3 = 1.066844317e-07; //Supposedly calculated for our 47k thermistor

  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2)); //kelvin
  T = T - 273.15;
  return T;

}

String expressTime(unsigned long rem) {
  int hours = rem / 3600000;
  rem = rem - 3600000 * hours;
  int mins = rem / 60000;
  rem = rem - 60000 * mins;
  int secs = rem / 1000;
  // rem = rem-1000*secs //to get the remaining milliseconds.
  return String(hours) + ":" + String(mins) + ":" + String(secs);
}
