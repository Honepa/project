#define RELE_K3 6
#define RELE_K4 7
#define KNOK 8

int stop_motor()
{
  digitalWrite(RELE_K4, 1);
  digitalWrite(RELE_K3, 1);
}

int start_levo()
{
  digitalWrite(RELE_K4, 0);
  digitalWrite(RELE_K3, 1);
}

int start_pravo()
{
  digitalWrite(RELE_K4, 1);
  digitalWrite(RELE_K3, 0);
}

void setup() 
{
  pinMode(RELE_K4, OUTPUT);
  pinMode(RELE_K3, OUTPUT);
  pinMode(KNOK, INPUT);
  stop_motor();
}

void loop() 
{
  /*while(digitalRead(KNOK) == 0)
  {
    start_levo();
  }
  */
  start_pravo();
  delay(5000);
  while(digitalRead(KNOK) == 0)
  {
    start_pravo();
  }
  stop_motor();
  delay(3413453);
}
