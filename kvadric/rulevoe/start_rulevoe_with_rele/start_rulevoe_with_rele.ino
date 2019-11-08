//танцует, не включать!!!!!!
#define motor1_A 4
#define motor1_B 5
#define motor1_pwm 3

#define motor2_A 12
#define motor2_B 13
#define motor2_pwm 11

#define PIN_STR_LFT 6
#define PIN_STR_RGT 7
#define PIN_STR_LIM 8
#define PIN_STR_POS A0

#define ST_STR_INIT       0
#define ST_STR_RIGHT      1
#define ST_STR_SAVE_R1    2
#define ST_STR_LEFT1      3
#define ST_STR_LEFT2      4
#define ST_STR_SAVE_R2    5
#define ST_STR_GO_MIDDLE1 6
#define ST_STR_GO_MIDDLE2 7
#define ST_STR_FAIL       8
#define ST_STR_WORK       9
#define ST_WORK           10

#define ST_WRK_TRON    0
#define ST_WRK_EHAT_1  1
#define ST_WRK_POWOROT 2
#define ST_WRK_PRYAMO  3
#define ST_WRK_EHAT_2  4
#define ST_WRK_STOP    5
#define ST_WRK_FAIL    6

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

long get_str_pos()
{
  long p = 0;
  for (int i = 0; i < 100; i++)
  {
    p += analogRead(PIN_STR_POS);
  }
  return p / 100;
}

int is_str_limit()
{
  int p = 0;
  for (int i = 0; i < 100; i++)
  {
    p += digitalRead(PIN_STR_LIM);
  }
  return p > 80;
}

void start_bc_motors()
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
}

void stop_bc_motors()
{
  for (int i = 255; i >= 0; i--)
  {
    analogWrite(motor1_pwm, i);
    analogWrite(motor2_pwm, i);
    delay(20);
  }
  digitalWrite(motor1_A, 0);
  digitalWrite(motor1_B, 0);

  digitalWrite(motor2_A, 0);
  digitalWrite(motor2_B, 0);
}

void setup()
{
  pinMode(6, 1);
  pinMode(7, 1);
  pinMode(8, 0);
  Serial.begin(9600);
  str_stop();
  ss_println();

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);

  pinMode(2, OUTPUT);
  pinMode(9, OUTPUT);
}

int state = ST_STR_INIT;
int work_state = 0;
long t  = 0;
long R1 = 0;
long R2 = 0;
int is_limit = 0;
int str_pos = 0;
int my_R, my_R1, my_R2;

int set_str_pos(int pos)
{
  int fff = 0;
  if (! is_limit)
  {
    if (millis() - t > 5000)
    {
      fff = 2;
    }

    if (abs(pos - str_pos) > 10)
    {
      if (pos > 0)
      {
        if (pos - str_pos > 0 )
        {
          str_left();
          fff = 0;
        }
      }
      else if (pos < 0)
      {
        if (pos - str_pos < 0)
        {
          str_right();
          fff = 0;
        }
      }

    }
    else
    {
      fff = 1;
    }
  }
  else
  {
    fff = 3;
  }
  return (fff);
}

void s_println()
{
  String tsate[11] = {"INIT", "RIGHT", "SR1", "L1", "L2", "SR2", "M1", "M2", "F", "STRWORK", "WORK"};
  String wstate[7] = {"TRON", "EHAT_1", "POWOROT", "PRYAMO", "EHAT_2", "STOP", "F"};
  Serial.print(str_pos);        Serial.print('\t');
  Serial.print(is_limit);       Serial.print('\t');
  Serial.print(R1);             Serial.print('\t');
  Serial.print(R2);             Serial.print('\t');
  Serial.print(t);              Serial.print('\t');
  Serial.print(state);          Serial.print('\t');
  Serial.print(work_state);     Serial.print('\t');
  Serial.print(str_pos);
  Serial.print('\n');
}

void ss_println()
{
  Serial.print("pos");         Serial.print('\t');
  Serial.print("limit");       Serial.print('\t');
  Serial.print("R1");          Serial.print('\t');
  Serial.print("R2");          Serial.print('\t');
  Serial.print("t");           Serial.print('\t');
  Serial.print("state");       Serial.print('\t');
  Serial.print("work_state");  Serial.print('\t');
  Serial.print("str_pos");     Serial.print('\n');
}

