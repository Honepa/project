#include <Stepper.h>

#define STEPS 200

#define EnaX 7
#define EnbX 2

#define EnaY 8
#define EnbY 13
Stepper stepperX (STEPS, 9, 10, 11, 12);
Stepper stepperY (STEPS, 6, 5, 4, 3);

void setup() 
{
 stepperX.setSpeed(30);
 stepperY.setSpeed(30);
 
 pinMode(EnaX, OUTPUT);
 pinMode(EnbX, OUTPUT);
 pinMode(EnaY, OUTPUT);
 pinMode(EnbY, OUTPUT);
 for(int i = 0; i < 210; i++)
 {
 analogWrite(EnaX, i);
 analogWrite(EnbX, i);
 analogWrite(EnaY, i);
 analogWrite(EnbY, i);
 }

}

void loop() 
{
  stepperX.step(50);
  stepperY.step(50);
  stepperX.step(-50);
  stepperY.step(-50);
}
