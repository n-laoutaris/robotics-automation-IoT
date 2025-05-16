#include <IRremote.h>

const int relayPin = 8;
const int RECV_PIN = 11;
boolean relayIsOn = false;
const int redled = 6;
const int greenled = 7;

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  irrecv.enableIRIn();
  irrecv.blink13(true);

  pinMode (redled, OUTPUT); pinMode (greenled, OUTPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); digitalWrite(redled, HIGH); digitalWrite(greenled, LOW);
}

void loop() {
  if (irrecv.decode(&results)) {

    switch (results.value) {
      case 2704:
        relayOnOff(); break;
    }
    irrecv.resume();
  }
}

void relayOnOff() {
  if (relayIsOn) {
    digitalWrite(relayPin, HIGH);
    relayIsOn = false;
    digitalWrite(redled, HIGH); digitalWrite(greenled, LOW);
  }
  else {
    digitalWrite(relayPin, LOW);
    relayIsOn = true;
    digitalWrite(redled, LOW); digitalWrite(greenled, HIGH);
  }
  delay(250);
}
