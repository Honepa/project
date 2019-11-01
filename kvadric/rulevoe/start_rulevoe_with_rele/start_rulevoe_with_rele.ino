#define motor1_A 4
#define motor1_B 5
#define motor1_pwm 3

#define motor2_A 12
#define motor2_B 13
#define motor2_pwm 11

#define RELE_K3 6
#define RELE_K4 7
#define KNOK 8

#define INIT 0
#define GO_LEFT 1
#define SAVE_R1 2
#define GO_RIGHT1 3
#define GO_RIGHT2 4
#define SAVE_R2 5
#define GO_MIDDLE1 6
#define GO_MIDDLE2 7
#define FAIL 8
#define WORK 9

#define TRON   0
#define EHAT_1 1
#define POWOROT 2
#define PRYAMO 3
#define EHAT_2 4
#define STOP 5

void s_println(int n_args, ...)
{
  va_list ap;
  va_start(ap, n_args);
  for (int i = 1; i < n_args; i++)
  {
    Serial.print(va_arg(ap, int));
    Serial.print(", ");
  }
  Serial.print(va_arg(ap, int));
  Serial.print("\n");
  va_end(ap);
}
  
int stop_motor()
{
  digitalWrite(RELE_K4, 1);
  digitalWrite(RELE_K3, 1);
}

int go_left()
{
  digitalWrite(RELE_K4, 0);
  digitalWrite(RELE_K3, 1);
}

int go_right()
{
  digitalWrite(RELE_K4, 1);
  digitalWrite(RELE_K3, 0);
}

int knokRead()
{
  int k = 0;
  for (int j = 0; j < 5000; j++) k += digitalRead(KNOK);
  return k > 4900;
}

void setup()
{
  Serial.begin(9600);
  pinMode(RELE_K4, OUTPUT);
  pinMode(RELE_K3, OUTPUT);
  pinMode(KNOK, INPUT);
  stop_motor();

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
}
int state = 0;
int work_state = 0;
int R1, R2, R, my_R = 0;
int Tt, t, t0, t1, dt;

void loop()
{
  s_println(4, R1, R2, R, analogRead(A0));
  switch (state)
  {
    case INIT:
      state = GO_LEFT;
      t = millis();
      Serial.println("INIT");
      break;

    case GO_LEFT:
      Serial.println("GO_LEFT");
      if (knokRead())
      {
        state = SAVE_R1;
      }
      else
      {
        go_left();
      }
      break;

    case SAVE_R1:
      Serial.println("SEVE_R1");
      R1 = analogRead(A0);
      stop_motor();
      state = GO_RIGHT1;
      break;

    case GO_RIGHT1:
      Serial.println("GO_RIGHT1");
      go_right();
      if (!knokRead())
      {
        state = GO_RIGHT2;
      }
      break;

    case GO_RIGHT2:
      Serial.println("GO_RIGHT2");
      go_right();
      if (knokRead())
      {

        state = SAVE_R2;
      }
      break;

    case SAVE_R2:
      Serial.println("SAVE_R2");
      R2 = analogRead(A0);
      t0 = millis();
      dt = t - t0;
      Tt = (dt / 100) * 10;
      R = (R1 + R2) / 2;
      stop_motor();
      state = GO_MIDDLE1;
      break;

    case GO_MIDDLE1:
      Serial.println("GO_MIDDLE1");
      go_left();
      if (!knokRead())
      {
        state = GO_MIDDLE2;
      }
      break;

    case GO_MIDDLE2:
      
      go_left();

      if (abs(analogRead(A0) - R) < 2)
      {
        state = WORK;
      }
      else if (knokRead())
      {
        state = FAIL;
      }
      else if ((millis() - t0) > dt + Tt)
      {
        state = FAIL;
      }
      break;

    case FAIL:
      Serial.println("FAIL");
      stop_motor();
      break;

    case WORK:
      Serial.println(work_state);
      switch (work_state)
      {
        case TRON:
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
          t0 = millis();
          work_state = EHAT_1;
          break;

        case EHAT_1:
          if (millis() - t0  >= 2000)
          {
            work_state = POWOROT;
          }
          break;

        case POWOROT:
          my_R = (R2 + R) / 2;
          go_right();
          if (abs(analogRead(A0) - my_R) < 2)
          {
            work_state = PRYAMO;
          }
          break;

        case PRYAMO:
          if (abs(analogRead(A0) - R) < 2)
          {
            work_state = EHAT_2;
            t0 = millis();
          }

          break;

        case EHAT_2:
          if (millis() - t0 >= 3000)
          {
            work_state = STOP;
          }
          break;

        case STOP:
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
          break;
      }
      break;
  }
}
