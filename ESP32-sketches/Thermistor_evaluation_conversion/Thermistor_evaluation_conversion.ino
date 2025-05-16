#define SERIESRESISTOR 47000
// Number of samples to average
#define SAMPLERATE 5
// Nominal resistance at 25C
#define THERMISTORNOMINAL 47000
// Nominal temperature in degrees
#define TEMPERATURENOMINAL 25
// Beta coefficient
#define BCOEFFICIENT 3380
const int tpin = 36;
void setup() {Serial.begin(9600);}
int getTemp() {
  double thermalSamples[SAMPLERATE];
  double average, kelvin, resistance, celsius;
  int i;
  // Collect SAMPLERATE (default 5) samples
  for (i = 0; i < SAMPLERATE; i++) { 
thermalSamples[i] = analogRead(tpin);
delay(10);
  }
  // Calculate the average value of the samples
  average = 0;
  for (i = 0; i < SAMPLERATE; i++) {
    average += thermalSamples[i];
  }
  average /= SAMPLERATE;
  // Convert to resistance
  resistance = 4095 / average - 1;
  resistance = SERIESRESISTOR / resistance;
  /*
     Use Steinhart equation (simplified B parameter equation) to convert resistance to kelvin
     B param eq: T = 1/( 1/To + 1/B * ln(R/Ro) )
     T  = Temperature in Kelvin
     R  = Resistance measured
     Ro = Resistance at nominal temperature
     B  = Coefficent of the thermistor
     To = Nominal temperature in kelvin
  */
  kelvin = resistance / THERMISTORNOMINAL;                            // R/Ro
  kelvin = log(kelvin);                                                                   // ln(R/Ro)
  kelvin = (1.0 / BCOEFFICIENT) * kelvin;                                 // 1/B * ln(R/Ro)
  kelvin = (1.0 / (TEMPERATURENOMINAL + 273.15)) + kelvin; // 1/To + 1/B * ln(R/Ro)
  kelvin = 1.0 / kelvin;                                                                  // 1/( 1/To + 1/B * ln(R/Ro) )
  // Convert Kelvin to Celsius
  celsius = kelvin - 273.15;
  // Send the value back to be displayed
  return celsius;
}

void loop() {
  int temp;

  // Call the function to get the temperature in degrees celsius
  temp = getTemp();

  // Output the temp to serial
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println(" C");
}
