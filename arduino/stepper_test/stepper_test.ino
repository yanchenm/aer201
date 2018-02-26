int dirPin = 9;
int stepPin = 9;

void setup() {
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  digitalWrite(dirPin, HIGH);

  for (int i = 0; i < 400; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);

    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }

  delay(1000);

  digitalWrite(dirPin, LOW);

  for (int i = 0; i < 400; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);

    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }

  delay(1000);
}
