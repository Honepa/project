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

void setup()
{
  Serial.begin(9600);
  pinMode(RELE_K4, OUTPUT);
  pinMode(RELE_K3, OUTPUT);
  pinMode(KNOK, INPUT);
  stop_motor();
}
int state = 0;
int R1, R2, R = 0;
int Tt, t, t0, t1, dt;

void loop()
{
  switch (state)
  {
    case INIT:
      state = GO_LEFT;
      t = millis();
      Serial.println("INIT");
      break;

    case GO_LEFT:
      Serial.println("GO_LEFT");
      if (digitalRead(KNOK))
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
      if (!digitalRead(KNOK))
      {
        state = GO_RIGHT2;
      }
      break;

    case GO_RIGHT2:
      Serial.println("GO_RIGHT2");
      go_right();
      if (digitalRead(KNOK))
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
      if (!digitalRead(KNOK))
      {
        state = GO_MIDDLE2;
      }
      break;

    case GO_MIDDLE2:
      Serial.println("GO_MIIDDLE2");
      go_left();
      if (abs(analogRead(A0) - R) < 2)
      {
        state = WORK;
      }
      else if (digitalRead(KNOK) == 1)
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
    Serial.println("WORK");
    stop_motor();
    break;
  }
}
