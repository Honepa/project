void setup() 
{
  Serial.begin(9600);

}

byte b = 0;

void loop() 
{
 if(Serial.available > 0)
 {
  b = Serial.read();
  Serial.println(b);
 }

}
