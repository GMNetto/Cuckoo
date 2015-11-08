#include <avr/pgmspace.h>
#include "cuckoo.h"
#include "dingdong.h"
#include "pendulum.h"


#define led_pin 13
#define speaker_pin 11

volatile uint8_t second_counter = 0;
volatile uint8_t minute_counter = 0;
volatile uint8_t hour_counter = 0;


uint8_t playing_sound = 0;
uint8_t k = 0;
int current_sample = 0;



ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  if(playing_sound == 0){
    current_sample = 0;
    playing_sound = 3;
  }
  second_counter++;;
  if (second_counter == 60) {
    second_counter = 0;
    minute_counter++;

    if (minute_counter == 30) {
      current_sample = 0;
      playing_sound = 2; //plays ding dong
    }

    if (minute_counter == 60) {
      minute_counter = 0;
      hour_counter++;

      k = 0; //clears counters for cuckoo and enables sound
      current_sample = 0;
      playing_sound = 1;

      if (hour_counter == 24) {
        hour_counter = 0;
      }
    }
  }
}

ISR(TIMER0_COMPA_vect)          // timer compare interrupt service routine
{

  if (playing_sound == 1) {
    digitalWrite(led_pin, 1);
    if (current_sample >= cuckoo_length) {
      current_sample = 0;
      //playing_sound = 0;
      digitalWrite(led_pin, 0);
      k++;
    }

    //check when to stop
    if (hour_counter == 0 | hour_counter == 12) {
      if (k == 12) playing_sound = 0;
    }
    else if (k == hour_counter%12) playing_sound = 0;
    OCR2A = pgm_read_byte(&cuckoo_data[current_sample]);

    current_sample++;
  }
  else if (playing_sound == 2) {
    digitalWrite(led_pin, 1);
    if (current_sample >= dingdong_length) {
      current_sample = 0;
      playing_sound = 0;
      digitalWrite(led_pin, 0);
    }
    OCR2A = pgm_read_byte(&dingdong_data[current_sample]); //set output compare to sample value

    current_sample++;
  }
  else if (playing_sound == 3){
    digitalWrite(led_pin, 1);
    if (current_sample >= pendulum_length) {
      current_sample = 0;
      playing_sound = 0;
      digitalWrite(led_pin, 0);
    }
    OCR2A = pgm_read_byte(&pendulum_data[current_sample]); //set output compare to sample value

    current_sample++;
  }

}

void setup_timer0() {
  //set Timer0 to 8khz to change the OCR of timer2
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR0A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR0B |= (1 << WGM01);
  // Set CS21 bit for 8 prescaler
  TCCR0B |= (1 << CS01);
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
}

void setup_timer1() {
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}

void setup_timer2() {

  TCCR2A |= (1 << WGM21) | (1 << WGM20); //set fast PWM
  TCCR2B &= ~_BV(WGM22);


  TCCR2A = (TCCR2A | (1 << COM2A1)) & ~(1 << COM2A0); //non-invert. PWM (goes down after count is over) on pin 11 (speaker)
  TCCR2A &= ~(1 << COM2B1) | (1 << COM2B0); //togle on compare match
  TCCR2B = (TCCR2B & ~((1 << CS12) | (1 << CS11))) | _BV(CS10); //001 - no prescaler (PWM as fast as possible)

  // Set initial pulse width to the first sample.
  OCR2A = pgm_read_byte(&cuckoo_data[0]);
}

void input_time() {
  int time_set = 0;
  char buffer_time[8];
  int hour;
  int minutes;
  int seconds;

  Serial.begin(9600);
  Serial.println("Please enter the current time (format HH:MM:SS):");
  while (time_set == 0) {

    if (Serial.available() > 0) {
      // read the user input
      Serial.readBytes(buffer_time, 8);
      time_set = 1;

      //check user input >48 and <58 for numbers and 58 for : symbol
      if (buffer_time[2] != 58) time_set = 0;
      if (buffer_time[5] != 58) time_set = 0;

      //check if time is valid
      hour = ((buffer_time[0] - 48) * 10) + buffer_time[1] - 48;
      minutes = ((buffer_time[3] - 48) * 10) + buffer_time[4] - 48;
      seconds = ((buffer_time[6] - 48) * 10) + buffer_time[7] - 48;

      if (hour > 23 || hour < 0) time_set = 0;
      if (minutes > 59 || minutes < 0) time_set = 0;
      if (seconds > 59 || seconds < 0) time_set = 0;


      if (time_set == 0) Serial.println("Invalid input. Try again");
      else {
        Serial.print("Time set to: ");
        Serial.print(hour);
        Serial.print(":");
        Serial.print(minutes);
        Serial.print(":");
        Serial.print(seconds);
      }
    }
  }

  hour_counter = hour;
  minute_counter = minutes;
  second_counter = seconds;

  Serial.end();
}


void setup() {
  //disable interrupts
  pinMode(led_pin, OUTPUT);
  pinMode(speaker_pin, OUTPUT);



  input_time(); //uses a timer, so must be set before

  //disable interrupts
  noInterrupts();


  setup_timer0();
  setup_timer1();
  setup_timer2();


  digitalWrite(led_pin, 0);
  interrupts();
  //enable interrrupts
}

void loop() {
  // put your main code here, to run repeatedly:

}
