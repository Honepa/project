#include <Mouse.h>

#include <Stepper.h>

#define motor1_A   4
#define motor1_B   5
#define motor1_pwm 3

#define motor2_A   12
#define motor2_B   13
#define motor2_pwm 11

void start_bc_motors_pr() //вперёд
{
  digitalWrite(motor1_A, 1);
  digitalWrite(motor1_B, 0);

  digitalWrite(motor2_A, 1);
  digitalWrite(motor2_B, 0);
  for (int i = 0; i < 256; i++)
  {
    analogWrite(motor1_pwm, i);
    analogWrite(motor2_pwm, i);
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

volotaile uint8_t wheel_bl
volotaile uint8_t wheel_br

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
ISR (WHEEL_BR)
{
  DK_BR++;
}
ISR (WHEEL_BR)
{
  DK_BL++;
}
ISR (TIMER)
{
  SPD1 = DK_BR / dt;
  SPD1 = DK_BL / dt;
  DK_BR = 0;
  DK_BL = 0;
  
}
void setup()
{
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 3125;            // compare match register 16MHz/256/2Hz
  // OCR1A = 3125;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS10);    // 1024 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  Serial.begin(9600);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);

  attachInterrupt(0, blink, RISING);
}

void loop()
{
  digitalWrite(pin, state);
}
