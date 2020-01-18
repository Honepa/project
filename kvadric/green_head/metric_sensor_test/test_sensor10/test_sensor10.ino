#define trig  3
#define echo  2
#define lazer 7

void setup() 
{
 pinMode(trig,  OUTPUT);
 pinMode(echo,  INPUT);
 pinMode(lazer, OUTPUT);
 Serial.begin(9600);
}
unsigned long in_data= 0;
double cm = 0; 
void loop() 
{
 digitalWrite(trig, 0);
 delayMicroseconds(5);
 digitalWrite(trig, 1);
 delayMicroseconds(10);
 digitalWrite(trig, 0);
 in_data = pulseIn(echo, 1);
 cm = (in_data / 2) / 29,1;
 Serial.print(in_data);
 Serial.print(" ");
 Serial.print(cm);
 Serial.print('\n');
 if(cm < 15 )
 {
  digitalWrite(lazer, 1); 
 }
 else
 {
  digitalWrite(lazer, 0);
 }
 //delay(1000);
}
