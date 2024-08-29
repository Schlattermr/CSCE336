/*--------------------------------------------------------------------
Name:   Matthew Schlatter
Date:   May 9 2024
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   pong.h (Pong Header)
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

#ifndef _PONG_H
#define _PONG_H

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128

typedef struct {
	  int16_t x;
    int16_t y;
} vector2d_t;

typedef struct {
    vector2d_t position;
    vector2d_t velocity;
    uint8_t  radius;
} ball_t;

typedef struct {
    vector2d_t position;
    int16_t velocity;
    uint8_t  height;
    uint8_t  width;
} paddle;

ball_t createBall(int16_t xPos, int16_t yPos, int16_t xVel, int16_t yVel, uint8_t  radius);

ball_t moveBall(ball_t ballToMove, paddle myPaddle);

paddle createPaddle(int16_t xPos, int16_t yPos, int16_t yVel, uint8_t height, uint8_t width);

paddle movePaddle(paddle paddleToMove);

int16_t readAnalogInput(uint8_t analogPin);

#endif
