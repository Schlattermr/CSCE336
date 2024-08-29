/*--------------------------------------------------------------------
Name:   Matthew Schlatter
Date:   3/28/24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   motors.cpp
HW/Lab: Project 1

Purp: Holds all motor functions for project 1

Doc:  

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

void motorSetup() {
  TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1); //fast PWM, non-inverting, TOP at 0xFF
  TCCR0B |= (1 << CS01); //prescaler 8

  // setup pins for DC motor control
  DDRD |= (1 << DDD5) | (1 << DDD6) | (1 << DDD7) | (1 << DDD4); //ENABLE_A, ENABLE_B, IN4, IN3
  DDRB |= (1 << DDB2) | (1 << DDB1); //IN1, IN2
}

void danceRobot() {
  
  robotForward(200, 200);
  _delay_ms(1500);

  robotStop();
  _delay_ms(500);

  robotBackwards(200, 200);
  _delay_ms(1500);

  robotStop();
  _delay_ms(500);

  turnLeft(100, 100);
  _delay_ms(300);

  robotStop();
  _delay_ms(400);

  turnRight(100, 100);
  _delay_ms(500);

  robotStop();
  _delay_ms(500);

  turnLeft(100, 100);
  _delay_ms(800);

  robotStop();
  _delay_ms(500);

  turnRight(100, 100);
  _delay_ms(1000);

  robotStop();
  _delay_ms(500);
}

void robotForward(int OCR0Ain, int OCR0Bin) {
  PORTB |= (1 << IN1); //HIGH
  PORTB &= ~(1 << IN2); //LOW
  PORTD |= (1 << IN3); //HIGH
  PORTD &= ~(1 << IN4); //LOW
  PORTD |= (1 << ENABLE_A); // enable motor A
  PORTD |= (1 << ENABLE_B); // enable motor B
  OCR0A = OCR0Ain;
  OCR0B = OCR0Bin;
}

void robotBackwards(int OCR0Ain, int OCR0Bin) {
  PORTB &= ~(1 << IN1); //LOW
  PORTB |= (1 << IN2); //HIGH
  PORTD &= ~(1 << IN3); //LOW
  PORTD |= (1 << IN4); //HIGH
  PORTD |= (1 << ENABLE_A);
  PORTD |= (1 << ENABLE_B);
  OCR0A = OCR0Ain;
  OCR0B = OCR0Bin;
}

void turnLeft(int OCR0Ain, int OCR0Bin) {
  PORTB &= ~(1 << IN1); //LOW
  PORTB |= (1 << IN2); //HIGH
  PORTD |= (1 << IN3); //HIGH
  PORTD &= ~(1 << IN4); //LOW
  PORTD |= (1 << ENABLE_A);
  PORTD |= (1 << ENABLE_B);
  OCR0A = OCR0Ain;
  OCR0B = OCR0Bin;
}

void turnRight(int OCR0Ain, int OCR0Bin) {
  PORTB |= (1 << IN1); //HIGH
  PORTB &= ~(1 << IN2); //LOW
  PORTD &= ~(1 << IN3); //LOW
  PORTD |= (1 << IN4); //HIGH
  PORTD |= (1 << ENABLE_A);
  PORTD |= (1 << ENABLE_B);
  OCR0A = OCR0Ain;
  OCR0B = OCR0Bin;
}

void robotStop() {
  PORTD &= ~(1 << ENABLE_A);
  PORTD &= ~(1 << ENABLE_B);
  OCR0A = 0;
  OCR0B = 0;
}

void wallFollowing(int ultDistanceSide, int ultDistanceFront) {
  if(ultDistanceFront < 13 && ultDistanceSide < 18) { // corner w/ wall in front and side
    robotStop();
    _delay_ms(300);
    turnRight(150, 150);
    _delay_ms(320);
    robotStop();
    _delay_ms(300);
    robotForward(130, 130);
    return;
  } else if(ultDistanceFront < 13 && ultDistanceSide > 20) { // obstacle in front, none to left
    robotStop();
    _delay_ms(300);
    turnLeft(150, 150);
    _delay_ms(320);
    robotStop();
    _delay_ms(300);
    robotForward(130, 130);
    return;
  } else if (ultDistanceSide >= 11 && ultDistanceSide <= 18) { // too far
    robotForward(143, 120);
    return;
  } else if(ultDistanceSide > 20 && ultDistanceFront > 20) { // no obstacle in front or left
    robotStop();
    _delay_ms(300);
    turnLeft(150, 150);
    _delay_ms(320);
    robotStop();
    _delay_ms(300);
    robotForward(130, 130);
    return;
  } else if(ultDistanceSide < 5 && ultDistanceSide != -1) { // too close
    robotForward(120, 143);
    return;
  } else {
    robotForward(130, 130);
    return;
  }
}
