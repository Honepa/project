#define fn_1  2
#define fn_2  9

#define motor1_A   4
#define motor1_B   5
#define motor1_pwm 3

#define motor2_A   12
#define motor2_B   13
#define motor2_pwm 11

#define PIN_STR_LFT 6
#define PIN_STR_RGT 7
#define PIN_STR_LIM 8
#define PIN_STR_POS A0

#define INIT 0
#define WORK 1
#define FAIL 2
#define END  3

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

#define INIT_WORK   0
#define WRK_EHAT1   1
#define WRK_POWOROT 2
#define WRK_EHAT2   3
#define WRK_PRYAMO  4
#define WRK_EHAT3   5
#define WRK_STOP    6
//#define poworot_1  1
//#define ehat_1     2
//#define poworot_2  3
//#define ehat_2     4
//#define poworot_3  5
//#define ehat_3    6
#define WRK_FAIL   7

#define OLD_POS      0
#define POS_GO_RIGHT 1
#define POS_GO_LEFT  2

#define OLD_POS        0
#define POS_GO_RIGHT_1 1
#define POS_GO_RIGHT_2 2
#define POS_GO_LEFT_1  3
#define POS_GO_LEFT_2  4
//вкл фонарей ↴
void fn_vkl()
{
  digitalWrite(fn_1, 0);
  digitalWrite(fn_2, 0);
}
// движение мотора рулевого управления ↴
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

void str_go(int dir)
{
  digitalWrite(PIN_STR_LFT, dir > 0 ? 1 : 0);
  digitalWrite(PIN_STR_RGT, dir < 0 ? 1 : 0);
}

//Датчики рулевого управления ↴
long get_str_pos() //считывние реостата
{
  long p = 0;
  for (int i = 0; i < 100; i++)
  {
    p += analogRead(PIN_STR_POS);
  }
  return p / 100;
}

int is_str_limit() // концевиики
{
  int p = 0;
  for (int i = 0; i < 100; i++)
  {
    p += digitalRead(PIN_STR_LIM);
  }
  return p > 80;
}

//запуск двигателей движения (для смены стороны обязательно использовать стоп) ↴
void start_bc_motors_pr() //вперёд
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

void start_bc_motors_bk() //назад
{
  digitalWrite(motor1_A, 0);
  digitalWrite(motor1_B, 1);

  digitalWrite(motor2_A, 0);
  digitalWrite(motor2_B, 1);

  for (int i = 0; i < 256; i++)
  {
    analogWrite(motor1_pwm, i);
    analogWrite(motor2_pwm, i);
  }
}
void stop_bc_motors() // стоп
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

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);

  pinMode(fn_1, OUTPUT);
  pinMode(fn_2, OUTPUT);
  digitalWrite(fn_1, 1);
  digitalWrite(fn_2, 1);
  
  str_stop();
  ss_println();
  fn_vkl();
}
// область видимости переменных в С!
int str_state = 0;
int prg_state = 0;
int wrk_state = 0;
int pos_not_lim_st = 0;
int pos_is_lim_st = 0;
long t  = 0;
long R1 = 0;
long R2 = 0;
int is_limit = 0;
int str_pos = 0;
int old_str_pos = 0;
int my_R, my_R1, my_R2;
int old_ppos = 0;

int str_init()
{
  int code = 0;
  switch (str_state)
  {
    case ST_STR_INIT:
      t = millis();
      str_state = ST_STR_RIGHT;
      break;

    case ST_STR_RIGHT:
      if (! is_limit)
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
        str_state = ST_STR_SAVE_R1;
      }
      break;

    case ST_STR_SAVE_R1:
      str_stop();
      if (millis() - t > 1000)
      {
        R1 = str_pos;
        t = millis();
        str_state = ST_STR_LEFT1;
      }
      break;

    case ST_STR_LEFT1:
      if (is_limit)
      {
        if (millis() - t > 1000)
        {
          str_state = ST_STR_FAIL;
        }
        str_left();
      }
      else
      {
        str_state = ST_STR_LEFT2;
      }
      break;

    case ST_STR_LEFT2:
      if (! is_limit)
      {
        if (millis() - t > 2500)
        {
          str_state = ST_STR_FAIL;
        }
        str_left();
      }
      else if (is_limit)
      {
        t = millis();
        str_state = ST_STR_SAVE_R2;
      }
      break;

    case ST_STR_SAVE_R2:
      str_stop();
      if (millis() - t > 1100)
      {
        R2 = str_pos;
        my_R2 = analogRead(A0);
        t = millis();
        str_state = ST_STR_GO_MIDDLE1;
      }
      break;
    case ST_STR_GO_MIDDLE1:
      if (is_limit)
      {
        if (millis() - t > 200)
        {
          str_state = ST_STR_FAIL;
        }
        str_right();
      }
      else
      {
        t = millis();
        str_state = ST_STR_GO_MIDDLE2;
      }
      break;
    case ST_STR_GO_MIDDLE2:
      if (! is_limit)
      {
        if (millis() - t > 2500)
        {
          str_state = ST_STR_FAIL;
        }
        str_right();
      }
      else
      {
        str_state = ST_STR_FAIL;
      }
      if (str_pos <= 0)
      {
        str_state = ST_STR_WORK;
      }
      break;
    case ST_STR_FAIL:
      str_stop();
      code = 2;
      break;
    case ST_STR_WORK:
      str_stop();
      code = 1;
      break;
  }
  return code;
}

