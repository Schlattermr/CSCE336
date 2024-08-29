/*--------------------------------------------------------------------
Name: Matthew Schlatter and Caleb Poggemeyer
Date: 2/1/2024
Course: CSCE 336 Embedded Systems (Spring 2024)
File: Lab_1.ino
HW/Lab: Lab 1

Purp:   A brief description of what this program does and 
        the general solution strategy. 

Doc:    Arduino examples website, wednesday office hours with Falkinburg, 
        https://www.circuitbasics.com/how-to-connect-and-program-push-buttons-on-the-arduino/,
        public domain code from servo Arduino reference

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
const int buttonPin = 2;
const int redPin =  5; 
const int greenPin =  6; 
const int bluePin =  7; 

int position = 0;
int resistorVal = 0;
int buttonState = HIGH;

void setup() {
  Serial.begin(9600);
  servo.attach(servoPin);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  servo.write(position);
}

void ledPattern() {
  digitalWrite(redPin, HIGH);   // turn the LED on by making the voltage HIGH
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

void photoresistorValues() {
  int photoresistorVal = analogRead(val1);
  
  Serial.println(photoresistorVal);

  delay(500);
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

void followLightTwoPhotoresistors() {
  int photoresistorVal1 = analogRead(val1);
  int photoresistorVal2 = analogRead(val2);

  if (photoresistorVal1 > photoresistorVal2) {
    resistorVal -= 1;
  } else {
    resistorVal += 1;
  }

  position = map(resistorVal, 0, 1023, 0, 180);

  servo.write(position);
}

void followLightOnePhotoresistor() {
  int lightIntensity = 0;
  int bestIntensity = 0;
  int currentPosition = 0;
  int bestPosition = 90;

  for (currentPosition = 0; currentPosition < 180; currentPosition++) {
    servo.write(currentPosition);

    lightIntensity = analogRead(val1);

      if(lightIntensity > bestIntensity) {
        bestIntensity = lightIntensity;
        bestPosition = currentPosition;
      }
   }
   servo.write(bestPosition);

  delay(5000);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  photoresistorValues();
  followLightTwoPhotoresistors();
  followLightOnePhotoresistor();
   
  if (buttonState == LOW){
    ledPattern();
    servoMovement();
  }
}
