#include <Wire.h>
#include <math.h>

#define INIT_KOMPAS 0
#define INIT        1
#define WORK        2
#define STOP        3
#define FAIL        4

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
long t0 = 0;
int koor[6];
double c, dg, DG, e, A, S_z, A_z, in_z, A_Z_0, k2 = 0;
//double dt = 0.0016;
double dt = 0.01;
int n = 10;
int is_limit_left, is_limit_right, R1, R2, str_pos, old_str_pos = 0;
int str_state = 0;
int state = INIT_KOMPAS;
long t = 0;
double micr, micr_0 = 0;

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

  Wire.begin();
  i2cWrite(0x6B, 0x00);
  Serial.print("in_z");
  Serial.print(" ");
  Serial.print("c");
  Serial.print(" ");
  Serial.print("A_z");
  Serial.print(" ");
  Serial.print("dg");
  Serial.println(" ");
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

float d_in_z, iiii = 0;

void loop()
{
  int sturn = 0;
  read_kompas();
  is_limit_left = limit_left();
  is_limit_right = limit_right();
  str_pos = get_str_pos() - R1 - R2 / 2;
  switch (state)
  {
    case INIT_KOMPAS:
      {
        if (millis() - t > 10000)
        {
          c = -in_z / 100;
          state = INIT;
        }
        else
        {
          in_z = in_z + koor[6] * dt;
        }
        break;
      }
    case INIT:
      {
        sturn = str_init();
        if (sturn = 1)
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
        in_z = in_z + koor[6] * dt + c;
        S_z = S_z + in_z - A_z;
        A_z = S_z / n;
        dg = -0.079 * A_z - 28.5281;
        k2 = 1;
        e = DG - dg;
        A = k2 * e;
        //Serial.print(" ");
        Serial.print(in_z);
        Serial.print(" ");
        Serial.print(c);
        Serial.print(" ");
        Serial.print(A_z);
        Serial.print(" ");
        Serial.print(dg);
        Serial.print(" ");
        Serial.print(koor[6]);
        Serial.print(" ");
        Serial.print(A);
        Serial.println(" ");
        break;
        if (str_pos > A - 20)
        {
          Serial.println("left");
        }
        else if (str_pos < A + 20)
        {
          Serial.println("right");
        }
        else
        {
          Serial.println("stop");
        }
        A_Z_0 = A_z;
        break;
      }
    case FAIL:
    {
      str_stop();
      break;
    }
  }

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
