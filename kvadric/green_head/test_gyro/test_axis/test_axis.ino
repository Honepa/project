#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "BasicStepperDriver.h"

#define MOTOR_STEPS 175//175
#define RPM 100

#define MICROSTEPS 1

#define DIRX  2
#define STEPX 5

#define DIRY  3
#define STEPY 6

#define DIRZ  4
#define STEPZ 7

BasicStepperDriver stepperX(MOTOR_STEPS, DIRX, STEPX);
BasicStepperDriver stepperY(MOTOR_STEPS, DIRY, STEPY);
BasicStepperDriver stepperZ(MOTOR_STEPS, DIRZ, STEPZ);

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
float y, Ay, Sy, ky, kiy, ey, Iy, Int_y, dt, ex, Ix, Int_x, Ax, x, Sx, kx, kix = 0;
float A_n = 100.0;
int stp = 5;
long t0, t = 0;
sensors_event_t event;

void setup()
{
  stepperX.begin(RPM, MICROSTEPS);
  stepperY.begin(RPM, MICROSTEPS);
  stepperZ.begin(RPM, MICROSTEPS);
  Serial.begin(115200);
  Serial.println("Accelerometer Test"); Serial.println("");
  if (!accel.begin())
  {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1);
  }
  // accel.setRange(ADXL345_RANGE_16_G);
  //displaySensorDetails();
  //displayDataRate();
  //displayRange();
  Serial.println("");
}

float getY()
{
  accel.getEvent(&event);
  float y = event.acceleration.y;
  Sy = Sy + y - Ay;
  Ay = Sy / A_n;
  return Ay;
}

float getX()
{
  accel.getEvent(&event);
  float x = event.acceleration.x;
  Sx = Sx + x - Ax;
  Ax = Sx / A_n;
  return Ax;
}
float Px, Py, ux, uy = 0;

int sign(float x)
{
  int r = 0;
  if (x < -0.5) r = -1;
  else if (x > 0.5) r = 1;
  else r = 0;
  return r;
}

void loop()
{
  t0 = millis();
  dt = (millis() - t) / 1000.0; t = millis();
  ky = -0.5;
  kiy = 0.0005;
  kx = -0.5;
  kix = 0.0005;

  y = getY();
  x = getX();//ay, ax
  
  ey = y;
  ex = x;
  //Iy = ki * e * dt;

  Px = kx * ex;
  Ix += kix * ex;
  ux = Px + Ix;

  Py = ky * ey;
  Iy += kiy * ey;
  uy = Py + Iy;

  Serial.print(x);
  Serial.print("\t");
  Serial.print(Px);
  Serial.print("\t");
  Serial.print(ux);
  Serial.print("\t");
  Serial.print(sign(ux));
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(Iy);
  Serial.print("\t");
  Serial.print(Py);
  Serial.print("\t");
  Serial.print(sign(uy));
  Serial.print("\t");
  Serial.print("\n");
  
  stepperX.move(sign(ux));
  stepperY.move(sign(uy));
  //stepperY.rotate(-Int_y);
  //stepperX.rotate(-Int_x);
  /*
    if(Ay < -0.5)
    {
    stepperY.rotate(stp);
    Serial.println("right!");
    }
    else if (Ay > 0.5)
    {
    stepperY.rotate(-stp);
    Serial.println("left!");
    }
    else
    {
    stepperY.rotate(0);
    Serial.println("normal");
    }
    if(millis() - t0 < 2000)
    {
    stepperY.rotate(0);
    }
  */
//  delay(10);
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
