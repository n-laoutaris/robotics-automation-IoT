//#include <Servo.h>
const int sensorLeft = A0;   //Using with ~10kΩ pulldown
const int sensorRight = A1;  //Using with ~10kΩ pulldown
//const int pwr = 12;
const int leftled = 6;
const int rightled = 7;
//Servo leftServo;
//Servo rightServo;

void setup() {
  Serial.begin(9600);
  //leftServo.attach(9);
  //rightServo.attach(10);
  pinMode(leftled, OUTPUT);
  pinMode(rightled, OUTPUT);
  //pinMode(pwr, INPUT_PULLUP);
  //  while (true) {
  //    if (digitalRead(pwr) == LOW) break;
  //  }
}

void loop() {
  int valueLeft = analogRead(sensorLeft);
  int valueRight = analogRead(sensorRight);

  Serial.print("Left brightness: "); Serial.print(valueLeft); Serial.print(". ");
  Serial.print("Right brightness: "); Serial.print(valueRight); Serial.println(". ");

  if (valueLeft > valueRight) {
    digitalWrite(leftled, HIGH);
    digitalWrite(rightled, LOW);
  }
  else {
    digitalWrite(leftled, LOW);
    digitalWrite(rightled, HIGH);
  }

  delay(25);
}

//// ————————————————————————— Drive
//
//void forward() {
//  leftServo.write(180);
//  rightServo.write(0);
//}
//
//void forwardRight() {
//  leftServo.write(180);
//  rightServo.write(90);
//}
//
//void forwardLeft() {
//  leftServo.write(90);
//  rightServo.write(0);
//}
