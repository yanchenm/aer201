int i;
float red_calib;
float blue_calib;

float red_val;
float blue_val;

float red_avg;
float blue_avg;

void setup() {
  Serial.begin(9600);

  red_avg = 0.0;
  blue_avg = 0.0;

  red_calib = 0.0;
  blue_calib = 0.0;
  
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  analogWrite(9, 255);

  for (i = 0; i < 10; i++) {
    red_calib += analogRead(A0);
    delay(100);    
  }
  
  analogWrite(9, 0);
  analogWrite(11, 255);
  
  red_calib = red_calib / 10;

  for (i = 0; i < 10; i++) {
    blue_calib += analogRead(A0);
    delay(100);    
  }
  analogWrite(11, 0);
  
  blue_calib = blue_calib / 10;
}
 
void loop() {
  red_avg = 0.0;
  blue_avg = 0.0;

  analogWrite(9, 255);
  
  for (i = 0; i < 10; i++) {
    red_avg += analogRead(A0);
    delay(100);
  }

  Serial.print("RED: ");
  Serial.println((red_avg / 10) / red_calib);

  analogWrite(9, 0);
  analogWrite(11, 255);

  for (i = 0; i < 10; i++) {
    blue_avg += analogRead(A0);
    delay(100);
  }

  analogWrite(11, 0);

  Serial.print("BLUE: ");
  Serial.println((blue_avg / 10) / blue_calib);
}
