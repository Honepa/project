void setup() 
{
  Serial.begin(9600);

}

void loop() 
{
  
  byte b = Serial.read();
  if (b == 49)
  {
    Serial.println("11");
  }
  else
  {
    Serial.println("no");
  }
}
