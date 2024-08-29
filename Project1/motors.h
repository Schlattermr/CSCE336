/*--------------------------------------------------------------------
Name:   Matthew Schlatter
Date:   3/28/24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   motors.h
HW/Lab: Project 1

Purp: Header file for motors.cpp implementation

Doc:  

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/
#ifndef MOTORS_H
#define MOTORS_H

#include <avr/io.h>

#include "Arduino.h"

#include <util/delay.h>

#define OCR2B_PWM PD3
#define TRIG_PIN A1
#define ECHO_PIN A0
#define RED_LED PB3
#define GREEN_LED PB4
#define BLUE_LED PB5

#define ENABLE_A PD5
#define ENABLE_B PD6
#define IN1 PB2
#define IN2 PB1
#define IN3 PD4
#define IN4 PD7

/**
 * Robots moves forward, backwards, turns left 45 degrees, 
 * turns right 45 degrees, turns left 90 degrees, and then
 * turns right 90 degrees.
*/
void danceRobot();

/**
 * Robot moves forward.
*/
void robotForward(int OCR0Ain, int OCR0Bin);

/**
 * Robot moves backwards by flipping IN pins.
*/
void robotBackwards(int OCR0Ain, int OCR0Bin);

/**
 * Robot turns left by moving left wheel backwards
 * and right wheel forward.
*/
void turnLeft(int OCR0Ain, int OCR0Bin);

/**
 * Robot turns right by moving right wheel backwards
 * and left wheel forward.
*/
void turnRight(int OCR0Ain, int OCR0Bin);

/**
 * Stops robot by turning PWM pins off and setting both
 * OCR0A and OCR0B to zero.
*/
void robotStop();

/**
 * Algorithm to follow the wall and adjust depending on
 * the distance from the wall, which is determined from
 * the ultrasonic sensor.
*/
void wallFollowing();

#endif
