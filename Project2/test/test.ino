/*--------------------------------------------------------------------
Name:   Matthew Schlatter
Date:   4/11/24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   test.ino
HW/Lab: Project 2, Measure IR Pulses

Purp: Uses counters and interrupts to measure IR Pulses for a 
    remote. 

Doc:  

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/
#include <avr/io.h>
#include <util/delay.h>
#include "ir_decoder.h"
#include "motors.h"
#include "ultrasonic.h"

#define SAMPLE_SIZE 34
#define IR_DECODER_PIN (PIND & (1 << PIND2)) // (i.e. Digital Pin 2)
#define IR_PIN (PIND2)
#define TRUE 1
#define FALSE 0

volatile uint8_t  newIrPacket = FALSE;
uint16_t time1[SAMPLE_SIZE], time0[SAMPLE_SIZE];
uint8_t  i;
volatile uint32_t irPacket;

void loop() {
  
    Serial.println("Waiting for IR input");

    while(IR_DECODER_PIN != 0); // IR input is nominally logic 1
    TCNT1 = 0; // reset timer  
    newIrPacket = FALSE;
    TIFR1 |= (1 << OCF1A); // clear Timer/Counter1, Output Compare A Match Flag by writing 1 
    TIMSK1 |= (1 << OCIE1A); // enable interrupt on match with OCR1A

    for(i=0; i<SAMPLE_SIZE; i++) {

      TCNT1 = 0; // reset timer and 
      while(IR_DECODER_PIN == 0 && newIrPacket == FALSE);   // wait while IR is logic 0
      time0[i] = TCNT1; // and store timer 1 count 

      TCNT1 = 0; // reset timer and
      while(IR_DECODER_PIN != 0 && newIrPacket == FALSE); // wait while IR is logic 1
      time1[i] = TCNT1; // and store timer 1 count 

    }

    Serial.println("Time Logic 1");
    for(i=0; i<SAMPLE_SIZE; i++) {
        Serial.print("time1["); Serial.print(i,DEC); Serial.print("] = ");Serial.println(time1[i],DEC);
        if(time1[i] >= 250 && time1[i] < 550) {
          irPacket = (irPacket << 1);
        } else if (time1[i] < 250 && time1[i] > 0) {
          irPacket = (irPacket << 1) | 0x1;
        } else {
          irPacket = irPacket;
        }
    }        
    Serial.println("Time Logic 0");
    for(i=0; i<SAMPLE_SIZE; i++) {
        Serial.print("time0["); Serial.print(i,DEC); Serial.print("] = ");Serial.println(time0[i],DEC);
    }        

    irPacket = ~irPacket;
    Serial.println(irPacket, HEX);

    /* Do something with new IR packet */  
    if (irPacket == Green_Button){    
      robotForward(200, 200);
      Serial.println("Green_Button");
      irPacket = 0;     // Clear irPacket
    } else if (irPacket == Red_Button) {
      robotStop();
      Serial.println("Red_Button");
      irPacket = 0;     // Clear irPacket
    } else if (irPacket == Blue_Button) {
      robotBackwards(200, 200);
      Serial.println("Blue_Button");
      irPacket = 0;     // Clear irPacket
    } else if (irPacket == Music1_Button) {
      turnLeft(100, 100);
      _delay_ms(400);
      robotStop();
      Serial.println("Music1_Button");
      irPacket = 0;     // Clear irPacket
    } else if (irPacket == Music4_Button) {
      turnRight(100, 100);
      _delay_ms(400);
      robotStop();
      Serial.println("Music4_Button");
      irPacket = 0;     // Clear irPacket
    }
}

void setup() {
  /* Turn off WDT */ 
  WDTCSR = 0x00; 
  
  Serial.begin(9600);
  Serial.println("Starting up.");

  //Init Timer 1
  TCCR1A = 0; // Reset TCCR1A & TCCR1B to defaults
  TCCR1B = 0; 
  TCCR1A |= (1 << WGM12); // set Timer 1 to CTC with OCR1A as top
  TCCR1B |= (1 << CS11) | (1 << CS10); // set clock prescaler to clk/64
  OCR1A = 0x3F7A; // configure OCR1A (the counter top) to generate an interrupt every 65ms.
  TCNT1H = 0; // clear timer 1 counter register high byte first
  TCNT1L = 0;

  DDRD &= ~(1 << IR_PIN); // input IR on Port D, Pin 2
  PORTD |= (1 << IR_PIN); // enable Pullup on Port D, Pin 2
}

/**
 * This function is called whenever the timer 1 output compare match OCR1A
 * is generated.
 **/
ISR(TIMER1_COMPA_vect){
  TIMSK1 &= ~(OCIE1A); // disable interrupt on match with OCR1A
  newIrPacket = TRUE;
}
