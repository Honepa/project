#define INIT 0
#define WORK 1
#define FAIL 2

#define PIN_STR_LFT       6
#define PIN_STR_RGT       7
#define PIN_STR_LIM_LEFT  8
#define PIN_STR_LIM_RIGHT 3
#define PIN_STR_POS A0

#define ST_STR_INIT    0
#define ST_STR_RIGHT   1
#define ST_STR_SAVE_R1 2
#define ST_STR_LEFT    3
#define ST_STR_SAVE_R2 4
#define ST_STR_MIDDLE  5
#define ST_STR_WORK    6
#define ST_STR_FAIL    7
long t, t0 = 0;
int str_state = 0;
int state = 0;
int is_limit_left, is_limit_right, R1, R2, str_pos, old_str_pos = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(6, 1);
  pinMode(7, 1);
  pinMode(8, 0);
  pinMode(3, 0);
  t = millis();
  t0 = millis();
  str_stop();

}
int sturn = 0;
void loop()
{
  Serial.print(state);
  Serial.print(" ");
  Serial.print(str_pos);
  Serial.println(" ");
  int sturn = 0;
  is_limit_left = limit_left();
  is_limit_right = limit_right();
  str_pos = get_str_pos() - R1 - R2 / 2;
  switch(state)
  {
    case INIT:
    {
      sturn = str_init();
      if (sturn == 1)
      {
        t = millis();
        state = WORK;
      }
      else if (sturn == 2)
      {
        str_stop();
        state = FAIL;
      }
      break;
    }
    case WORK:
    {
      break;
    }
    case FAIL:
    {
      break;
    }
  }
}
int limit_left()
{
  int left = 0;
  if (digitalRead(PIN_STR_LIM_LEFT) == 0)
  {
    left = 1;
  }
  else
  {
    left = 0;
  }
  return left;
}

int limit_right()
{
  int right = 0;
  if (digitalRead(PIN_STR_LIM_RIGHT) == 0)
  {
    right = 1;
  }
  else
  {
    right = 0;
  }
  return right;
}

int str_init()
{
  int code = 0;
  switch (str_state)
  {
    case ST_STR_INIT:
      {
        t = millis();
        str_state = ST_STR_RIGHT;
        break;
      }
    case ST_STR_RIGHT:
      {
        if (!is_limit_right)
        {
          if (millis() - t > 2500)
          {
            str_state = ST_STR_FAIL;
          }
          str_right();
        }
        else
        {
          t = millis();
          str_stop();
          str_state = ST_STR_SAVE_R1;
        }
        break;
      }
    case ST_STR_SAVE_R1:
      {
        if (millis() - t > 250)
        {
          R1 = str_pos;
          t = millis();
          str_state = ST_STR_LEFT;
        }
        break;
      }
    case ST_STR_LEFT:
      {
        if (!is_limit_left)
        {
          if (millis() - t > 2500)
          {
            str_state = ST_STR_FAIL;
          }
          str_left();
        }
        else
        {
          t = millis();
          str_stop();
          str_state = ST_STR_SAVE_R2;
        }
        break;
      }
    case ST_STR_SAVE_R2:
      {
        str_stop();
        if (millis() - t > 250)
        {
          R2 = str_pos;
          t = millis();
          str_state = ST_STR_MIDDLE;
        }
        break;
      }
    case ST_STR_MIDDLE:
      {
        if (!is_limit_right)
        {
          if (millis() - t > 2500)
          {
            str_stop();
            str_state = ST_STR_FAIL;
          }
          str_right();
        }
        else
        {
          str_stop();
          str_state = ST_STR_FAIL;
        }
        if (str_pos <= 0)
        {
          str_state = ST_STR_WORK;
        }
        break;
      }
    case ST_STR_WORK:
      {
        str_stop();
        code = 1;
        break;
      }
    case ST_STR_FAIL:
      {
        str_stop();
        code = 2;
        break;
      }
  }
  return code;
}

long get_str_pos() //считывние реостата
{
  long p = 0;
  for (int i = 0; i < 100; i++)
  {
    p += analogRead(PIN_STR_POS);
  }
  return p / 100;
}

void str_stop()
{
  digitalWrite(PIN_STR_LFT, 0);
  digitalWrite(PIN_STR_RGT, 0);
}

void str_right()
{
  digitalWrite(PIN_STR_LFT, 1);
  digitalWrite(PIN_STR_RGT, 0);
}

void str_left()
{
  digitalWrite(PIN_STR_LFT, 0);
  digitalWrite(PIN_STR_RGT, 1);
}
