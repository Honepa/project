#define A 3
#define B 2
#define pwm 10

void setup()
{
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop()
{
  digitalWrite(A, 1);
  digitalWrite(B, 0);
  for (int i = 0; i < 256; i++)
  {
    analogWrite(pwm, i);
    delay(20);
  }
  delay (2000);
  for (int i = 255; i >= 0; i--)
  {
    analogWrite(pwm, i);
    delay(20);
  }
  delay(50000);
}
