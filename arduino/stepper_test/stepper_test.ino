#include <Stepper.h>

int steps = 100;

Stepper stepper(steps, 8, 9, 10, 11);

int previous = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  stepper.setSpeed(90);
}

void loop() {
  // put your main code here, to run repeatedly:
  stepper.step(10);
}
