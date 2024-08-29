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
#include "Arduino.h"
#include "motors.h"
#include "ultrasonic.h"

int ultDistanceLeft = 0;
int ultDistanceMiddle = 0;
int ultDistanceRight = 0;
int ultDistanceLeftPrev = 0;
bool distanceJumped = false;

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
  PORTD |= (1 << ENABLE_A); //enable motor A
  PORTD |= (1 << ENABLE_B); //enable motor B
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

void wallFollowing() {
  ultDistanceLeft = checkDistance();
  _delay_ms(10);
  
  if((ultDistanceLeftPrev - ultDistanceLeft) > 2) { //check to see if there is a rogue value, robot cannot teleport to the right >2 inch
    distanceJumped = true;
  } else {
    distanceJumped = false;
  }
  
  if (ultDistanceLeft >= 10 && ultDistanceLeft < 18 && !distanceJumped) { //too far from wall
    robotForward(220, 179);
  } else if (ultDistanceLeft <= 5.5 && ultDistanceLeft >= 1.5 && !distanceJumped) { //too close to wall
    robotForward(179, 220);
  } else if (ultDistanceLeft < 10 && ultDistanceLeft > 4.5 && ultDistanceLeft != -1 && !distanceJumped) {
    robotForward(185, 185);
  } else if (ultDistanceLeft < 1.5 && ultDistanceLeft != -1) { //robot hit wall
    robotStop();
    while(1) {}
  } else {
    ultDistanceLeftPrev = ultDistanceLeft;
    return;
  }
  ultDistanceLeftPrev = ultDistanceLeft;
}
