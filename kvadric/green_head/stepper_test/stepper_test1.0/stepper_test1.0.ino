#define EN       8
#define DR1      2
#define ST1      5

#define DR2      3
#define ST2      6

#define DR3      4
#define ST3      7
void setup()
{
  for(int i = 2; i < 8; i++) pinMode(i, 1);
  pinMode(13,  1);
  digitalWrite(EN, 0);
}

void loop()
{
  for (int i = 0; i < 100; i++)
  {
    digitalWrite(ST1, ! digitalRead(ST1));
    digitalWrite(DR1, ! digitalRead(DR1));
    digitalWrite( 13, ! digitalRead( 13));
    delayMicroseconds(1000);
  }
  for (int i = 0; i < 100; i++)
  {
    digitalWrite(ST2, ! digitalRead(ST2));
    digitalWrite( 13, ! digitalRead( 13));
    delayMicroseconds(1000);
  }
  for (int i = 0; i < 100; i++)
  {
    digitalWrite(ST3, ! digitalRead(ST3));
    digitalWrite( 13, ! digitalRead( 13));
    delayMicroseconds(1000);
  }
  delay(1000);
  digitalWrite(DR1, ! digitalRead(DR1));
  digitalWrite(DR2, ! digitalRead(DR2));
  digitalWrite(DR3, ! digitalRead(DR3));
}
