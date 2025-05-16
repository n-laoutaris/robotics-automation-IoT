#include <Servo.h>

Servo myservo;

int pos = 0;    // variable to store the servo position
int k=1;        // direction of servo rotation

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {

  pos = pos + k;

  if (pos==180) {
    k=-1;
  }
  else if (pos==0) {
    k=1;
  }

  myservo.write(pos);
  delay(20);

}
