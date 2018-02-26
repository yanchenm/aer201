int sensorPin = A1;
int sensorValue = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(sensorPin);

  if (sensorValue < 50) {
    Serial.print(sensorValue);
  }
  
  Serial.print('\n');
  delay(10);
}
