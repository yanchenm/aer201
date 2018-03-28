#include <Servo.h>

Servo gateServo;
Servo dispenseServo;

int servoPin = 3;
int gatePosition = 0;

int dispensePin = 5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gateServo.attach(servoPin);
  gateServo.write(90);

  dispenseServo.attach(dispensePin);
  dispenseServo.write(70);
}

void loop() {
  //dispense();
  //flipGate();
  delay(3000);
}

void flipGate() {
  if (gatePosition == 0) {
    gateServo.write(120);
    delay(500);
    gatePosition = 1;
    Serial.println("Gate 0");
  }
  else {
    gateServo.write(60);
    delay(500);
    gatePosition = 0;
    Serial.println("Gate 1");
  }
}

void dispense() {
  dispenseServo.write(0);
  delay(1000);
  dispenseServo.write(70);
  delay(500);
}

