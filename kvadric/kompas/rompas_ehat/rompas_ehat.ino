#include <Wire.h>
#include <math.h>

#define INIT 0
#define WORK 1
#define FAIL 2

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

uint8_t IMUAddress = 0x68;
/* IMU Data */
int16_t accX;
int16_t accY;
int16_t accZ;
int16_t tempRaw;
int16_t gyroX;
int16_t gyroY;
int16_t gyroZ;

int k, k1 = 1;
int koor[6];
double c, dg, S_z, A_z, in_z = 0;
double dt = 0.01;
int n = 10;
int state = 0;
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
int l = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println(1000);
  Wire.begin();
  i2cWrite(0x6B, 0x00);
  pinMode(6, 1);
  pinMode(7, 1);
  pinMode(8, 0);
  t = millis();
}

void loop()
{
  //Serial.println(state);
  read_kompas();
  switch (state)
  {
    case INIT:
      {
        int f = str_init();
        if (f == 1)
        {
          if (millis() - t > 10000)
          {
            c = - in_z / 100;
            state = WORK;
          }
          else
          {
            in_z = in_z + koor[6] * dt;
          }
        }
        else if (f == 2)
        {
          state = FAIL;
        }
        break;
      }
    case WORK:
      {
        in_z = in_z + koor[6] * dt + c;
        S_z = S_z + in_z - A_z;
        A_z = S_z / n;
        dg = -0.079 * A_z - 28.5281;
        if (dg > 2)
        {
          str_right();
          int l = 1;
        }
        else if (dg  < 2)
        {
          str_left();
          int l = 1;
        }
        else
        {
          if (l == 1)
          {
            int j = set_str_pos(0);
            if (j = 1)
            {
              str_stop();

            }
            else if (j == 2 or j == 3)
            {
              state = FAIL;
            }
          }
        }
        break;
      }
    case FAIL:
      {
        break;
      }
  }

}

int read_kompas()
{
  int bytes[14];
  int nbytes = 14;
  Wire.beginTransmission(IMUAddress);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(IMUAddress, nbytes);
  for (uint8_t i = 0; i < nbytes; i++)
  {
    bytes[i] = Wire.read();
  }
  accX = bytes[0];
  accX = accX << 8;
  accX = accX | bytes[1];

  accY = bytes[2];
  accY = accY << 8;
  accY = accY | bytes[3];

  accZ = bytes[4];
  accZ = accZ << 8;
  accZ = accZ | bytes[5];

  tempRaw = bytes[6];
  tempRaw = tempRaw << 8;
  tempRaw = tempRaw | bytes[7];

  gyroX = bytes[8];
  gyroX = gyroX << 8;
  gyroX = gyroX | bytes[9];

  gyroY = bytes[10];
  gyroY = gyroY << 8;
  gyroY = gyroY | bytes[11];

  gyroZ = bytes[12];
  gyroZ = gyroZ << 8;
  gyroZ = gyroZ | bytes[13];

  koor[0] = accX;
  koor[1] = accY;
  koor[2] = accZ;
  koor[3] = tempRaw;
  koor[4] = gyroX;
  koor[5] = gyroY;
  koor[6] = gyroZ;
  return koor;
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
void i2cWrite(uint8_t registerAddress, uint8_t data)
{
  Wire.beginTransmission(IMUAddress);
  Wire.write(registerAddress);
  Wire.write(data);
  Wire.endTransmission(); // Send stop
}

uint8_t* i2cRead(uint8_t registerAddress, uint8_t nbytes)
{
  uint8_t data[nbytes];
  Wire.beginTransmission(IMUAddress);
  Wire.write(registerAddress);
  Wire.endTransmission(false); // Don't release the bus
  Wire.requestFrom(IMUAddress, nbytes); // Send a repeated start and then release the bus after reading
  for (uint8_t i = 0; i < nbytes; i++)
    data [i] = Wire.read();
  return data;
}
