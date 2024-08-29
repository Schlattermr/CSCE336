/*--------------------------------------------------------------------
Name:   Matthew Schlatter
Date:   3/3/24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   Lab_3.ino
HW/Lab: Lab 3

Purp: Sense objects within 12 inches of the robot using the ultrasonic
      sensor and servo movements.

Doc:  PWM timer demo video, lafvin tutorial ultrasonic sensor code
      on dropbox, pulseIn() arduino website

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/

#include <Arduino.h>
#include <NewPing.h>

#define OCR2A_PWM PINB3
#define OCR2B_PWM PIND3
#define TRIG_PIN A1
#define ECHO_PIN A0
#define RED_LED 11 //red for wall to the right
#define GREEN_LED 12 //green for wall in front
#define BLUE_LED 13 //blue for wall to the left
int ultDistanceLeft = 0;
int ultDistanceMiddle = 0;
int ultDistanceRight = 0;

float checkDistance() {
  PORTC &= ~(1 << PORTC1); //set TRIG_PIN to low
  _delay_us(2);
  PORTC |= (1 << PORTC1); //set TRIG_PIN to high
  _delay_us(10);
  PORTC &= ~(1 << PORTC1); //set TRIG_PIN to low
  
  unsigned long duration = pulseIn(ECHO_PIN, HIGH);
  float seconds = duration / 1000000.0;
  float distance = (seconds * 340 * 100 * (1 / 2.54)) / 2.0; //(us to seconds) * (seconds to meters, 
                                                             //used 340m/s as speed of sound) *
                                                             //(m to cm) * (cm to inches) / 2.0 for one-way distance
  delay(10);
  return distance;
}

void pwmServo() {
  Serial.println("Move servo right");
  OCR2B = 10;
  ultDistanceRight = checkDistance();
  Serial.print("Distance on right: ");
  Serial.println(ultDistanceRight);
  if(ultDistanceRight > 0 && ultDistanceRight < 12) { //within 12 inches of the object
    digitalWrite(RED_LED, HIGH);
  } else {
    digitalWrite(RED_LED, LOW);
  }
  delay(400);

  
  Serial.println("Move servo middle");
  OCR2B = 25;
  ultDistanceMiddle = checkDistance();
  Serial.print("Distance on middle: ");
  Serial.println(ultDistanceMiddle);
  if(ultDistanceMiddle > 0 && ultDistanceMiddle < 12) {
    digitalWrite(GREEN_LED, HIGH);
  } else {
    digitalWrite(GREEN_LED, LOW);
  }
  delay(400);

  
  Serial.println("Move servo left");
  OCR2B = 40;
  ultDistanceLeft = checkDistance();
  Serial.print("Distance on left: ");
  Serial.println(ultDistanceLeft);
  if(ultDistanceLeft > 0 && ultDistanceLeft < 12) {
    digitalWrite(BLUE_LED, HIGH);
  } else {
    digitalWrite(BLUE_LED, LOW);
  }
  delay(400);
}

void servoOneDirection() {
  Serial.println("Set servo middle");
  OCR2B = 25;
  ultDistanceMiddle = checkDistance();
  Serial.print("Distance on middle: ");
  Serial.print(ultDistanceMiddle);
  Serial.println("in");
  if(ultDistanceMiddle > 0 && ultDistanceMiddle < 12) {
    digitalWrite(GREEN_LED, HIGH);
  } else if(ultDistanceMiddle == 0 || ultDistanceMiddle >= 12) {
    digitalWrite(GREEN_LED, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  //setup timer 2 for servo
  DDRD |= (1 << DDD3); //set OCR2B as output
  TCCR2A = ((1 << COM2B1) | (1 << WGM21) | (1 << WGM20)); //Fast PWM with OCR2A as top
                                                          //Non-inverting mode for OCR2B
  TCCR2B = ((1 << WGM22) | (1 << CS22) | (1 << CS21) | (1 << CS20)); //1024 prescaler
  TCNT2 = 0;
  
  OCR2A = 255; //16ms pulse width, max for timer 2
  OCR2B = 16; //left for 1ms

  //setup timer 1 for ultrasonic sensor
  DDRC |= (1 << DDC1); //TRIG_PIN set to output
  DDRC &= ~(1 << DDC0); //ECHO_PIN set to input

  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS12) | (1 << CS11); //falling edge clock
  TCCR1C = 0;
 
  OCR1A = 3125; // Set TOP value for 50ms
}

void loop() {
  pwmServo();
  //servoOneDirection();
}
