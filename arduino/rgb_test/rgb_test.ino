int value;

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  analogWrite(9, 255);
}

void loop() {
  Serial.println(analogRead(9));
  delay(500);
}
