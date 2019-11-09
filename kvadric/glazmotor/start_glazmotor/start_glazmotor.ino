
#define motor1_A 4
#define motor1_B 5
#define motor1_pwm 3

#define motor2_A 12
#define motor2_B 13
#define motor2_pwm 11

void setup()
{
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop()
{
  digitalWrite(motor1_A, 1);
  digitalWrite(motor1_B, 0);
  
  digitalWrite(motor2_A, 1);
  digitalWrite(motor2_B, 0);
  for (int i = 0; i < 256; i++)
  {
    analogWrite(motor1_pwm, i);
    analogWrite(motor2_pwm, i);
    delay(20);
  }
  delay (5000);
  for (int i = 255; i >= 0; i--)
  {
    analogWrite(motor1_pwm, i);
    analogWrite(motor2_pwm, i);
    delay(20);
  }
  digitalWrite(motor1_A, 0);
  digitalWrite(motor1_B, 1);
  
  digitalWrite(motor2_A, 0);
  digitalWrite(motor2_B, 1);
  delay(2000);
    for (int i = 0; i < 256; i++)
  {
    analogWrite(motor1_pwm, i);
    analogWrite(motor2_pwm, i);
    delay(20);
  }
  delay (5000);
  for (int i = 255; i >= 0; i--)
  {
    analogWrite(motor1_pwm, i);
    analogWrite(motor2_pwm, i);
    delay(20);
  }
  delay(500000);
}
