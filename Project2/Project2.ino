/*--------------------------------------------------------------------
Name:   Matthew Schlatter
Date:   4/11/24  
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   project2.ino
HW/Lab: Project 2, Decoding an IR Packet

Purp: Uses counters and interrupts to decode an IR packet for a 
    remote. 

Doc:  Class resources

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

#define OCR2B_PWM PD3
#define TRIG_PIN A1
#define ECHO_PIN A0
#define SAMPLE_SIZE       34
#define   BIT0            0x00000001
volatile uint32_t irPacket;
volatile uint8_t  newIrPacket = FALSE;
volatile uint16_t packetData[34];
volatile uint8_t  packetIndex = 0;
uint8_t i = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting up...");
  //irDecoderSetup();
  sensorSetup();
  motorSetup();

  DDRD |= (1 << DDD3); // set OCR2B as output for servo

  //setup timer 2 for servo
  TCCR2A = ((1 << COM2B1) | (1 << WGM21) | (1 << WGM20)); // Fast PWM, non-inverting, TOP at OCR2A
  TCCR2B = ((1 << WGM22) | (1 << CS22) | (1 << CS21) | (1 << CS20)); //1 024 prescaler
  TCNT2 = 0;
  OCR2A = 255; //16ms pulse width, max for timer 2
  OCR2B = 25; //middle to start
  
  //sei();
  Serial.println("Setup complete.");

  //wall is to the left
  OCR2B = 38; // OCR2B = 38 for wall on the left, 13 for right, 25 for front
  // startup motors for wall following
  robotForward(130, 130);
  _delay_ms(500);
}

/*
 * main loop
 */
void loop() {
  int ultDistanceSide = checkDistance();
  OCR2B = 25;
  _delay_ms(300);
  int ultDistanceFront = checkDistance();

  wallFollowing(ultDistanceSide, ultDistanceFront);

  OCR2B = 38;
  _delay_ms(300);

//  if (newIrPacket == TRUE){
//    //code to convert packetData[] counts to 32 bit irpacket
//    irPacket = convertData(packetData);
//    Serial.println(irPacket, HEX);
//    newIrPacket = FALSE;
//  }

//  moveRobotFromIr(irPacket);
//
//  if(irPacket == LightGreen_Button || irPacket == Music1_Button) {
//    _delay_ms(2000);
//  } else if(irPacket == LightBlue_Button || irPacket == Music4_Button) {
//    _delay_ms(2000);
//  }
  
//  irPacket = 0;

}

// -----------------------------------------------------------------------
// Since the IR decoder is connected to INT0, we want an interrupt
// to occur every time that the pin changes - this will occur on
// a positive edge and a negative edge.
//
// Negative Edge:
// The negative edge is associated with end of the logic 1 half-bit and
// the start of the logic 0 half of the bit.  The timer contains the
// duration of the logic 1 pulse, so we'll pull that out, process it
// and store the bit in the global irPacket variable. Going forward there
// is really nothing interesting that happens in this period, because all
// the logic 0 half-bits have the same period.  So we will turn off
// the timer interrupts and wait for the next (positive) edge on INT0
//
// Positive Edge:
// The positive edge is associated with the end of the logic 0 half-bit
// and the start of the logic 1 half-bit.  There is nothing to do in
// terms of the logic 0 half bit because it does not encode any useful
// information.  On the other hand, we are going into the logic 1 half bit
// and the portion which determines the bit value, the start of the
// packet, or if the timer rolls over, the end of the ir packet.
// Since the duration of this half-bit determines the outcome
// we will turn on the timer and its associated interrupt.
// -----------------------------------------------------------------------

//ISR(INT0_vect){
//
//  uint8_t   pin;
//  uint16_t  pulseDuration;      // The timer is 16-bits
//
//  if (IR_DECODER_PIN)   pin=1;  else pin=0;
//
//  switch (pin) {          // read the current pin level
//    case 0:           // !!!!!!!!!NEGATIVE EDGE!!!!!!!!!!
//      pulseDuration = TCNT1;    //**Note** Timer Count register is 16-bits
//      packetData[packetIndex++] = pulseDuration;
//      LOW_2_HIGH;               // Set up pin INT0 on positive edge
//      TIMSK1 &= ~(OCIE1A);          // disable interrupt on match with OCR1A
//      
//      break;
//
//    case 1:             // !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
//      TCNT1 = 0x0000;           // time measurements are based at time 
//      TIFR1 |= (1 << OCF1A); // clear Timer/Counter1, Output Compare A Match Flag by writing 1 
//      TIMSK1 |= (1 << OCIE1A); // enable interrupt on match with OCR1A
//      HIGH_2_LOW;               // Set up pin INT0 on falling edge
//      break;
//  }
//
//}

// -----------------------------------------------------------------------
//            0 half-bit  1 half-bit    TIMER 1 COUNTS    TIMER 1 COUNTS
//  Logic 0   xxx
//  Logic 1
//  Start
//  End
//
// -----------------------------------------------------------------------
/**
 * This function is called whenever the timer 1 output compare match OCR1A
 * is generated.
 **/
//ISR(TIMER1_COMPA_vect){
//  TIMSK1 &= ~(OCIE1A); // disable interrupt on match with OCR1A
//  packetIndex = 0;
//  newIrPacket = TRUE;
//}
