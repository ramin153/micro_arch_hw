#include <Arduino.h>

#define PWM_OUT 4
#define INPUT_POT A3

static bool toggle = false;
static int count = 0;
void setup() {
  pinMode(INPUT_POT, INPUT);
  Serial.begin(9600);
  pinMode(PWM_OUT, OUTPUT);

  cli();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = 1023;

  TCCR1B |= (1 << WGM12) | ((0<<CS10) |(1<<CS11)|(0<<CS12));
  TIMSK1 |= (OCIE1A << 1) ;

  sei();

}

ISR(TIMER1_COMPA_vect){
  toggle = ! toggle;

  if(toggle){
  OCR1A = count;
  }else{
  OCR1A = 1023 - count;
  }
}


void loop() {
  count = map(analogRead(INPUT_POT),0,1023,1023*0.1,1023*0.9) ;
  //  count = map(analogRead(INPUT_POT),0,1023,0,1023) ;
  digitalWrite(PWM_OUT,toggle);

}