#include <Arduino.h>
#include "BasicStepperDriver.h"

#define MOTOR_STEPS 175
#define RPM 200

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

void setup() 
{
 stepperX.begin(RPM, MICROSTEPS);
 stepperY.begin(RPM, MICROSTEPS);
 stepperZ.begin(RPM, MICROSTEPS);
}

void loop() 
{
 int stp = 90;
 //for (int i = 100; i < 250; i ++)
 //{
 //stepperX.begin(i, MICROSTEPS);
 stepperX.rotate(stp);
 delay(5);
 //stepperX.rotate(-stp);
 while(1);
 //}

 /*
 stepperY.rotate(stp);
 delay(500);
 stepperY.rotate(-stp);

 
 stepperZ.rotate(stp);
 delay(500);
 stepperZ.rotate(-stp);
 //stepper.move(-MOTOR_STEPS*MICROSTEPS);
 */
 delay(1000);
 
}