int set_str_pos(int pos)
{
  int code = 0;
  int k;
  old_ppos > pos ?  k = 1 :  k = -1;
  if ((pos > -80) or (pos < 80))
  {
    if (! is_limit)
    {
      str_go(k);
      if (millis() - t > 2500)
      {
        str_stop();
        code = 3;
      }
      if (k * (str_pos - pos) < 0)
      {
        str_stop();
        code = 1;
      }
    }
    else
    {
      str_stop();
      code = 2;
    }
  }
  else
  {
    str_stop();
    code = 3;
  }
  return code;
}

int go_to_pos(int pos)
{
  int code = 0;
  if (!(pos == 0))
  {
    if (! is_limit)
    {
      if (millis() - t > 5000)
      {
        str_stop();
        code = 3;
      }
      if (abs(pos - str_pos) > 10)
      {
        if (pos > 0 )
        {
          str_left();
        }
        else if (pos < 0 )
        {
          str_right();
        }

      }
      else
      {
        str_stop();
        code = 1;
      }
    }
    else
    {
      str_stop();
      code = 2;
    }
  }
  else
  {
    if (! is_limit)
    {
      switch (pos_not_lim_st)
      {
        case OLD_POS:
          int old_pos = str_pos;
          if (old_pos > 0)
          {
            pos_not_lim_st = POS_GO_RIGHT;
          }
          else if (old_pos < 0)
          {
            pos_not_lim_st = POS_GO_LEFT;
          }
          else
          {
            str_stop();
            code = 1;
          }
          break;

        case POS_GO_RIGHT:
          str_right();
          if (str_pos <= 0)
          {
            str_stop();
            code = 1;
          }
          else
          {
            code = 0;
          }
          break;

        case POS_GO_LEFT:
          str_left();
          if (str_pos >= 0)
          {
            str_stop();
            code = 1;
          }
          else
          {
            code = 0;
          }
          break;
      }
    }
    else
    {
      switch (pos_is_lim_st)
      {
        case OLD_POS:
          int old_pos = str_pos;
          if (old_pos > 0)
          {
            t = millis();
            pos_is_lim_st = POS_GO_RIGHT_1;
          }
          else if (old_pos < 0)
          {
            t = millis();
            pos_is_lim_st = POS_GO_LEFT_1;
          }
          else
          {
            str_stop();
            code = 1;
          }
          break;

        case POS_GO_RIGHT_1:
          if (is_limit)
          {
            if (millis() - t > 200)
            {
              str_stop();
              code = 3;
            }
            str_right();
          }
          else
          {
            t = millis();
            pos_is_lim_st = POS_GO_RIGHT_2;
          }
          break;

        case POS_GO_RIGHT_2:
          if (! is_limit)
          {
            if (millis() - t > 2500)
            {
              str_stop();
              code = 3;
            }
            str_right();
          }
          else
          {
            str_stop();
            code = 2;
          }
          if (str_pos <= 0)
          {
            str_stop();
            code = 1;
          }
          break;

        case POS_GO_LEFT_1:
          if (is_limit)
          {
            if (millis() - t > 200)
            {
              str_stop();
              code = 3;
            }
            str_left();
          }
          else
          {
            t = millis();
            pos_is_lim_st = POS_GO_LEFT_2;
          }
          break;

        case POS_GO_LEFT_2:
          if (! is_limit)
          {
            if (millis() - t > 2500)
            {
              str_stop();
              code = 3;
            }
            str_left();
          }
          else
          {
            str_stop();
            code = 2;
          }
          if (str_pos >= 0)
          {
            str_stop();
            code = 1;
          }
          break;
      }
    }
  }
  return code;
}

void s_println()
{
  String str_states[11] = {"S_INIT", "RIGHT", "SR1", "L1", "L2", "SR2", "M1", "M2", "F", "STRWORK", "WORK"};
  String wrk_states[7] =  {"W_INIT", "EHAT_1", "POWOROT", "PRYAMO", "EHAT_2", "STOP", "F"};
  String prg_states[4] =  {"P_INIT", "WORK", "FAIL", "END"};
  Serial.print(str_pos);        Serial.print('\t');
  Serial.print(is_limit);       Serial.print('\t');
  Serial.print(R1);             Serial.print('\t');
  Serial.print(R2);             Serial.print('\t');
  Serial.print(t);              Serial.print('\t');
  Serial.print(prg_states[prg_state]); Serial.print('\t');
  Serial.print(str_states[str_state]); Serial.print('\t');
  Serial.print(wrk_states[wrk_state]); Serial.print('\t');
  Serial.print(str_pos);        Serial.print('\t');
  Serial.print('\n');
  //delay(500);
}


