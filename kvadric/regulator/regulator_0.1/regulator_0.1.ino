#include <Stepper.h>

#define motor1_A   4
#define motor1_B   5
#define motor1_pwm 3

#define motor2_A   11
#define motor2_B   12
#define motor2_pwm 13

void start_bc_motors_pr() //вперёд
{
  digitalWrite(motor1_A, 1);
  digitalWrite(motor1_B, 0);

  digitalWrite(motor2_A, 1);
  digitalWrite(motor2_B, 0);
  for (int i = 0; i < 256; i++)
  {
    //    analogWrite(motor1_pwm, i);
    analogWrite(motor2_pwm, i);
    delay(20);
  }
  delay(10000);
  for (int i = 0; i < 256; i++)
  {
    //    analogWrite(motor1_pwm, 255 - i);
    analogWrite(motor2_pwm, 255 - i);
    delay(20);
  }
}

void start_bc_motors_bk() //назад
{
  digitalWrite(motor1_A, 0);
  digitalWrite(motor1_B, 1);

  digitalWrite(motor2_A, 0);
  digitalWrite(motor2_B, 1);

  for (int i = 0; i < 256; i++)
  {
    analogWrite(motor1_pwm, i);
    analogWrite(motor2_pwm, i);
  }
}
void stop_bc_motors() // стоп
{
  for (int i = 255; i >= 0; i--)
  {
    analogWrite(motor1_pwm, i);
    analogWrite(motor2_pwm, i);
    delay(20);
  }
  digitalWrite(motor1_A, 0);
  digitalWrite(motor1_B, 0);

  digitalWrite(motor2_A, 0);
  digitalWrite(motor2_B, 0);
}

volatile uint8_t wheel_bl;
volatile uint8_t wheel_br;
volatile uint8_t DK_BR = 0;
volatile uint8_t DK_BL = 0;
/*

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 3125;            // compare match register 16MHz/256/2Hz
  // OCR1A = 3125;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS10);    // 1024 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  ISR(TIMER1_COMPA_vect)
  {
  TCNT1 = 0;
  handsWork();
  // legsWork();
  }
*/
void wheel_BR()
{
  DK_BR++;
}
void wheel_BL()
{
  DK_BL++;
}
volatile uint8_t bl, br, bl_old, br_old = 0;

ISR (TIMER4_COMPA_vect)
{
  bl = analogRead(A1);
  if (bl < bl_old)
  { 
    DK_BL++;
  }
  else
  {
    bl_old = bl;
  }
}

ISR(TIMER5_COMPA_vect)
{  
  br = analogRead(A2);
  if (br < br_old)
  {
    DK_BR++;
  }
  else
  {
    br_old = br;
  }
}

ISR (TIMER1_COMPA_vect)
{
  wheel_br = DK_BR;
  wheel_bl = DK_BL;
  DK_BR = 0;
  DK_BL = 0;
  Serial.print(wheel_bl);
  Serial.print("\t");
  Serial.println(wheel_br);
  /*int dt =777
    SPDBR = DK_BR / dt;
    SPDBL = DK_BL / dt;
    DK_BR = 0;
    DK_BL = 0;
    pwm1 = k(spd
  */
  //digitalWrite(13, ! digitalRead(13));
}
void setup()
{
  // настройка таймера 1
  TCCR1A = 0;
  //TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 3125;            // compare match register 16MHz/256/2Hz
  //OCR1B = 3125;            // compare match register 16MHz/256/2Hz
  //TCCR1B |= (1 << WGM12);   // CTC mode
  //TCCR1B |= (1 << CS10);    // 1024 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  //настройка таймера 4
  TCCR4A = 0;
  TCNT4 =0;

  OCR4A = 312;
  TIMSK4 |= (1 << OCIE4A);
  
  //настройка таймера 6
  TCCR5A = 0;
  TCNT5 =0;

  OCR5A = 312;
  TIMSK5 |= (1 << OCIE5A);
  Serial.begin(9600);

  attachInterrupt(5, wheel_BL, RISING);
  attachInterrupt(4, wheel_BR, RISING);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  sei();
}

void loop()
{
  start_bc_motors_pr();
}
