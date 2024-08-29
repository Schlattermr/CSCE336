/*--------------------------------------------------------------------
Name:   Matthew Schlatter
Date:   May 9 2024
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   pong.c (Pong Implementation)
HW/Lab: Pong Lab

Purp: Implements a subset of the pong game

Doc:    None

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
-------------------------------------------------------------------------*/
#include <Arduino.h>         // jlf added for vs_code
#include <Wire.h>            // jlf added for vs_code
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>       
#include "pong.h"

ball_t createBall(int16_t xPos, int16_t yPos, int16_t xVel, int16_t yVel, uint8_t radius) {
  ball_t myNewBall = {xPos, yPos, xVel, yVel, radius};
	return myNewBall;
}

// By defining the collision detector functions in the implementation file instead of the header
// Makes it so main can't call these functions  
ball_t checkBounceTopOrBottom(ball_t ballToTest);

ball_t checkBounceLeftOrRight(ball_t ballToTest, paddle paddleToTest);

// Collision detector function implementations
ball_t checkBounceTopOrBottom(ball_t ballToTest) {
  ballToTest.velocity.y = -ballToTest.velocity.y;
  return ballToTest;
}

ball_t checkBounceLeftOrRight(ball_t ballToTest, paddle paddleToTest) {
  if(((ballToTest.position.y <= (paddleToTest.position.y + paddleToTest.height)) && 
      (ballToTest.position.y >= paddleToTest.position.y) && ballToTest.position.x >= 149) || 
       ballToTest.position.x <= 6) { // ball hits paddle or left wall
    ballToTest.velocity.x = -ballToTest.velocity.x;
  } else {
    ballToTest.velocity.x = 0;
    ballToTest.velocity.y = 0;
  }
  return ballToTest;
}

ball_t moveBall(ball_t ballToMove, paddle paddleToTest) {
  if(ballToMove.position.y <= 6 || ballToMove.position.y >= 122) { // top or bottom walls
    ballToMove = checkBounceTopOrBottom(ballToMove);
    ballToMove.position.x += ballToMove.velocity.x;
    ballToMove.position.y += ballToMove.velocity.y;
  } else if(ballToMove.position.x >= 149) { // right wall
    ballToMove = checkBounceLeftOrRight(ballToMove, paddleToTest);
    ballToMove.position.x += ballToMove.velocity.x;
    ballToMove.position.y += ballToMove.velocity.y;
  } else if(ballToMove.position.x <= 6) {
    ballToMove = checkBounceLeftOrRight(ballToMove, paddleToTest);
    ballToMove.position.x += ballToMove.velocity.x;
    ballToMove.position.y += ballToMove.velocity.y;
  } else {
    ballToMove.position.x += ballToMove.velocity.x;
    ballToMove.position.y += ballToMove.velocity.y;
  }
	return ballToMove;
}

paddle createPaddle(int16_t xPos, int16_t yPos, int16_t yVel, uint8_t height, uint8_t width) {
  paddle myNewPaddle = {xPos, yPos, yVel, height, width};
  return myNewPaddle;
}

paddle movePaddle(paddle paddleToMove) {
  paddleToMove.position.y += paddleToMove.velocity;
  return paddleToMove;
}

int16_t readAnalogInput(uint8_t analogPin) {
  int16_t ADCvalue;
  ADMUX = (ADMUX & 0xF0) | analogPin; // set mux for appropriate input ADC pin while clearing bottom 4 bits
  ADCSRA |= (1 << ADSC); // writing a '1' to the ADC Start Conversion bit in the ADC Control and Status Register A (ADCSRA.ADSC).
  while (ADCSRA & (1 << ADSC)); // while no ADSC == 1 (i.e. ADC conversion not complete)
  ADCvalue = ADC; // read ADC values and return
  return ADCvalue;
}
