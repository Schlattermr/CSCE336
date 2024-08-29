/*--------------------------------------------------------------------
Name: Matthew Schlatter
Date: 2/1/2024
Course: CSCE 336 Embedded Systems (Spring 2024)
File: TwoPhotoresistors.ino
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
const int val1 = A0;
const int val2 = A1;
const int servoPin = 9;
int resistorVal = 0;
int position = 90;

void setup() {
  servo.attach(servoPin);
  Serial.begin(9600);
  servo.write(position);
}

void loop() {
  int photoresistorVal1 = analogRead(val1);
  int photoresistorVal2 = analogRead(val2);

  if (photoresistorVal1 > photoresistorVal2) {
    resistorVal -= 1;
  } else if (photoresistorVal2 > photoresistorVal1 && location > 0) {
    resistorVal += 1;
  }

  position = map(resistorVal, 0, 1023, 0, 180);

  servo.write(position);
}
