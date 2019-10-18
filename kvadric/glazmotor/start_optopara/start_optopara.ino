void setup() 
{
  pinMode(13, OUTPUT);
  pinMode(8, INPUT_PULLUP);

}

void loop() 
{
  if (digitalRead(8)) digitalWrite (13, 1);
  else digitalWrite(13, 0);
}
