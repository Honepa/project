#define s_print Serial.print

void s_println(int n_args, ...)
{
  va_list ap;
  va_start(ap, n_args);
  for (int i = 1; i < n_args; i++)
  {
    s_print(va_arg(ap, int));
    s_print(", ");
  }
  s_print(va_arg(ap, int));
  s_print("\n");
  va_end(ap);
}


void setup()
{
  pinMode( 3, 1);
  pinMode( 2, 1);
  pinMode(10, 1);
  pinMode(13, 1);
  pinMode( 8, 2);
  Serial.begin(9600);
  digitalWrite( 3, 1);
  digitalWrite( 2, 0);
  digitalWrite(10, 0);
  s_print("t0, t1, e, v, a\n");
  delay(100);
}

uint32_t t0 = 0;
uint32_t t1 = 1000;
uint32_t t2 = 0;
float v0 = 1;
uint8_t st = 1;
float k1 = 5;
float k2 = 5;
float ie = 0;

void loop()
{
  digitalWrite(13, digitalRead( 8));
  if (digitalRead( 8) != st)
  {
    st = digitalRead( 8);
    t1 = millis() - t0;
    t0 = millis();
  }
  if (millis() - t2 > 1000)
  {
    t2 = millis();
    float v = 1.0 / t1;
    float e = v0 - v;
    ie += e;
    uint8_t a = min(255, max(0, k1 * e + k2 * ie));
    analogWrite(10, a);
  }
}
