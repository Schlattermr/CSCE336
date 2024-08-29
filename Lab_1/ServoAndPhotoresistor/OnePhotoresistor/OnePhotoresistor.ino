/*--------------------------------------------------------------------
Name: Matthew Schlatter
Date: 2/1/2024
Course: CSCE 336 Embedded Systems (Spring 2024)
File: OnePhotoresistor.ino
HW/Lab: Lab 1, Servo and photoresistor component

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
#include <Servo.h>

Servo servo;
const int val = A0;
int position = 0;

void setup() {
  servo.attach(9);
  Serial.begin(9600);
}

void loop() {
  int lightIntensity = 0;
  int currentPosition = 0;
  int bestPosition = 90;
  int bestIntensity = 0;

 for (currentPosition = 0; currentPosition < 180; currentPosition++) {
  servo.write(currentPosition);

  lightIntensity = analogRead(photoresistorPin);

    if(lightIntensity > bestIntensity) {
      bestIntensity = lightIntensity;
      bestPosition = currentPosition;
    }
  }
  myServo.write(bestPosition);

  delay(5000);
}
