int servoPin = 3;
int pulse = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(servoPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(servoPin, HIGH);
  delay(1);
  digitalWrite(servoPin, LOW);
  delay(19);
}
