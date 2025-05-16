/*
  Test_DeA_DC_Motor_Board.ino
  Il programma ha il compito di verificare il funzionamento
  della scheda DeA DC Motor Board - Rev.D
  Su questa scheda sono presenti due integrati L293DNE
  ognuno comanda un motore.
  Il pilotaggio avviene tramite 4 pin presenti sul connettore X1.
  Pin 17-18 - Motor 1 - X2
  Pin 6-8   - Motor 2 - X3
  Pin 20    - Vin
  Pin, 1-2   - GND

  Su Arduino sono utilizzati i seguenti pin
  Pin GND         -> Pin 1-2
  Pin Vin         -> Pin 20
  Pin Digital 2   -> Pin 17
  Pin Digital 3   -> Pin 18
  Pin Digital 4   -> Pin 6
  Pin Digital 5   -> Pin 8

  Ultima modifica il 4/4/2015
  Applicazione realizzata da Adriano Gandolfo
  Sito http://www.adrirobot.it
  Blog http://it.emcelettronica.com/author/adrirobot
  This example code is in the public domain.
*/
#include <IRremote.h>

// Pin Definition
const int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;
const int M1A = 2;
const int M1B = 3;
const int M2A = 4;
const int M2B = 5;

void setup() {
  irrecv.enableIRIn();
  irrecv.blink13(true);
  pinMode(M1A, OUTPUT);   pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);   pinMode(M2B, OUTPUT);
}

void loop() {
  if (irrecv.decode(&results)) {

    switch (results.value) {
      case 1552: //Keypad button P<>
        pinchOpen();        break;
      case 272: //Keypad button P><
        pinchClose();        break;
      case 3600: //Keypad button P^
        pinchUp();        break;
      case 2320: //Keypad button P,
        pinchDown();        break;
      default:
        m_stop();           break;
    }

    irrecv.resume();
  }

}
// ————————————————————————— Drive

void m_stop() {
  digitalWrite(M1A, LOW);  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);  digitalWrite(M2B, LOW);
  delay(25);
}

void pinchOpen() {
  digitalWrite(M1A, LOW);  digitalWrite(M1B, HIGH);
  delay(25);
}

void pinchClose() {
  digitalWrite(M1A, HIGH);  digitalWrite(M1B, LOW);
  delay (25);
}

void pinchUp() {
  digitalWrite(M2A, LOW);  digitalWrite(M2B, HIGH);
  delay(25);
}

void pinchDown() {
  digitalWrite(M2A, HIGH);  digitalWrite(M2B, LOW);
  delay(25);
}
