#include <Servo.h>
#include <IRremote.h>

const int RECV_PIN = 7;
const int LED = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;
Servo leftServo;
Servo rightServo;
int v = 100;  // Velocity modifier percentage
//int brightness = 225;

void setup() {
  irrecv.enableIRIn();
  irrecv.blink13(true);
  leftServo.attach(9);
  rightServo.attach(10);
  pinMode(LED, OUTPUT);
}

void loop() {
  if (irrecv.decode(&results)) {

    switch (results.value) {
      case 2064: //Keypad button M^^
        forward(v);             break;
      case 528: //Keypad button M,,
        reverse(v);             break;
      case 16: //Keypad button ms^
        forwardRight(v);        break;
      case 3088: //Keypad button ms,
        reverseRight(v);        break;
      case 1040: //Keypad button md^
        forwardLeft(v);         break;
      case 2576: //Keypad button md,
        reverseLeft(v);         break;
//      case 144: //Keypad button 14+
//        if (v < 100) v = v + 10;      break;
//      case 2192: //Keypad button 17-
//        if (v > 0)   v = v - 10;      break;
      default:
        s_stop();              break;
    }
    irrecv.resume();
  }
//  brightness = map(v, 0, 100, 0, 255);
//  analogWrite(LED, brightness);
}
// ————————————————————————— Drive

void s_stop() {
  leftServo.write(90);
  rightServo.write(90);
}

void forward(int v) {
  leftServo.write(180);
  rightServo.write(0);
}

void reverse(int v) {
  leftServo.write(0);
  rightServo.write(180);
}

void forwardRight(int v) {
  leftServo.write(180);
  rightServo.write(90);
}

void forwardLeft(int v) {
  leftServo.write(90);
  rightServo.write(0);
}

void reverseRight(int v) {
  leftServo.write(0);
  rightServo.write(90);
}

void reverseLeft(int v) {
  leftServo.write(90);
  rightServo.write(180);
}
