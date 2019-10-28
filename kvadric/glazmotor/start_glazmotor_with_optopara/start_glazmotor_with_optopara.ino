#define motor1_A 3
#define motor1_B 2
#define motor1_pwm 10

#define INIT 0
#define SEK 1
int state = 0;

void setup()
{
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);

  pinMode(8, INPUT);
  Serial.begin(9600);
  digitalWrite(motor1_A, 1);
  digitalWrite(motor1_B, 0);
  for (int i = 0; i < 256; i++)
  {
    analogWrite(motor1_pwm, i);
    delay(10);
  }
}

int k = 0;

void loop()
{
  if (digitalRead(8) == 0)
  {
    k++;
    Serial.println(k); 
  }
  else 
  {
    Serial.println(k);
  }
}
