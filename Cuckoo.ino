#include <avr/pgmspace.h>
#include "cuckoo.h"
#include "dingdong.h"


ISR(TIMER2_COMPA_vect){
  //Interrupt action
}

void setup_every_1s(){
  //I still have to discover what is it for. But TCC is a timer for sure.
  TCCR2B = _BV(CS21) || _BV(CS20);

  // set WGM to CTC mode (010)
  // In this mode Timer2 counts up until it matches OCR2A
  TCCR2A = _BV(WGM21);

  //frequency to interrupt.
  OCR2A = 1;

  //when timer 2 matches OCR, it interrupts
  TIMSK2 = _BV(OCIE2A);
}

void setup() {
  //disable interrupts
  cli();
  setup_every_1s();

  //enable interrrupts
  sei();
}

void loop() {
  // put your main code here, to run repeatedly:

}
