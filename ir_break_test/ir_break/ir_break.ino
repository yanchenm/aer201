int sensorPin = A1;
int sensorValue = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(sensorPin);
  Serial.print(sensorValue);
  Serial.print('\n');
  delay(100);
}
