#define motor1_A     4
#define motor1_B     5
#define motor_br_pwm 3

#define motor2_A     11
#define motor2_B     12
#define motor_bl_pwm 13

#define INIT 0
#define RUN1  1
#define RUN2 2
#define RUN3 3
#define STOP 4
#define FAIL 5

uint8_t SPDB = 0;

volatile uint8_t wheel_bl;
volatile uint8_t wheel_br;
volatile uint8_t DK_BR = 0;
volatile uint8_t DK_BL = 0;

volatile uint8_t bl_old, br_old = 0;

int8_t k = 40;
int n = 20;
float S_br, A_br, S_bl, A_bl = 0;
float SPDBR, SPDBL = 0;
int pwm_br, pwm_bl = 0;

int state = 0;

int t, t0 = 0;

float e, dt, Ie = 0;

void start_bc_motors_pr() //вперёд
{
  digitalWrite(motor1_A, 1);
  digitalWrite(motor1_B, 0);

  digitalWrite(motor2_A, 1);
  digitalWrite(motor2_B, 0);
  /* for (int i = 0; i < 256; i++)
    {
     analogWrite(motor_br_pwm, i);
     analogWrite(motor_bl_pwm, i);
     delay(100);
    } */
  delay(10000);
  for (int i = 0; i < 256; i++)
  {
    analogWrite(motor_br_pwm, 255 - i);
    analogWrite(motor_bl_pwm, 255 - i);
    delay(100);
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
    analogWrite(motor_br_pwm, i);
    analogWrite(motor_bl_pwm, i);
  }
}

void stop_bc_motors() // стоп
{
  for (int i = 255; i >= 0; i--)
  {
    analogWrite(motor_br_pwm, i);
    analogWrite(motor_bl_pwm, i);
    delay(20);
  }
  digitalWrite(motor1_A, 0);
  digitalWrite(motor1_B, 0);

  digitalWrite(motor2_A, 0);
  digitalWrite(motor2_B, 0);
}

void wheel_BR()
{
  DK_BR++;
}

void wheel_BL()
{
  DK_BL++;
}


ISR (TIMER4_COMPA_vect)
{
  TCNT4  = 0;
  int bl = analogRead(A1);
  if ((bl >= 400) and (bl_old == 0))
  {
    DK_BL++;
    bl_old = 1;
  }
  if ((bl <  400) and (bl_old == 1))
  {
    DK_BL++;
    bl_old = 0;
  }
  int br = analogRead(A2);
  if ((br >= 400) and (br_old == 0))
  {
    DK_BR++;
    br_old = 1;
  }
  if ((br <  400) and (br_old == 1))
  {
    DK_BR++;
    br_old = 0;
  }
}


ISR (TIMER1_COMPA_vect)
{
  wheel_br = DK_BR;
  wheel_bl = DK_BL;
  DK_BR = 0;
  DK_BL = 0;

  n = 20;

  S_br = S_br + wheel_br - A_br;
  A_br = S_br / n;

  S_bl = S_bl + wheel_bl - A_bl;
  A_bl = S_bl / n;

  SPDBR = A_br / 0.230;
  SPDBL = A_bl / 0.230;

  pwm_br = k * (SPDB - SPDBR);
  pwm_bl = k * (SPDB - SPDBL);

  k = 1;
  e = SPDB - A_br;
  dt = 1;
  float k1 = 1.25;
  Ie += e * dt;
  pwm_br = max(min(k * e + k1 * Ie, 255), 0);
  analogWrite(motor_br_pwm, pwm_br);

  //analogWrite(motor_br_pwm, pwm_br);
  //analogWrite(motor_bl_pwm, pwm_bl);

  //Serial.print(wheel_bl);
  //Serial.print("\t");
  //Serial.print(wheel_br);
  //Serial.print("\t");
  //Serial.print(A_bl);
  //Serial.print("\t");
  Serial.print(SPDB);
  Serial.print("\t");
  Serial.print(A_br);
  Serial.print(" \t");
  Serial.print(e);
  Serial.print("\t");
  Serial.print(pwm_br / 10);



  //Serial.print(pwm_bl);
  Serial.print(" \n");
  //digitalWrite(13, ! digitalRead(13));

  digitalWrite(motor1_A, 1);
  digitalWrite(motor1_B, 0);

}
void setup()
{
  // настройка таймера 1
  TCCR1A = 0;
  //TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 3125;            // compare match register 16MHz/256/2Hz
  //OCR1B = 3125;            // compare match register 16MHz/256/2Hz
  //TCCR1B |= (1 << WGM12);   // CTC mode
  //TCCR1B |= (1 << CS10);    // 1024 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  //настройка таймера 4
  TCCR4A = 0;
  TCNT4  = 0;

  OCR4A = 312;
  TIMSK4 |= (1 << OCIE4A);

  Serial.begin(115200);

  attachInterrupt(5, wheel_BL, RISING);
  attachInterrupt(4, wheel_BR, RISING);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  sei();

  t = millis();
}

void loop()
{

  switch (state)
  {
    case INIT:
      {
        if (millis() - t > 1500)
        {
          t = millis();
          state = RUN1;
        }
        break;
      }

    case RUN1:
      {
        SPDB = 6;

        if (millis() - t > 20000)
        {
          t = millis();
          state = RUN2;
        }
        break;
      }
    case RUN2:
      {
        SPDB = 8;
        if (millis() - t > 20000)
        {

          t = millis();
          state = RUN3;
        }
        break;
      }
    case RUN3:
      {
        SPDB = 2;
        if (millis() - t > 20000)
        {

          t = millis();
          state = STOP;
        }
        break;
      }

    case STOP:
      {
        SPDB = 0;
        break;
      }

    case FAIL:
      {
        break;
      }
  }
}
