#include <Wire.h>
#include <math.h>

#define INIT 0
#define WORK 1

uint8_t IMUAddress = 0x68;
/* IMU Data */
int16_t accX;
int16_t accY;
int16_t accZ;
int16_t tempRaw;
int k = 1;
int k1 = 1;
int16_t gyroX;
int16_t gyroY;
int16_t gyroZ;

long t = 0;
int koor[6];
double S_z, A_z = 0;
double in_z = 0;
double dt = 0.01;
int n = 10;

double c = 0;
double dg = 0;
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  i2cWrite(0x6B, 0x00);
  t = millis();
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

double A_z_0 = 0;
int state = 0;

void loop()
{
  read_kompas();
  switch (state)
  {
    case INIT:
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
        break;
      }
    case WORK:
      {
        in_z = in_z + koor[6] * dt + c;
        S_z = S_z + in_z - A_z;
        A_z = S_z / n;
        dg = -0.079 * A_z - 28.5281;
        Serial.print(in_z);
        Serial.print(" ");
        Serial.print(c);
        Serial.print(" ");
        Serial.print(A_z);
        Serial.print(" ");
        Serial.print(dg);
        Serial.println(" ");
        A_z_0 = A_z;
        break;
      }
  }
  delay(100);
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
