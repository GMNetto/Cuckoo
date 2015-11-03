#include <avr/pgmspace.h>
#include "cuckoo.h"
#include "dingdong.h"

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

void setup() {
  //disable interrupts
  pinMode(led_pin, OUTPUT);
  noInterrupts();
  setup_every_1s();
  

  //enable interrrupts
  interrupts();
  Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
