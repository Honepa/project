#define PWR_OK 3
#define PWR_ON 2
#define CMP_ON 4

#define not_used A0

void setup() 
{
 pinMode(PWR_OK, OUTPUT); 
 pinMode(CMP_ON, OUTPUT);
 pinMode(PWR_ON, OUTPUT);
 pinMode(not_used, OUTPUT);
}

void loop() 
{
 digitalWrite(3, 1);
 delay(150);
 digitalWrite(3, 0);
 digitalWrite(2, 1);
 delay(150);
 digitalWrite(2, 0);
 digitalWrite(4, 1);
 delay(150);
 digitalWrite(4, 0);
}