void loop()
{
  is_limit = is_str_limit();
  str_pos = get_str_pos() - R1 - R2 / 2;
  s_println();
  switch (state)
  {

    case ST_STR_INIT:
      t = millis();
      state = ST_STR_RIGHT;
      break;
    case ST_STR_RIGHT:
      if (! is_limit)
      {
        if (millis() - t > 2500)
        {
          state = ST_STR_FAIL;
        }
        str_right();
      }
      else
      {
        t = millis();
        state = ST_STR_SAVE_R1;
      }
      break;
    case ST_STR_SAVE_R1:
      str_stop();
      if (millis() - t > 1000)
      {
        R1 = str_pos;
        my_R1 = analogRead(A0);
        t = millis();
        state = ST_STR_LEFT1;
      }
      break;
    case ST_STR_LEFT1:
      if (is_limit)
      {
        if (millis() - t > 1000)
        {
          state = ST_STR_FAIL;
        }
        str_left();
      } else
      {
        state = ST_STR_LEFT2;
      }
      break;
    case ST_STR_LEFT2:
      if (!is_limit)
      {
        if (millis() - t > 2500)
        {
          state = ST_STR_FAIL;
        }
        str_left();
      } else if (is_limit)
      {
        t = millis();
        state = ST_STR_SAVE_R2;
      }
      break;
    case ST_STR_SAVE_R2:
      str_stop();
      if (millis() - t > 1100)
      {
        R2 = str_pos;
        my_R2 = analogRead(A0);
        t = millis();
        state = ST_STR_GO_MIDDLE1;
      }
      break;
    case ST_STR_GO_MIDDLE1:
      if (is_limit)
      {
        if (millis() - t > 200)
        {
          state = ST_STR_FAIL;
        }
        str_right();
      }
      else
      {
        t = millis();
        state = ST_STR_GO_MIDDLE2;
      }
      break;
    case ST_STR_GO_MIDDLE2:
      if (! is_limit)
      {
        if (millis() - t > 2500)
        {
          state = ST_STR_FAIL;
        }
        str_right();
      }
      else
      {
        state = ST_STR_FAIL;
      }
      if (str_pos <= 0)
      {
        state = ST_STR_WORK;
      }
      break;
    case ST_STR_FAIL:
      str_stop();
      break;
    case ST_STR_WORK:
      str_stop();
      state = ST_WORK;
      break;
    case ST_WORK:
      switch (work_state)
      {
        case ST_WRK_PRYAMO:
          //work_state = ST_WRK_STOP;
          //digitalWrite(2, 0);
          //digitalWrite(9, 0);
          if (! is_limit)
          {
            if (millis() - t > 2500)
            {
              work_state = ST_STR_FAIL;
            }
            if (str_pos >= 0)
            {
              str_right();
            }
            else if (str_pos <= 0)
            {
              str_left();
            }
            else
            {
              work_state = ST_WRK_EHAT_2;
            }
          }
          break;
          
        case ST_WRK_TRON:
          //start_bc_motors();
          t = millis();
          work_state = ST_WRK_EHAT_1;
          break;

        case ST_WRK_EHAT_1:
          if (millis() - t  >= 2000)
          {
            t = millis();
            work_state = ST_WRK_POWOROT;
          }
          break;

        case ST_WRK_POWOROT:
          int f = set_str_pos(-40);
          if (f == 1)
          {
            str_stop();
            t = millis();
            work_state = ST_WRK_PRYAMO;

          }
          else if ( f == 2 )
          {
            str_stop();
            work_state = ST_WRK_FAIL;
          }
          else if ( f == 3 )
          {
            str_stop();
            work_state = ST_WRK_FAIL;
          }
          break;

        

        case ST_WRK_EHAT_2:
          if (millis() - t >= 500)
          {
            work_state = ST_WRK_STOP;
          }
          break;

        case ST_WRK_STOP:
          stop_bc_motors();
          str_stop();
          break;

        case ST_WRK_FAIL:
          str_stop();
          stop_bc_motors();
          break;
      }
      break;
  }
}
