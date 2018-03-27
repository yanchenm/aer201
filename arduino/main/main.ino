#include <Wire.h>
#include <Servo.h>

int dispenserSensor[3] = {A1, A2, A3};
int sensor, minVal, Val[3] , colArray[3] = {4, 5, 6},  total;

float Percent[3];

int i, j , readRGB[3] , readMax[3], Domin;

int sensorMax[3] = {0, 0, 0};

long calibtime, prevtime;

int gatePosition = 0;
int orientation = -1;

Servo dispenserServo[3]; // 0 -> round, 1 -> flat, 2 -> long
Servo gateServo;

int box_position[3] = {20, 25, 0};
int mid_position[3] = {90, 90, 75};
int dump_position[3] = {150, 180, 140};

void setup() {

  Serial.begin(9600);
  Serial.println("Setup");
  
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  dispenserServo[0].attach(9);
  dispenserServo[1].attach(10);
  dispenserServo[2].attach(11);

  gateServo.attach(3);
  gateServo.write(60);
  delay(500);
  
  dispenserServo[0].write(mid_position[0]);
  delay(500);
 
  dispenserServo[1].write(mid_position[1]);
  delay(500);
  
  dispenserServo[2].write(mid_position[2]);
  delay(500);

  //  sensorCalibrate();
  //  rgbCalibrate();
}

void loop() {

}

void receiveEvent(void) {
  uint8_t x = Wire.read();
  Serial.println(x);

  int action = (x & B11000000) >> 6;
  int servoNum = (x & B00110000) >> 4;
  int number = (x & B00001100) >> 2;

  switch (action) {
    case 0:
      // rgb();
      break;
    case 1:
      dispense(servoNum);
      Serial.println("dispense\n");
      break;
    case 2:
      flipGate();
      break;
    case 3:
      ret(servoNum);
      Serial.println("return\n");
      break;
  }
}

void requestEvent(void) {
  Wire.write(orientation);
}

void dispense (int dispenser) {
    dispenserServo[dispenser].write(box_position[dispenser]);
    Serial.println(box_position[dispenser]);
}

void ret (int dispenser) {
  dispenserServo[dispenser].write(mid_position[dispenser]);
  Serial.println(mid_position[dispenser]);
}

void rgb() {
  int r = 0;
  int b = 0;

  for (j = 0; j < 3; j++) {
    total = 0 ;

    for ( i = 0 ; i < 3 ; i++)                        //  CHECK VALUES IN A LOOP
    {
      prevtime = millis();
      while (millis() - prevtime < 1000)                   //  AVOID DELAY
      {
        analogWrite(colArray[i], Val[i]);                     //  WRITE THE CALIBRATED VALUES
        readRGB[i] = 1024 - analogRead(0);
        delay(50);
      }

      digitalWrite(colArray[i], 0);

      prevtime = millis();                                         // RESET TIME

      total = total + readRGB[i];

    }


    for (i = 0 ; i < 3 ; i ++)
    {
      Percent[i] = readRGB[i] * 100.0 / total;                //  STORE IN THE FORM OF PERCENTAGE
    }

    if (Percent[0] > (Percent[2] + 10)) {
      r++;
    }
    else if (Percent[2] > Percent[0] + 10) {
      b++;
    }
    else {
      // j--;
    }

    delay(300);
  }

  if (r > b) {
    orientation = 1;
  }
  else {
    orientation = 0;
  }
}

void rgbCalibrate()                                        //   CALIBRATE FUNCTION
{
  for (i = 0; i < 3; i++)
  {
    while (millis() - calibtime < 1000)                 //    FLASH EACH COLOR AT MAX FOR 1 SEC
    {
      analogWrite(colArray[i], 255);
      readMax[i] = 1024 - analogRead(0);                     //    RECORD MAX VALUES
    }

    analogWrite(colArray[i], 0);
    Serial.println(readMax[i]);
    delay(10);

    calibtime = millis();

  }

  if (readMax[0] < readMax[1] && readMax[0] < readMax[2])     //   GET THE MINIMUM VALUE FROM ARRAY

    minVal = readMax[0];

  else
  {

    if ( readMax[1] < readMax[0] && readMax[1] < readMax[2])

      minVal = readMax[1];


    else

      minVal = readMax[2];

  }


  for (i = 0 ; i < 3 ; i++)
  {

    analogWrite(colArray[i], 10);
    sensor = 1024 - analogRead(0);                                    // START CALIBRATION
    delay(100);

    while ( sensor - minVal <= -1 || sensor - minVal >= 1 )  //    GET THE DIFFERENCE BETWEEN CURRENT VALUE AND THRESHOLD
    {
      sensor = 1024 - analogRead(0);

      if ( sensor > minVal )                                      //   INCREASE OR DECREASE THE VALUE TO EQUALIZE THE BRIGHTNESS
        Val[i]--;

      else
        Val[i]++;

      delay(50);

      Val[i] = constrain(Val[i], 0, 255);               //   CONSTRAIN THE VALUE B/W  0  -- 255
      analogWrite(colArray[i], Val[i]);
    }

    analogWrite(colArray[i], 0);
    delay(50);

  }

}

void sensorCalibrate() {
  for (i = 0; i < 3; i++) {
    sensorMax[i] = analogRead(dispenserSensor[i]);
  }
}

void flipGate() {
  if (gatePosition == 0) {
    gateServo.write(120);
    gatePosition = 1;
    Serial.println("Gate 0");

    delay(1000);
  }
  else {
    gateServo.write(60);
    gatePosition = 0;
    Serial.println("Gate 1");

    delay(1000);
  }
}

