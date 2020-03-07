void setup() 
{
  Serial.begin(9600);

}

byte b = 0;

void loop() 
{
  b = 2;
  Serial.print('1');
  //b = 0;
 
  /*
 if(Serial.available > 0)
 {
  b = Serial.read();
  Serial.println(b);
 }
 */
}
