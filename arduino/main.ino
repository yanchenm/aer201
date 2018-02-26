#include <Wire.h>
#include <Servo.h>

uint8_t state = 0;
uint8_t incomingByte;

int dispenser[3] = {9, 10, 11};
int dispenserSensor[3] = {A0, A1, A2};
int gate = 3;

void setup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}
