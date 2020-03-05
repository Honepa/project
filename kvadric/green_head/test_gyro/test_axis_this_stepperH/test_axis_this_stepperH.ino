#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Stepper.h>

#define STEPS 200

#define EnaX 7
#define EnbX 2

#define EnaY 8
#define EnbY 13
Stepper stepperX (STEPS, 9, 10, 11, 12);
Stepper stepperY (STEPS, 6, 5, 4, 3);

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

sensors_event_t event;
float ax, ay, Sy, Sx, Ay, Ax, Px, Py, ux, uy = 0;
long t, t0 = 0;
float ky, kiy, kx, kix, Ix, Iy, ey, ex, dt = 0;
int A_n = 100;

void setup() 
{
 Serial.begin(9600);
 Serial.println("Accelerometer Test"); Serial.println("");
  if (!accel.begin())
  {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1);
  }
 Serial.println("");
 stepperX.setSpeed(30);
 stepperY.setSpeed(30);
 
 pinMode(EnaX, OUTPUT);
 pinMode(EnbX, OUTPUT);
 pinMode(EnaY, OUTPUT);
 pinMode(EnbY, OUTPUT);
 for(int i = 0; i < 220; i++)
 {
 analogWrite(EnaX, i);
 analogWrite(EnbX, i);
 analogWrite(EnaY, i);
 analogWrite(EnbY, i);
 }
 
}

float getY()
{
  accel.getEvent(&event);
  float y = 0;
  y = event.acceleration.y;
  Sy = Sy + y - Ay;
  Ay = Sy / A_n;
  return Ay;
}

float getX()
{
  accel.getEvent(&event);
  float x = 0; 
  x = event.acceleration.x;
  Sx = Sx + x - Ax;
  Ax = Sx / A_n;
  return Ax;
}

void loop() 
{
  
  t0 = millis();
  dt = (millis() - t) / 1000.0; t = millis();
  ky =  -5;
  kiy = -1;
  kx =  -1.5;
  kix = -0.5;
  
  ay = getY();
  ax = getX();
  
  ey = ay;
  ex = ax;
  
  Px = kx * ex;
  Ix = kix * ex;
  ux = Px + Ix;
 
  Py = ky * ey;
  Iy = kiy * ey;
  uy = Py + Iy;

  Serial.print(Px);
  Serial.print("\t");
  Serial.print(Ix);
  Serial.print("\t");
  Serial.print(-ux);
  Serial.print("\t");
//  Serial.print(sign(ux));
  Serial.print("\t");
  Serial.print(Py);
  Serial.print("\t");
  Serial.print(Iy);
  Serial.print("\t");
  Serial.print(uy);
  Serial.print("\t");
 // Serial.print(sign(uy));
  Serial.print("\t");
  Serial.print("\n");

  stepperX.step(-ux);
  stepperY.step(uy);
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayDataRate(void)
{
  Serial.print  ("Data Rate:    ");

  switch (accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 ");
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 ");
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 ");
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 ");
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 ");
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 ");
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 ");
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 ");
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 ");
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 ");
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 ");
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 ");
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 ");
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 ");
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 ");
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 ");
      break;
    default:
      Serial.print  ("???? ");
      break;
  }
  Serial.println(" Hz");
}

void displayRange(void)
{
  Serial.print  ("Range:         +/- ");

  switch (accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print  ("16 ");
      break;
    case ADXL345_RANGE_8_G:
      Serial.print  ("8 ");
      break;
    case ADXL345_RANGE_4_G:
      Serial.print  ("4 ");
      break;
    case ADXL345_RANGE_2_G:
      Serial.print  ("2 ");
      break;
    default:
      Serial.print  ("?? ");
      break;
  }
  Serial.println(" g");
}
