#include <Servo.h>

Servo servo;

int servoPin = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(servoPin);
  pinMode(9, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//
//  servo.write(0);
//  delay(1000);
//  servo.write(45);
//  delay(1000);
//  servo.write(90);
//  delay(1000);
//  servo.write(135);
//  delay(1000);
//  servo.write(180);
//  delay(1000);

  digitalWrite(9, HIGH);
}
