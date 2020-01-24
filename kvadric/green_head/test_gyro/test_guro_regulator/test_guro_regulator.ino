#include <Wire.h>
#include <ITG3200.h>
ITG3200 gyro = ITG3200();
#include <Arduino.h>
#include "BasicStepperDriver.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#define MOTOR_STEPS 175
#define RPM 100

#define MICROSTEPS 1

#define DIRX  2
#define STEPX 5

#define DIRY  3
#define STEPY 6

#define DIRZ  4
#define STEPZ 7

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

BasicStepperDriver stepperX(MOTOR_STEPS, DIRX, STEPX);
BasicStepperDriver stepperY(MOTOR_STEPS, DIRY, STEPY);
BasicStepperDriver stepperZ(MOTOR_STEPS, DIRZ, STEPZ);

float x, y, z = 0;
int ix, iy, iz = 0;
float Ix, SumX, Ax, dx, e, k, ki , ie, Int_X, idx, vdx = 0;
int n_A, n = 0;
float dt = 0;
long t0, t = 0;
float stp = 0;
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
  
  switch(accel.getDataRate())
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
  
  switch(accel.getRange())
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

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  delay(1000);
  //gyro.init(ITG3200_ADDR_AD0_LOW);
 #ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
 accel.setRange(ADXL345_RANGE_16_G);
 displaySensorDetails();
  displayDataRate();
  displayRange();
 Serial.print("ZroCalibruting///");
  //gyro.zeroCalibrate(2500, 2);
  Serial.println("done");
  Serial.println("Accelerometer Test"); Serial.println("");
   sensors_event_t event; 
  accel.getEvent(&event);
  /*t = millis();
  double x0;//, y, z = 0;
  t0 = t;
  while(t - t0 < 2000)
  {
    gyro.readGyro(&x, &y, &z);
    dx = x - x0;
    dt = (millis() - t) / 1000.0; t = millis();
    idx = idx + dx * dt;
    n += 1;
    delay(1);
  }
  vdx = idx / n;
  Serial.print(n);
  Serial.print(dt);
  Serial.println(vdx);
  */
  Serial.println("Ax, Ix:");
  stepperX.begin(RPM, MICROSTEPS);
 stepperY.begin(RPM, MICROSTEPS);
 stepperZ.begin(RPM, MICROSTEPS);
}

void loop()
{
  sensors_event_t event; 
  accel.getEvent(&event);
 
  //t= millis();
  n_A = 10;
  k =  1;
  ki = 1;
  dt = (millis() - t) / 1000.0; t = millis();
  gyro.readGyro(&x, &y, &z);
  //Serial.println(x);
 // x -= 0.005025;
  SumX = SumX + x - Ax;
  Ax = SumX / n_A;
  e = Ax;
  Ix += ki * e * dt;
  Int_X = k * e + ki * Ix;
 // stp = max(min(Int_X, 15), -15);
 // stepperX.rotate(-stp);
  //Serial.print(Ax);
  //Serial.print(" ");
  //Serial.print(Ix);
  //Serial.print(" ");
  //Serial.print(Int_X);
  //Serial.print("\n");
  stp = 5;
  Serial.println(event.acceleration.y);
  if(event.acceleration.y < -1.5)
  {
    stepperX.rotate(stp);
  }
  else if (event.acceleration.y > 1.5)
  {
    stepperX.rotate(-stp);
  }
  else
  {
    stepperX.rotate(0);
  }
}
