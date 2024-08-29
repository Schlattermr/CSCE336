/*--------------------------------------------------------------------
Name:   Matthew Schlatter
Date:   4/2/24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   ir_decoder.cpp
HW/Lab: Project 2, Decoding an IR Packet

Purp: Uses counters and interrupts to decode an IR packet for a 
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

// -----------------------------------------------------------------------
// In order to decode IR packets, the Arduino needs to be configured to
// tell time and generate interrupts on positive going edges.  The
// edge sensitivity is used to detect the first incoming IR packet.
// The PinD2 (i.e. INT0) pin change ISR will then toggle the edge sensitivity of
// the interrupt in order to measure the times of the high and low
// pulses arriving from the IR decoder.
//
// Timer 1 must be enabled so that we can tell how long the pulses
// last.  In some degenerate cases, we will need to generate a interrupt
// when the timer rolls over.  This will indicate the end of a packet
// and will be used to alert main that we have a new packet.
// -----------------------------------------------------------------------
void irDecoderSetup() {

  // init timer 1
  TCCR1A = 0; // reset TCCR1A & TCCR1B to defaults
  TCCR1B = 0;
  TCCR1A |= (1 << WGM12); // set Timer 1 to CTC with OCR1A as top
  TCCR1B |= (1 << CS11) | (1 << CS10); // set clock prescaler to clk/64
  OCR1A = 0x3F7A; // configure OCR1A (the counter top) to generate an interrupt every 65ms.
  TCNT1H = 0; // clear timer 1 counter register high byte first
  TCNT1L = 0;

  DDRD &= ~(1 << IR_PIN); // input IR on Port D, Pin 2
  PORTD |= (1 << IR_PIN); // enable Pullup on Port D, Pin 2

  HIGH_2_LOW;	// check the header out.  INT0 edge select has changed
  EIMSK |= (1 << INT0); // Enable External Interrupt Mask 0

}

/**
 * Function to convert time into irPacket data
 */
uint32_t convertData(uint16_t packetData[]) {
  volatile uint32_t irPacket = 0;

  for(int i=0; i<34; i++) {
        if(packetData[i] >= 250 && packetData[i] < 550) { // 1 on ir decoding
          irPacket = (irPacket << 1) | 0x1;
        } else if (packetData[i] < 250 && packetData[i] > 0) { // 0 on ir decoding
          irPacket = (irPacket << 1);
        } else { // ignore start and stop bits here
          irPacket = irPacket;
        }
    }
    
  return irPacket;
}

/**
 * Moves robot in specified way based on ir input
 */
void moveRobotFromIr(uint32_t irPacket) {
  if (irPacket == Green_Button || irPacket == On_Button || irPacket == Elegoo_Up_Button){  // forward 
      robotForward(150, 150);
      return;
    } else if (irPacket == Red_Button || irPacket == Off_Button || irPacket == Elegoo_Select_Button) { // stop
      robotStop();
      return;
    } else if (irPacket == Blue_Button || irPacket == Elegoo_Down_Button) { // backwards
      robotBackwards(150, 150);
      return;
    } else if (irPacket == Music1_Button || irPacket == LightGreen_Button || irPacket == Elegoo_Left_Button) { // left 
      turnLeft(150, 150);
      _delay_ms(350);
      robotStop();
      return;
    } else if (irPacket == Music4_Button || irPacket == LightBlue_Button || irPacket == Elegoo_Right_Button) { // right
      turnRight(150, 150);
      _delay_ms(335);
      robotStop();
      return;
    } else if (irPacket == Quick_Button) { // faster
      robotForward(230, 230);
      return;
    } else if (irPacket == Slow_Button) { // slower
      robotForward(80, 80);
      return;
    } else {
      return;
    }
}
