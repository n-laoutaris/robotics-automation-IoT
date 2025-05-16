#include <Servo.h>

const int sensorLeft = A4;
const int sensorRight = A5;
const int LED = 11;
const int pwr = 12;

Servo leftServo;
Servo rightServo;

void setup() {
  leftServo.attach(9);
  rightServo.attach(10);
  pinMode(LED, OUTPUT);
  pinMode(pwr, INPUT);
  while(true){if (digitalRead(pwr) == HIGH) break;}
}

void loop() {

  forward();
  delay(125);
  int leftState = digitalRead(sensorLeft);
  int rightState = digitalRead(sensorRight);

  if (leftState == 0) {
    digitalWrite(LED, HIGH);
    reverse(); delay(250);
    reverseLeft(); delay(750);
    digitalWrite(LED, LOW);
  }

  if (rightState == 0) {
    digitalWrite(LED, HIGH);
    reverse(); delay(250);
    reverseRight(); delay(750);
    digitalWrite(LED, LOW);
  }

}

// ————————————————————————— Drive

void s_stop() {
  leftServo.write(90);
  rightServo.write(90);
}

void forward() {
  leftServo.write(180);
  rightServo.write(0);
}

void reverse() {
  leftServo.write(0);
  rightServo.write(180);
}

void forwardRight() {
  leftServo.write(180);
  rightServo.write(90);
}

void forwardLeft() {
  leftServo.write(90);
  rightServo.write(0);
}

void reverseRight() {
  leftServo.write(0);
  rightServo.write(90);
}

void reverseLeft() {
  leftServo.write(90);
  rightServo.write(180);
}
