#define EN      8
#define DERECKT 2
#define steps   5
void setup()
{
  pinMode(EN,      1);//krasny
  pinMode(DERECKT, 1);//oranjev
  pinMode(steps,   1);//zel
  pinMode(13,      1);//cin
  digitalWrite(EN,      0);
  digitalWrite(DERECKT, 0);
}

void loop()
{
  for (int i = 0; i < 100; i++)
  {
    digitalWrite(steps, ! digitalRead(steps));
    digitalWrite(   13, ! digitalRead(   13));
    delayMicroseconds(1000);
  }
  delay(1000);
  digitalWrite(DERECKT, ! digitalRead(DERECKT));
}
