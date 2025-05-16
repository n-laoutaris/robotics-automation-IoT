#include <Servo.h>

Servo radar;
const int angle=60;
int left = 94 + (angle/2);
int right = 94 - (angle/2);
int pos = left;

Servo leftServo;
Servo rightServo;

const int trigPin = A4;
const int echoPin = A5;

const int buzzer = A0;
const int leftled = 6;
const int rightled = 7;
const int pwr = 12;


void setup() {

  Serial.begin(9600);

  radar.attach(11);  
  leftServo.attach(10);
  rightServo.attach(9);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(leftled, OUTPUT);
  pinMode(rightled, OUTPUT);
  pinMode(pwr, INPUT_PULLUP);

  radar.write(94);
  while (true) {
    if (digitalRead(pwr) == LOW) break;
  }
}

void loop() {

  if (pos == right) {
    pos = left;
  }
  else if (pos == left) {
    pos = right;
  }
  radar.write(pos);
  delay(80);

  if (pos == left) {
    if (getDistance() <= 30) {
      digitalWrite(leftled, HIGH);
      pivotRight();
    }
    else {
      digitalWrite(leftled, LOW);
      forward();
    }
  }

  if (pos == right) {
    if (getDistance() <= 30) {
      digitalWrite(rightled, HIGH);
      pivotLeft();
    }
    else {
      digitalWrite(rightled, LOW);
      forward();
    }
  }
  delay(420);

}

int getDistance() {
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH, 24000);
  if (duration == 0) {
    Serial.println("Distance out of range.");
    return 999;
  }
  // Calculating the distance
  int distance = (duration / 2) / 29.1; //centimeters
  Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm.");
  return distance;
}

// ———————————  Drive ——————————————

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
void pivotLeft() {
  leftServo.write(0);
  rightServo.write(0);
}
void pivotRight() {
  leftServo.write(180);
  rightServo.write(180);
}
