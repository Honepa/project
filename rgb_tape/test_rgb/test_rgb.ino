#define green_pin 3
#define red_pin   5
#define blue_pin  6

#define fade 10
void setup() 
{
  pinMode(green_pin, OUTPUT);
  pinMode(red_pin,   OUTPUT);
  pinMode(blue_pin,  OUTPUT);

}

void loop() 
{
 

}

void day_theme()
{
  int r, g, b;
  
  for(r = 0; r < 256; r++)
 {
  analogWrite(red_pin, r);
 }
 for(g = 0; g < 256; g++)
 {
  analogWrite(green_pin, g);
 }
 for(b = 0; b < 256; b++)
 {
  analogWrite(blue_pin, b);
 }
}

void night_theme()
{
  int r, g, b;
  
  for (r = 0; r < 256; r++) 
  { 
    analogWrite(red_pin, r);
    delay(fade);
  } 
  
  for (b = 255; b > 0; b--) 
  { 
    analogWrite(blue_pin, b);
    delay(fade);
  } 
  
  for (g = 0; g < 256; g++) 
  { 
    analogWrite(green_pin, g);
    delay(fade);
  } 
  
  for (r = 255; r > 0; r--) 
  { 
    analogWrite(red_pin, r);
    delay(fade);
  } 
  
  for (b = 0; b < 256; b++) 
  { 
    analogWrite(blue_pin, b);
    delay(fade);
  } 
  
  for (g = 255; g > 0; g--) 
  { 
    analogWrite(green_pin, g);
    delay(fade);
  }
}
