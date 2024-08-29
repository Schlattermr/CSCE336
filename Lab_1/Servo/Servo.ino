/*--------------------------------------------------------------------
Name: Matthew Schlatter
Date: 2/1/2024
Course: CSCE 336 Embedded Systems (Spring 2024)
File: Servo.ino
HW/Lab: Lab 1, Servo component

Purp:   A brief description of what this program does and 
        the general solution strategy. 

Doc:    Arduino examples website, public domain code from servo 
        Arduino reference, google for how to install servo library

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor 
code. I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/
#include <Servo.h>
Servo servo;

const int buttonPin = 2;

int position = 0;
int buttonState = HIGH;

void setup() {
  servo.attach(9);
  pinMode(buttonPin, INPUT_PULLUP);
  servo.write(position);
}

void servoMovement() {
  for(position = 0; position < 180; position += 1) {
      servo.write(position);
      delay(10);
  }

  delay(500);
  
  for(position = 180; position > 0; position -= 1) {
      servo.write(position);
      delay(10);
  }
}

void loop() {
  buttonState = digitalRead(buttonPin);
   
  if (buttonState == LOW){
    servoMovement();
  }
}
