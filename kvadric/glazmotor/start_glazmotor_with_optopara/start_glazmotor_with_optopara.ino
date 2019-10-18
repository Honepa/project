#define motor1_A 3
#define motor1_B 2
#define motor1_pwm 10

void setup() 
{
 pinMode(3, OUTPUT);
 pinMode(2, OUTPUT);
 pinMode(13, OUTPUT);
 pinMode(8, INPUT_PULLUP);

}

void loop() 
{
  digitalWrite(motor1_A, 1);
  digitalWrite(motor1_B, 0);
  for (int i = 0; i < 256; i++)
  {
    analogWrite(motor1_pwm, i);
    delay(10);
  }
  int k = 0;
  while (k < 6)
  {
   if (digitalRead (8) == 0)
   {
    k++; 
   }
  }
  for (int i = 255; i >= 0; i--)
  {
    analogWrite(motor1_pwm, i);
    delay(10);     
  }
  delay (50000);
  
}
