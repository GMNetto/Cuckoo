#include <avr/pgmspace.h>
#include "cuckoo.h"
#include "dingdong.h"
#include "PCM.h"
/*
const int led_pin = 13;
int state = 0;
int counter = 0;
int minute_counter = 60;

ISR(TIMER1_COMPA_vect){
  //Interrupt action
  state = 1 - state;
  digitalWrite(led_pin, state);
  counter++;
  Serial.println(counter);
  if(counter == 10){
    int i;
    for(i = 0; i < dingdong_length; i++){
      tone(11, 8000, cuckoo_data[i]);
    }
    counter = 0;
    Serial.println(state);
    noTone(11);
  }
}

void play_sound(){
  int speakerPin = 11;
  pinMode(speakerPin, OUTPUT);
  
  // Set up Timer 2 to do pulse width modulation on the speaker
  // pin.
  
  // Use internal clock (datasheet p.160)
  ASSR &= ~(_BV(EXCLK) | _BV(AS2));
  
  // Set fast PWM mode  (p.157)
  TCCR2A |= _BV(WGM21) | _BV(WGM20);
  TCCR2B &= ~_BV(WGM22);
  
  // Do non-inverting PWM on pin OC2A (p.155)
  // On the Arduino this is pin 11.
  TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
  TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));
  
  // No prescaler (p.158)
  TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
  
  // Set initial pulse width to the first sample.
  OCR2A = pgm_read_byte(&cuckoo_data[0]);

}

void setup_every_1s(){
  //Timers to 0
  TCCR1A = 0;
  TCCR1B = 0;

  //Acummulator timer
  TCNT1 = 0;

  // compare match register 16MHz/256/2Hz
  OCR1A = 62500;
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt            
}
*/


void setup() {
  //disable interrupts
  /*pinMode(led_pin, OUTPUT);
  noInterrupts();
  setup_every_1s();
  
  
  //enable interrrupts
  interrupts();*/
  startPlayback(cuckoo_data, sizeof(cuckoo_data));
  Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
