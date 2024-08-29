/*--------------------------------------------------------------------
Name:   Matthew Schlatter
Date:   3/6/24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   ultrasonic.h
HW/Lab: Lab 3

Purp: Header file for ultrasonic.cpp implementation

Doc:  PWM timer demo video, lafvin tutorial ultrasonic sensor code
      on dropbox, pulseIn() arduino website

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/
#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <avr/io.h>
#include "NewPing.h"

#define OCR2A_PWM PINB3
#define OCR2B_PWM PIND3
#define TRIG_PIN A1
#define ECHO_PIN A0
#define RED_LED PB3
#define GREEN_LED PB4
#define BLUE_LED PB5

/**
 * Calculates distance between ultrasonic sensor and object.
 */
float checkDistance();

/**
 * Moves servo from right to the middle to the left and takes
 * a measurement from the ultrasonic sensor at each location.
 * If an object is within 12 inches to the right, the green led will
 * turn on. If an object is within 12 inches in front, the blue led
 * will turn on. If an object is within 12 inches of the left, the red
 * led will turn on.
 */
void pwmServo();

/**
 * Keeps servo in the middle and measures distance in front of robot.
 * Turns on green led if an object is within 12 inches.
 */
void servoOneDirection();

/**
 * Setup timer 1 and input/output for ultrasonic sensor
 */
void sensorSetup();

#endif
