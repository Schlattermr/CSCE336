/*--------------------------------------------------------------------
Name: Matthew Schlatter
Date: 2/1/2024
Course: CSCE 336 Embedded Systems (Spring 2024)
File: LEDs.ino
HW/Lab: Lab 1, LED component

Purp:   A brief description of what this program does and 
        the general solution strategy. 

Doc:    Arduino examples website, 

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor 
code. I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/

const int redPin = 5;
const int greenPin = 6;
const int bluePin = 7;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(redPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                     
  digitalWrite(redPin, LOW);    // turn the LED off by making the voltage LOW
  delay(500);        
  digitalWrite(greenPin, HIGH);   
  delay(500);                     
  digitalWrite(greenPin, LOW);   
  delay(500);  
  digitalWrite(bluePin, HIGH);  
  delay(500);                     
  digitalWrite(bluePin, LOW);  
  delay(500);        
}
