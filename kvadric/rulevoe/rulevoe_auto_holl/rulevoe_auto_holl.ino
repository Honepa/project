#define INIT        0
#define INIT_KOMPAS 1
#define WORK        2
#define STOP        3

#define PIN_STR_LFT       6
#define PIN_STR_RGT       7
#define PIN_STR_LIM_LEFT  18
#define PIN_STR_LIM_RIGHT 19
#define PIN_STR_POS A0

#define ST_STR_INIT    0
#define ST_STR_RIGHT   1
#define ST_STR_SAVE_R1 2
#define ST_STR_LEFT    3
#define ST_STR_SAVE_R2 4
#define ST_STR_MIDDLE  5
#define ST_STR_WORK    6
#define ST_STR_FAIL    7

volatile int is_lim_right = LOW;
volatile int is_lim_left  = LOW;
int str_state, state = 0;
int R1, R2, str_pos, old_str_pos = 0;
long t = 0;
void setup()
{
  attachInterrupt(4, is_lim_rght, CHANGE);
  attachInterrupt(5, is_lim_lft,  CHANGE );
  
  Serial.begin(9600);
  
  pinMode(6, 1);
  pinMode(7, 1);
  pinMode(8, 0);
  pinMode(3, 0);
  
}

void loop()
{
  Serial.print(str_state);
  Serial.print(" ");
  Serial.print(state);
  Serial.print(" ");
  Serial.print(is_lim_right);
  Serial.print(" ");
  Serial.print(is_lim_left);
  Serial.print(" ");
  Serial.print(str_pos);
  Serial.print(" ");
  Serial.print(digitalRead(8));
  Serial.print(" ");
  Serial.print(digitalRead(3));
  Serial.println(" ");
  int sturn = 0;
  str_pos = get_str_pos() - R1 - R2 / 2;
  switch(state)
  {
    case INIT:
    {
      sturn = str_init();
        if (sturn == 1)
        {
          t = millis();
          str_stop();
          state = WORK;
        }
        else if (sturn == 2)
        {
          str_stop();
          state = STOP;
        }

      break;
    }
    case INIT_KOMPAS:
    {
      break;
    }
    case WORK:
    {
      break;
    }
    case STOP:
    {
      break;
    }
  }
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
        if (!is_lim_right)
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
          str_stop;
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
          break;
        }
      }
    case ST_STR_LEFT:
      {
        if (!is_lim_left)
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
        if (!is_lim_right)
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

void is_lim_rght()
{
  is_lim_right = !is_lim_right;
}

void is_lim_lft()
{
  is_lim_left = 1;
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
