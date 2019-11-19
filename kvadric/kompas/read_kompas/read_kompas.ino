#include <Wire.h>
#include <math.h>

uint8_t IMUAddress = 0x68;
/* IMU Data */
int16_t accX;
int16_t accY;
int16_t accZ;
int16_t tempRaw;
int16_t gyroX;
int16_t gyroY;
int16_t gyroZ;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  i2cWrite(0x6B, 0x00);
}
long t = 0;
int koor[6];

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
  /*
  Serial.print(" ");
  Serial.print("accX:");
  Serial.print(accX);
  Serial.print(" ");
  Serial.print("accY:");
  Serial.print(accY);
  Serial.print(" ");
  Serial.print("accZ:");
  Serial.print(accZ);
  Serial.print(" ");
  Serial.print("tempRaw");
  Serial.print(tempRaw);
  Serial.print(" ");
  Serial.print("X:");
  Serial.print(gyroX);
  Serial.print(" ");
  Serial.print("Y:");
  Serial.print(gyroY);
  Serial.print(" ");
  Serial.print("Z:");
  Serial.print(gyroZ);
  Serial.println(" ");
  */
  return koor;
}


void loop()
{
  read_kompas();
  /*for (int i = 0; i < 7; i++)
  {
    Serial.print(koor[i]);
    Serial.print("  ");
  }
  Serial.println(" ");
   */
  double accYangle = (atan2(koor[0],koor[2])+PI)*RAD_TO_DEG;
  double accXangle = (atan2(koor[1],koor[2])+PI)*RAD_TO_DEG;  
  double gyroXrate = (double)koor[4]/131.0;
  double gyroYrate = -((double)koor[5]/131.0);
  Serial.print("x:");
  Serial.print(" ");
  Serial.print(gyroXrate);
  Serial.print("  ");
  Serial.print("y:");
  Serial.print(" ");
  Serial.print(gyroYrate);
  Serial.println(" ");
  delay(500);
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
