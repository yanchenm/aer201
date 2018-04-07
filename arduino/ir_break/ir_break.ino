#include <Servo.h>

int sensorPin = A0;
int sensorValue = 0;

Servo servo;

void setup() {
  Serial.begin(9600);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(A0, INPUT);

  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  
  if (sensorValue < 50) {
    Serial.print("Detected");
    Serial.print('\n');
    delay(500);
  }
  
  delay(1);
}  
