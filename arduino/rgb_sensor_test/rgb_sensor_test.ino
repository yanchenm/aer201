int sensor, minVal, Val[3] , colArray[3] = {4, 5, 6},  total;

float Percent[3];

int i , readRGB[3] , readMax[3], Domin;                 //  DECLARE VARIABLES

long calibtime, prevtime;                               //  RECORD THE TIME ELAPSED

void setup()
{
  Serial.begin(9600);
  for (i = 0 ; i < 3 ; i++)
  {
    pinMode(colArray[i], OUTPUT);                     // SET THE OUTPUT PINS
  }

  calibrate();                                     //  RUN THE CALIBRATE FUNCTION
}

void loop()
{

  total = 0 ;

  for ( i = 0  ; i < 3 ; i++)                        //  CHECK VALUES IN A LOOP
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
    Percent[i] = readRGB[i] * 100.0 / total;                 //  PRINT IN THE FORM OF PERCENTAGE
    Serial.print(Percent[i]);
    Serial.print(" %   ");
  }

  Serial.println("");
  delay(1000);

}

void calibrate()                                        //   CALIBRATE FUNCTION
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

      Serial.print(1024 - analogRead(0));
      Serial.print("      ");
      Serial.println(minVal);

      delay(50);

      Val[i] = constrain(Val[i], 0, 255);               //   CONSTRAIN THE VALUE B/W  0  -- 255
      analogWrite(colArray[i], Val[i]);
    }

    analogWrite(colArray[i], 0);
    delay(50);

  }

}

