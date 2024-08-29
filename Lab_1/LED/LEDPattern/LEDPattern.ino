/*--------------------------------------------------------------------
Name: Matthew Schlatter
Date: 2/1/2024
Course: CSCE 336 Embedded Systems (Spring 2024)
File: LedPattern.ino
HW/Lab: Lab 1, LED and button component

Purp:   A brief description of what this program does and 
        the general solution strategy. 

Doc:    Arduino examples website, wednesday office hours with Falkinburg, 
        https://www.circuitbasics.com/how-to-connect-and-program-push-buttons-on-the-arduino/

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor 
code. I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/
#include <Arduino.h>

const int buttonPin = 2;  
const int redPin =  5; 
const int greenPin =  6; 
const int bluePin =  7; 

int buttonState = HIGH; 

void setup() {
  // initialize the LED pin as an output:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == LOW) {
    digitalWrite(redPin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);      
    digitalWrite(greenPin, HIGH);
    delay(1000);               
    digitalWrite(redPin, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);        
    digitalWrite(bluePin, HIGH);
    delay(1000);                     
    digitalWrite(greenPin, LOW);    
    delay(1000);  
    digitalWrite(redPin, HIGH);   
    delay(1000);                     
    digitalWrite(bluePin, LOW);
    digitalWrite(redPin, LOW);    
    delay(1000);  
  }
}
