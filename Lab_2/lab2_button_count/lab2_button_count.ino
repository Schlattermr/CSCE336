/*--------------------------------------------------------------------
Name:   Matthew Schlatter and Caleb Poggemeyer
Date:   2/15/24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   lab2_button_count.ino
HW/Lab: Lab 2, Button Count Code

Purp: Various timer and button functions

Doc:  millis() arduino website, Atmega datasheet, arduino uno schematic
      course slides, lab 2 video

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/

#include <Arduino.h>
//#include <avr/io.h>

// Definitions for Port B & D registors
#define PINB_Reg (*((volatile uint8_t *) 0x23))
#define DDRB_Reg (*((volatile uint8_t *) 0x24))
#define PORTB_Reg (*((volatile uint8_t *) 0x25))
#define PIND_Reg (*((volatile uint8_t *) 0x29))
#define DDRD_Reg (*((volatile uint8_t *) 0x2a))
#define PORTD_Reg (*((volatile uint8_t *) 0x2b))
// Definitions for LED assignments:
#define BOARD_LED 5   //pin 13 is PortB bit 5
#define RED_LED 1     //pin 9 is PortB bit 1  
#define GREEN_LED 2  //pin 10 is PortB bit 2
#define BLUE_LED 3   //pin 11 is PortB bit 3

#define BUTTON 5 

int TCNT1old = 0;
double value = 0;
double bounceTime = 0;
int buttonState = HIGH;
int tempButtonState = HIGH;
unsigned long startTime = 0;
unsigned long endTime = 0;

/**
 * Init all of the LEDs and test them
 **/ 
void LEDInit(){
 //Set pinmode for LEDs to output 
  DDRB_Reg |= (1 << BOARD_LED);
  DDRB_Reg |= (1 << RED_LED);
  DDRB_Reg |= (1 << GREEN_LED);
  DDRB_Reg |= (1 << BLUE_LED);

  //Turn all off
  PORTB_Reg &= ~(1 << BOARD_LED); //clear output
  PORTB_Reg &= ~(1 << RED_LED);   //clear output
  PORTB_Reg &= ~(1 << GREEN_LED); //clear output
  PORTB_Reg &= ~(1 << BLUE_LED);  //clear output

  //Test LEDs
  Serial.println("Testing LEDs...");
  PORTB_Reg |= (1 << BOARD_LED);  //set output
  PORTB_Reg |= (1 << RED_LED);    //set output
  delay(400);
  PORTB_Reg &= ~(1 << RED_LED);   //clear output
  PORTB_Reg |= (1 << GREEN_LED);  //set output
  delay(400);
  PORTB_Reg &= ~(1 << GREEN_LED); //clear output
  PORTB_Reg |= (1 << BLUE_LED);   //set output
  delay(400);
  PORTB_Reg &= ~(1 << BLUE_LED);   //clear output
  PORTB_Reg &= ~(1 << BOARD_LED);   //clear output
  Serial.println("Finished LED testing!");
 }

void setup() {                
  Serial.begin(9600);
  Serial.println("Starting up...");
  LEDInit();
  //Set pinmode for Button as input
  DDRD_Reg &= ~(1 << BUTTON);
  //Enable pullup 
  PORTD_Reg |= (1 << BUTTON);  //set output to enable pullup resistor

  
  //Init counter1
  TCCR1A = 0; //Normal mode 0xffff top, rolls over
  TCCR1B = (1 << CS10); //1024 prescaler
  TCCR1C = 0;
  //Set counter to zero, high byte first
  TCNT1H = 0;
  TCNT1L = 0;  
  //Make sure interrupts are disabled 
  TIMSK1 = 0;
  TIFR1 = 0;
  
  Serial.println("Finished setup!");
}

/**
 * Counts each time a button is pressed and also detects bounces
 */
void buttonCounter() {
  buttonState = digitalRead(BUTTON);

  //button pressed
  if(buttonState == LOW) {
    if(TCNT1 != TCNT1old) {
      //bounce detection because it should only increment by one
      if(TCNT1 > (TCNT1old + 1)) {
        Serial.print("TCNT1: ");
        Serial.print(TCNT1);
        Serial.println(" (WARNING: BUTTON BOUNCED)");
      } else {
        Serial.print("TCNT1: ");
        Serial.println(TCNT1);
      }
      TCNT1old = TCNT1;
    }
  }
  
  delay(100);
}

/**
 * Calculates prescaler time
 */
void prescalerTimer() {
  float onePrescaler = 65536.0 / (16.0 / 1.0);
  float eightPrescaler = 65536.0 / (16.0 / 8.0);
  float sixFourPrescaler = 65536.0 / (16.0 / 64.0);
  float twoFiveSixPrescaler = 65536.0 / (16.0 / 256.0);
  float oneZeroTwoFourPrescaler = 65536.0 / (16.0 / 1024.0);

  Serial.print(onePrescaler);
  Serial.println(" microseconds");
  Serial.print(eightPrescaler);
  Serial.println(" microseconds");
  Serial.print(sixFourPrescaler);
  Serial.println(" microseconds");
  Serial.print(twoFiveSixPrescaler);
  Serial.println(" microseconds");
  Serial.print(oneZeroTwoFourPrescaler);
  Serial.println(" microseconds");

  delay(10000);
}

/**
 * Determines length of bounce on press if bounce occurs
 */
void bounceTimerPress() {
  buttonState = digitalRead(BUTTON);

  if(buttonState == LOW) {
    //reset registers to be ready for bounce
    TCNT1 = 0;
    ICR1 = 0;
    delay(200);

    value = ICR1;
  
    if(value != 0) {
      bounceTime = value / (16.0 / 64.0);
      
      Serial.print("Bounce time: ");
      Serial.print(bounceTime);
      Serial.println(" microseconds");
    } else {
      Serial.println("No bounce occurred");
    }
  }
}

/**
 * Determines length of bounce on release if bounce occurs
 */
void bounceTimerRelease() {
  buttonState = digitalRead(BUTTON);

  if(buttonState == LOW) {
    //detect if button is pressed first
    tempButtonState = LOW;
  }

  //button is released
  if(buttonState != tempButtonState) {
    tempButtonState = buttonState;
    //reset registers to be ready for bounce
    TCNT1 = 0;
    ICR1 = 0;
    delay(200);

    value = ICR1;
  
    if(value != 0) {
      bounceTime = value / (16.0 / 64.0);
      
      Serial.print("Bounce time: ");
      Serial.print(bounceTime);
      Serial.println(" microseconds");
    } else {
      Serial.println("No bounce occurred");
    }
  }
}

/**
 * Times how long button was pressed
 */
void timeButtonPress() {
  buttonState = digitalRead(BUTTON);

  //button pressed
  if(buttonState == LOW && tempButtonState == HIGH) {
    startTime = millis();
  }

  //button released
  if(buttonState == HIGH && tempButtonState == LOW) {
    endTime = millis();

    Serial.print("Button held for ");
    Serial.print(endTime - startTime);
    Serial.println(" milliseconds");
  }

  //set both to HIGH
  tempButtonState = buttonState;
}

void loop() {
  //buttonCounter();
  //prescalerTimer();
  //bounceTimerPress();
  //bounceTimerRelease();
  timeButtonPress();
}
