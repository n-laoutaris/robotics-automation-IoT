#include <Servo.h>
#include <IRremote.h>

Servo leftServo;
Servo rightServo;
const int RECV_PIN = 11;
const int M1A = 2;
const int M1B = 3;
const int M2A = 4;
const int M2B = 5;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  irrecv.enableIRIn();
  irrecv.blink13(true);
  leftServo.attach(9);
  rightServo.attach(10);
  pinMode(M1A, OUTPUT);   pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);   pinMode(M2B, OUTPUT);
}

void loop() {
  if (irrecv.decode(&results)) {

    switch (results.value) {
      case 2064: //Keypad button M^^
        forward();             break;
      case 528: //Keypad button M,,
        reverse();             break;
      case 16: //Keypad button ms^
        forwardRight();        break;
      case 3088: //Keypad button ms,
        reverseRight();        break;
      case 1040: //Keypad button md^
        forwardLeft();         break;
      case 2576: //Keypad button md,
        reverseLeft();         break;
      case 1552: //Keypad button P<>
        pinchOpen();           break;
      case 272: //Keypad button P><
        pinchClose();          break;
      case 3600: //Keypad button P^
        pinchUp();             break;
      case 2320: //Keypad button P,
        pinchDown();           break;
      default:
        s_stop(); m_stop();    break;
    }
    irrecv.resume();
  }
}
// ———————————— Drive ————————————
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
// ———————————— Pinch ————————————
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
  delay(25);
}
void pinchUp() {
  digitalWrite(M2A, LOW);  digitalWrite(M2B, HIGH);
  delay(25);
}
void pinchDown() {
  digitalWrite(M2A, HIGH);  digitalWrite(M2B, LOW);
  delay(25);
}
