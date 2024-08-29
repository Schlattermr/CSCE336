/*--------------------------------------------------------------------
Name: Matthew Schlatter
Date: 2/1/2024
Course: CSCE 336 Embedded Systems (Spring 2024)
File: Photoresistor.ino
HW/Lab: Lab 1, Photoresistor component

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

const int val = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int photoresistorVal = analogRead(val);
  
  Serial.println(photoresistorVal);

  delay(500);
}
