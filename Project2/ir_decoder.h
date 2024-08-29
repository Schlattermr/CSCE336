/*--------------------------------------------------------------------
Name:   Matthew Schlatter
Date:   4/2/24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   ir_decoder.h
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
#ifndef IR_DECODER_H
#define IR_DECODER_H

#include <avr/io.h>
#include <util/delay.h>

#define		TRUE				1
#define		FALSE				0

/*-----------------------------------------------------------------
	Function prototypes found in project2.cpp
-----------------------------------------------------------------*/
void  irDecoderSetup();

uint32_t convertData(uint16_t packetData[]);

void moveRobotFromIr(uint32_t irPacket);

//-----------------------------------------------------------------
// Each ISC0n bit selects the external interrupt 0 edge for the corresponding I/O pin.
// Reference page 89 of the datasheet
//	Bits = 10 : The INT0 flag is set with a high-to-low (falling edge) transition
//  Bits = 11 : The INT0 flag is set with a low-to-high (rising edge) transition
//-----------------------------------------------------------------

#define 	IR_DECODER_PIN  (PIND & (1 << PIND2))
#define 	IR_PIN    		(PIND2)
#define		HIGH_2_LOW		(EICRA = (EICRA & ~(1 << ISC00)) | (1 << ISC01))
#define		LOW_2_HIGH		(EICRA |= (1 << ISC00) | (1 << ISC01))

/* Elegoo Remote */
#define		Elegoo_Up_Button 0x00FF629D
#define		Elegoo_Down_Button 0x00FFA857
#define		Elegoo_Left_Button 0x00FF22DD
#define		Elegoo_Right_Button 0x00FFC23D
#define		Elegoo_Select_Button 0x00FF02FD

/* LED Remote */
#define		Green_Button 0x00FF9A65
#define		Red_Button 0x00FF1AE5
#define		Music1_Button 0x00FF20DF
#define		Music4_Button 0x00FFE01F
#define   LightBlue_Button 0x00FF926D
#define   LightGreen_Button 0x00FFAA55
#define		On_Button	0x00FF02FD
#define   Off_Button 0x00FF827D
#define   Blue_Button 0x00FFA25D
#define   Quick_Button 0x00FF6897
#define   Slow_Button 0x00FF48B7

#endif
