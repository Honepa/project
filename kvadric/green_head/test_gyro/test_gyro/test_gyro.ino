#include <Wire.h>
#include <ITG3200.h>
ITG3200 gyro = ITG3200();

#define INIT      0
#define GO_LEFT   1
#define STR_LEFT  2
#define GO_RIGHT  3
#define STR_RIGHT 4
#define GO_MID    5
#define WORK      6
#define FAIL      7

float x, y, z = 0;
int ix, iy, iz = 0;
int state = 0;
long t, t0 = 0;
float max_x, min_x, mid_x = 0;
void setup() 
{
 Serial.begin(9600);
 Wire.begin();
 delay(1000);
 gyro.init(ITG3200_ADDR_AD0_LOW);

 Serial.print("ZroCalibruting///");
 gyro.zeroCalibrate(2500, 2);
 Serial.println("done");
}

void loop() 
{
  switch(state)
  {
    case(INIT):
    {
      Serial.print("Start init");
      state = GO_LEFT;
      t = millis();
      break;
    }
    
    case(GO_LEFT):
    {
      go_left_x();
      if (millis - t > 100)
      {
        state = STR_LEFT;
      }
      break;
    }
    
    case(STR_LEFT):
    {
       gyro.readGyro(&x,&y, &z);
       Serial.print("X:");
       Serial.println(x);
      break;
    }
    
    case(GO_RIGHT):
    {
      break;
    }
    
    case(STR_RIGHT):
    {
      break;
    }
    
    case(GO_MID):
    {
      break;
    }
    
    case(WORK):
    {
      break;
    }
    
    case(FAIL):
    {
      break;
    }
  }
 /*
 while (gyro.isRawDataReady())
 {
  gyro.readGyro(&x,&y, &z);
  Serial.print("X:");
  Serial.print(x);
  Serial.print("  Y:");
  Serial.print(y);
  Serial.print("  Z:");
  Serial.println(z);
  delay(100);
 }
 */
}
void go_left_x()
{
  
}
void go_right_x()
{
  
}
void stop_x()
{
  
}
