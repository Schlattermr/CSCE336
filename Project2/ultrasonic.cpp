/*--------------------------------------------------------------------
Name:   Matthew Schlatter
Date:   3/6/24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   ultrasonic.cpp
HW/Lab: Lab 3

Purp: Hold all functions for lab 3

Doc:  PWM timer demo video, lafvin tutorial ultrasonic sensor code
      on dropbox, pulseIn() arduino website

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/
#include <avr/io.h>
#include "ir_decoder.h"
#include "motors.h"
#include "ultrasonic.h"
#include "NewPing.h"

float checkDistance() {
  PORTC &= ~(1 << PORTC1); //TRIG_PIN LOW
  _delay_us(2);
  PORTC |= (1 << PORTC1); //HIGH
  _delay_us(10);
  PORTC &= ~(1 << PORTC1); //LOW

  unsigned long duration = pulseIn(ECHO_PIN, HIGH);
  
  float seconds = duration / 1000000.0;
  float distance = (seconds * 340 * 100 * (1 / 2.54)) / 2.0; //((us to seconds) * (seconds to meters, 
                                                             //used 340m/s as speed of sound) *
                                                             //(m to cm) * (cm to inches)) / 2.0 for one-way distance
  if(distance == 0.00) {
    return -1;
  } else {
    return distance;
  }
}

void pwmServo() {
  int ultDistanceLeft = 0;
  int ultDistanceMiddle = 0;
  int ultDistanceRight = 0;
  
  Serial.println("Move servo right");
  OCR2B = 10;
  ultDistanceRight = checkDistance();
  Serial.print("Distance on right: ");
  Serial.println(ultDistanceRight);
  if(ultDistanceRight > 0 && ultDistanceRight < 12) { //within 12 inches of the object
    PORTB |= (1 << RED_LED);
  } else {
    PORTB &= ~(1 << RED_LED);
  }
  _delay_ms(400);
  
  Serial.println("Move servo middle");
  OCR2B = 25;
  ultDistanceMiddle = checkDistance();
  Serial.print("Distance on middle: ");
  Serial.println(ultDistanceMiddle);
  if(ultDistanceMiddle > 0 && ultDistanceMiddle < 12) {
    PORTB |= (1 << GREEN_LED);
  } else {
    PORTB &= ~(1 << GREEN_LED);
  }
  _delay_ms(400);
  
  Serial.println("Move servo left");
  OCR2B = 40;
  ultDistanceLeft = checkDistance();
  Serial.print("Distance on left: ");
  Serial.println(ultDistanceLeft);
  if(ultDistanceLeft > 0 && ultDistanceLeft < 12) {
    PORTB |= (1 << BLUE_LED);
  } else {
    PORTB &= ~(1 << BLUE_LED);
  }
  _delay_ms(400);
}

void servoOneDirection() {
  int ultDistanceLeft = 0;
  
  Serial.println("Set servo left");
  OCR2B = 40;
  ultDistanceLeft = checkDistance();
  Serial.print("Distance on left: ");
  Serial.print(ultDistanceLeft);
  Serial.println("in");
  if(ultDistanceLeft > 0 && ultDistanceLeft < 12) {
    PORTB |= (1 << BLUE_LED);
  } else if(ultDistanceLeft == 0 || ultDistanceLeft >= 12) {
    PORTB &= ~(1 << BLUE_LED);
  }
}

void sensorSetup() {
  DDRC |= (1 << DDC1); //TRIG_PIN set to output
  DDRB &= ~(1 << DDB0); //ECHO_PIN set to input

  //setup timer 1 for ultrasonic sensor
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << ICES1) | (1 << ICNC1); //prescaler 1024, capture rising edge, noise canceler
  TCCR1C = 0;
  OCR1A = 781; // Set TOP value to 50ms
}