void ss_println()
{
  Serial.print("pos");         Serial.print('\t');
  Serial.print("limit");       Serial.print('\t');
  Serial.print("R1");          Serial.print('\t');
  Serial.print("R2");          Serial.print('\t');
  Serial.print("t");           Serial.print('\t');
  Serial.print("prg_state");   Serial.print('\t');
  Serial.print("str_state");   Serial.print('\t');
  Serial.print("wrk_state");   Serial.print('\t');
  Serial.print("str_pos");     Serial.print('\n');
}

void loop()
{
  is_limit = is_str_limit();
  str_pos = get_str_pos() - R1 - R2 / 2;
  s_println();
  //delay(1000);
  switch (prg_state)
  {
    case INIT:
      {
        int f = str_init();
        if (f == 1)
        {
          prg_state = WORK;
        }
        else if (f == 2)
        {
          prg_state = FAIL;
        }
        break;
      }
    case WORK:
      {
        switch (wrk_state)
        {
          case INIT_WORK:
            {
              t = millis();
              start_bc_motors_pr();
              wrk_state = WRK_EHAT1;
              break;
            }
         
          case WRK_EHAT1:
            {
              if (millis() - t > 3500)
              {
                t = millis();
                old_ppos = str_pos;
                wrk_state = WRK_POWOROT;
              }
              break;
            }

          case WRK_POWOROT:
            {
              int f = set_str_pos(-40);
              if (f == 1)
              {
                str_stop();
                t = millis();
                old_ppos = str_pos;
                wrk_state = WRK_EHAT2;
              }
              else if (f == 2)
              {
                str_stop();
                wrk_state = WRK_FAIL;
              }
              break;
            }

          case WRK_EHAT2:
          {
            if (millis() - t > 1500)
            {
              t = millis();
              wrk_state = WRK_PRYAMO;
            }
            break;
          }
          case WRK_PRYAMO:
            {
              
              int ff = set_str_pos(0);
              if (ff == 1)
              {
                str_stop();
                t = millis();
                wrk_state = WRK_EHAT3;
              }
              else if ((ff == 2) or ff == 3)
              {
                str_stop;
                wrk_state = WRK_FAIL;
              }
              break;
            }

          case WRK_EHAT3:
            {
              if (millis() - t > 1500)
              {
                wrk_state = WRK_STOP;
              }
              break;
            }

          case WRK_STOP:
            {
              str_stop();
              stop_bc_motors();
              prg_state = END;
              break;
            }
          
          /*
          case poworot_1:
          {
            int f = set_str_pos(-60);
              if (f == 1)
              {
                str_stop();
                t = millis();
                old_ppos = str_pos;
                wrk_state = ehat_1;
              }
              else if (f == 2)
              {
                str_stop();
                wrk_state = WRK_FAIL;
              }
              break;
          }
         case ehat_1:
         {
          start_bc_motors_pr();
          if ( millis() - t > 6000)
          {
            stop_bc_motors();
            old_ppos = str_pos;
            wrk_state = poworot_2;
          }
          break;
         }

         case poworot_2:
         {
          int ff = set_str_pos(60);
              if (ff == 1)
              {
                str_stop();
                t = millis();
                
                wrk_state = ehat_2;
              }
              else if (ff == 2)
              {
                str_stop();
                wrk_state = WRK_FAIL;
              }
            break;
         }
         case ehat_2:
         {
          start_bc_motors_bk();
          if(millis() - t > 3500)
          {
            stop_bc_motors();
            old_ppos = str_pos;
            wrk_state = poworot_3;
          }
          break;
         }
         case poworot_3:
         {
          int fff = set_str_pos(-60);
              if (fff == 1)
              {
                str_stop();
                t = millis();
                //old_ppos = str_pos;
                wrk_state = ehat_3;
              }
              else if (fff == 2)
              {
                str_stop();
                wrk_state = WRK_FAIL;
              }
           
          break;
         }
        case ehat_3:
        {
          start_bc_motors_pr();
          if(millis() - t > 3500)
          {
            stop_bc_motors();
            old_ppos = str_pos;
            wrk_state = poworot_3;
          }
          break;
        }
        */
        case WRK_FAIL:
            {
              str_stop();
              stop_bc_motors();
              prg_state = FAIL;
              break;
            }
        }
        break;
    
      }
    case FAIL:
      {
        str_stop();
        stop_bc_motors();
        break;
      }

    case END:
      {
        Serial.println("Work is over");
        break;
      }
  }
}
