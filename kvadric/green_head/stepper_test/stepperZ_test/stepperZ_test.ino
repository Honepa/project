#include <Stepper.h>
int stepZ = 50;
Stepper myStepperZ(stepZ, 8, 9, 10,11);
void setup() 
{
 myStepperZ.setSpeed(300);

}

void loop() 
{
 myStepperZ.step(stepZ);
 delay(500);
 myStepperZ.step(-stepZ);
 delay(500);
}
