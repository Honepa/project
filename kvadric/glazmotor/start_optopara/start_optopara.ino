void setup() 
{
  pinMode(13, OUTPUT);
  pinMode(12, INPUT);

}

void loop() 
{
  digitalWrite(13, digitalRead(12));
}
