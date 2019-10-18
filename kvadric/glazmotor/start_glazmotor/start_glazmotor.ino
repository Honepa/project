#define motor1_A 3
#define motor1_B 2
#define motor1_pwm 10

void setup()
{
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop()
{
  digitalWrite(motor1_A, 1);
  digitalWrite(motor1_B, 0);
  for (int i = 0; i < 256; i++)
  {
    analogWrite(motor1_pwm, i);
    delay(20);
  }
  delay (2000);
  for (int i = 255; i >= 0; i--)
  {
    analogWrite(motor1_pwm, i);
    delay(20);
  }
  delay(50000);
}
