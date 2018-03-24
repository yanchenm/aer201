#include <Servo.h>

int dirPin = 9;
int stepPin = 10;

int gatePosition = 0;

Servo gateServo;

void setup() {
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);

  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, LOW);

  gateServo.attach(3);

  gateServo.write(60);
  Serial.begin(9600);
}


void loop() {
  digitalWrite(dirPin, HIGH);
  Serial.println("Forward");

  int distance = 0;
  float rev = 0.0;
  int steps = 0;

  distance = 110;
  rev = distance * 0.125;
  steps = rev * 200;

  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);

    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }

  delay(5000);

  distance = 27;
  rev = distance * 0.125;
  steps = rev * 200;

  for (int j = 0; j < 7; j++) {
    for (int i = 0; i < steps; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(500);
  
      digitalWrite(stepPin, LOW);
      delayMicroseconds(500);
    }

    delay(5000);
    //flipGate();
  }

  distance = 59;
  rev = distance * 0.125;
  steps = rev * 200;

  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);

    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }

  delay(5000);

  digitalWrite(dirPin, LOW);

  distance = 360;
  rev = distance * 0.125;
  steps = rev * 200;

  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);

    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }

  delay(10000);
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
