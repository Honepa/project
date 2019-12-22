#define INIT        0
#define CMP_TURN    1
#define CMP_ON      2
#define EXPECT_KNOK 3
#define ALL_OFF     4
#define ARD_OFF     5

#define LED_PWR_OK 3
#define LED_PWR_ON 2
#define LED_CMP_ON 4

#define knok A1
#define key  A0

#define big_ard   A7
#define rele_mass A6
#define rele_cmp  A5

float t = 0;

void setup()
{
  pinMode(LED_PWR_OK, OUTPUT);
  pinMode(LED_PWR_ON, OUTPUT);
  pinMode(LED_CMP_ON, OUTPUT);

  pinMode(knok, INPUT);
  pinMode(key,  INPUT);

  pinMode(big_ard,   OUTPUT);
  pinMode(rele_mass, OUTPUT);
  pinMode(rele_cmp,  OUTPUT);

  t = millis();

  Serial.begin(9600);

  digitalWrite(rele_mass, 1);
  digitalWrite(rele_cmp,  1);

  digitalWrite(LED_PWR_OK, 1);
  digitalWrite(9, 0); //не трогать, dont touch, nicht!
}

int state = 0;
int key_on, knok_on = 0;
String arr_st[7] = {"INIT", "CMP_TURN", "CMP_ON", "EXPECT_KNOK", "ALL_OFF", "ARD_OFF"};
void loop()
{
  key_on = is_key();
  knok_on = is_knok();
  Serial.print(arr_st[state]);
  Serial.print(" ");
  Serial.print(key_on);
  Serial.print(" ");
  Serial.print(knok_on);
  Serial.println(" ");
  switch (state)
  {
    case INIT:
      {
        digitalWrite(LED_PWR_OK, 1);
        if ((key_on == 1) and (knok_on == 1))
        {
          digitalWrite(LED_PWR_ON, 1);
          digitalWrite(rele_mass, 0);
          t = millis();
          state = CMP_TURN;
        }
        else if (knok_on == 3)
        {
          digitalWrite(rele_mass, 1);
        }
        break;
      }
    case CMP_TURN:
      {
        digitalWrite(rele_cmp, 0);
        if ((millis() - t > 31000) and (knok_on == 1))
        {
          digitalWrite(LED_CMP_ON, 1);
          state = EXPECT_KNOK;
        }
        else if (knok_on == 3)
        {
          digitalWrite(rele_mass, 1);
          digitalWrite(rele_cmp,  1);

          digitalWrite(LED_CMP_ON, 0);
          digitalWrite(LED_PWR_ON, 0);
          state = INIT;
        }
        break;
      }
    case EXPECT_KNOK:
      {
        if (knok_on == 2)
        {
          state = ARD_OFF;
          digitalWrite(LED_CMP_ON, 0);
          digitalWrite(LED_PWR_ON, 0);
        }
        else if (knok_on == 3)
        {

          digitalWrite(LED_CMP_ON, 0);
          digitalWrite(LED_PWR_ON, 0);
          state = ALL_OFF;
        }
        else if (key_on == 1)
        {
          int k = 0;
          while (k < 25)
          {
            for (int i = 2; i < 5; i++)
            {
              digitalWrite(i, 1);
              delay(100);
              digitalWrite(i, 0);
            }
            k++;
          }
          k = 0;
          digitalWrite(LED_PWR_ON, 1);
          digitalWrite(LED_CMP_ON, 1);
          digitalWrite(LED_PWR_OK, 1);
          state = EXPECT_KNOK;
        }
        break;
      }
    case ARD_OFF:
      {
        digitalWrite(big_ard, 1);
        state = INIT;
        break;
      }
    case ALL_OFF:
      {
        digitalWrite(rele_mass, 1);
        digitalWrite(rele_cmp,  1);
        state = INIT;
        break;
      }
  }
}

int is_key()
{
  int code_key = 0;
  int is_key = 0;
  if (digitalRead(key))
  {
    for (int i = 1; i <= 1000; i++)
    {
      is_key += digitalRead(key);
      delay(1);
    }
    if (is_key > 800)
    {
      code_key = 1;
    }
    else
    {
      code_key = 0;
    }
  }
  else
  {
    code_key = 0;
  }
  return code_key;
}

int is_knok()
{
  int code_knok = 1;
  int is_knok = 0;
  if (digitalRead(knok) == 0)
  {
    for (int i = 1; i <= 1000; i++)
    {
      is_knok += !digitalRead(knok);
      delay(1);
    }
    if (is_knok > 150)
    {
      if (digitalRead(knok) == 0)
      {
        for (int i = 1; i <= 3000; i++)
        {
          is_knok += !digitalRead(knok);
          delay(1);
        }
        if (is_knok > 3800)
        {
          code_knok = 3;
          is_knok = 0;
        }
        else
        {
          code_knok = 1;
          is_knok = 0;
        }
      }
      else
      {
        code_knok = 2;
      }
    }
    else
    {
      code_knok = 1;
      is_knok = 0;
    }
  }
  else
  {
    code_knok = 1;
  }
  return code_knok;
}
