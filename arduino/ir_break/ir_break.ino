#include <Servo.h>

int sensorPin = A1;
int sensorValue = 0;

Servo servo;

void setup() {
  Serial.begin(9600);

  servo.attach(3);
  servo.write(150);
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  
  //if (sensorValue < 50) {
    Serial.print(sensorValue);
    Serial.print('\n');
  //}
  
  delay(10);
}  
