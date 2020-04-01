#include <Servo.h>

Servo front_axis;

#define in1 6
#define in2 7

void setup() 
{
 pinMode(in1, OUTPUT);
 pinMode(in2, OUTPUT);
 front_axis.attach(8);
}

void loop() 
{
 digitalWrite(in1, 1);
 digitalWrite(in2, 0);
 delay(150);
 front_axis.write(50);
}
