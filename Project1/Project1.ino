/*--------------------------------------------------------------------
Name:   Matthew Schlatter
Date:   3/19/24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   Project1.ino
HW/Lab: Project 1

Purp: Sense objects within 12 inches of the robot using the ultrasonic
      sensor and servo movements.

Doc:  PWM timer demo video, lafvin tutorial ultrasonic sensor code
      on dropbox

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/
#include <Arduino.h>
#include <NewPing.h>
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

int ultDistanceLeft = 0;
int ultDistanceMiddle = 0;
int ultDistanceRight = 0;
int ultDistanceLeftPrev = 0;
bool distanceJumped = false;
unsigned long duration = 0;
unsigned long startTime = 0;
unsigned long endTime = 0;
int ICF1past = 0;

float checkDistance() {
  PORTC &= ~(1 << PORTC1); //TRIG_PIN LOW
  _delay_us(2);
  PORTC |= (1 << PORTC1); //HIGH
  _delay_us(10);
  PORTC &= ~(1 << PORTC1); //LOW

  unsigned long duration = pulseIn(ECHO_PIN, HIGH);

  float seconds = duration / 1000000.0;
  float distance = (seconds * 340 * 100 * (1 / 2.54)) / 2.0; //((us to seconds) * (seconds to meters, 
                                                             //used 340m/s as speed of sound) *
                                                             //(m to cm) * (cm to inches)) / 2.0 for one-way distance
  Serial.println(distance);
  if(distance <= 0.00) {
    return -1;
  } else {
    return distance;
  }
}

void pwmServo() {
  Serial.println("Move servo right");
  OCR2B = 10;
  ultDistanceRight = checkDistance();
  Serial.print("Distance on right: ");
  Serial.println(ultDistanceRight);
  if(ultDistanceRight > 0 && ultDistanceRight < 12) { //within 12 inches of the object
    PORTB |= (1 << RED_LED);
  } else if (ultDistanceRight != -1) {
    PORTB &= ~(1 << RED_LED);
  } else {}
  _delay_ms(400);
  
  Serial.println("Move servo middle");
  OCR2B = 25;
  ultDistanceMiddle = checkDistance();
  Serial.print("Distance on middle: ");
  Serial.println(ultDistanceMiddle);
  if(ultDistanceMiddle > 0 && ultDistanceMiddle < 12) {
    PORTB |= (1 << GREEN_LED);
  } else if (ultDistanceMiddle != -1) {
    PORTB &= ~(1 << GREEN_LED);
  } else {}
  _delay_ms(400);
  
  Serial.println("Move servo left");
  OCR2B = 40;
  ultDistanceLeft = checkDistance();
  Serial.print("Distance on left: ");
  Serial.println(ultDistanceLeft);
  if(ultDistanceLeft > 0 && ultDistanceLeft < 12) {
    PORTB |= (1 << BLUE_LED);
  } else if (ultDistanceLeft != -1) {
    PORTB &= ~(1 << BLUE_LED);
  } else {}
  _delay_ms(400);
}

void servoOneDirection() {
  OCR2B = 40;
  ultDistanceLeft = checkDistance();
  if(ultDistanceLeft > 0 && ultDistanceLeft < 12) {
    PORTB |= (1 << BLUE_LED);
  } else if ((ultDistanceLeft < 0 && ultDistanceLeft != -1) || ultDistanceLeft >= 12) {
    PORTB &= ~(1 << BLUE_LED);
  } else {}
}

void danceRobot() {
  
  robotForward(200, 200);
  _delay_ms(1500);

  robotStop();
  _delay_ms(500);

  robotBackwards(200, 200);
  _delay_ms(1500);

  robotStop();
  _delay_ms(500);

  turnLeft(100, 100);
  _delay_ms(300);

  robotStop();
  _delay_ms(400);

  turnRight(100, 100);
  _delay_ms(500);

  robotStop();
  _delay_ms(500);

  turnLeft(100, 100);
  _delay_ms(800);

  robotStop();
  _delay_ms(500);

  turnRight(100, 100);
  _delay_ms(1000);

  robotStop();
  _delay_ms(500);
}

void robotForward(int OCR0Ain, int OCR0Bin) {
  PORTB |= (1 << IN1); //HIGH
  PORTB &= ~(1 << IN2); //LOW
  PORTD |= (1 << IN3); //HIGH
  PORTD &= ~(1 << IN4); //LOW
  PORTD |= (1 << ENABLE_A); //enable motor A
  PORTD |= (1 << ENABLE_B); //enable motor B
  OCR0A = OCR0Ain;
  OCR0B = OCR0Bin;
}

void robotBackwards(int OCR0Ain, int OCR0Bin) {
  PORTB &= ~(1 << IN1); //LOW
  PORTB |= (1 << IN2); //HIGH
  PORTD &= ~(1 << IN3); //LOW
  PORTD |= (1 << IN4); //HIGH
  PORTD |= (1 << ENABLE_A);
  PORTD |= (1 << ENABLE_B);
  OCR0A = OCR0Ain;
  OCR0B = OCR0Bin;
}

void turnLeft(int OCR0Ain, int OCR0Bin) {
  PORTB &= ~(1 << IN1); //LOW
  PORTB |= (1 << IN2); //HIGH
  PORTD |= (1 << IN3); //HIGH
  PORTD &= ~(1 << IN4); //LOW
  PORTD |= (1 << ENABLE_A);
  PORTD |= (1 << ENABLE_B);
  OCR0A = OCR0Ain;
  OCR0B = OCR0Bin;
}

void turnRight(int OCR0Ain, int OCR0Bin) {
  PORTB |= (1 << IN1); //HIGH
  PORTB &= ~(1 << IN2); //LOW
  PORTD &= ~(1 << IN3); //LOW
  PORTD |= (1 << IN4); //HIGH
  PORTD |= (1 << ENABLE_A);
  PORTD |= (1 << ENABLE_B);
  OCR0A = OCR0Ain;
  OCR0B = OCR0Bin;
}

void robotStop() {
  PORTD &= ~(1 << ENABLE_A);
  PORTD &= ~(1 << ENABLE_B);
  OCR0A = 0;
  OCR0B = 0;
}

void wallFollowing() {
  ultDistanceLeft = checkDistance();
  _delay_ms(10);
  
  if((ultDistanceLeftPrev - ultDistanceLeft) > 2) { //check to see if there is a rogue value, robot cannot teleport to the right >2 inch
    distanceJumped = true;
  } else {
    distanceJumped = false;
  }
  
  if (ultDistanceLeft >= 10 && ultDistanceLeft < 18 && !distanceJumped) { //too far from wall
    robotForward(220, 179);
  } else if (ultDistanceLeft <= 5.5 && ultDistanceLeft >= 1.5 && !distanceJumped) { //too close to wall
    robotForward(179, 220);
  } else if (ultDistanceLeft < 10 && ultDistanceLeft > 5.5 && ultDistanceLeft != -1 && !distanceJumped) {
    robotForward(185, 185);
  } else if (ultDistanceLeft < 1.5 && ultDistanceLeft != -1) { //robot hit wall
    robotStop();
    while(1) {}
  } else {
    ultDistanceLeftPrev = ultDistanceLeft;
    return;
  }
  ultDistanceLeftPrev = ultDistanceLeft;
}

void setup() {
  Serial.begin(9600);
  DDRB |= (1 << RED_LED);
  DDRB |= (1 << GREEN_LED);
  DDRB |= (1 << BLUE_LED);

  DDRD |= (1 << DDD3); //set OCR2B as output for servo

  //setup timer 2 for servo
  TCCR2A = ((1 << COM2B1) | (1 << WGM21) | (1 << WGM20)); //Fast PWM, non-inverting, TOP at OCR2A
  TCCR2B = ((1 << WGM22) | (1 << CS22) | (1 << CS21) | (1 << CS20)); //1024 prescaler
  TCNT2 = 0;
  OCR2A = 255; //16ms pulse width, max for timer 2
  OCR2B = 25; //middle to start

  //setup pins for ultrasonic sensor data control
  DDRC |= (1 << DDC1); //TRIG_PIN set to output
  DDRB &= ~(1 << DDB0); //ECHO_PIN set to input

  //setup timer 1 for ultrasonic sensor
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << ICES1) | (1 << ICNC1); //prescaler 1024, capture rising edge, noise canceler
  TCCR1C = 0;
  OCR1A = 781; // Set TOP value to 50ms

  //setup timer 0 for motors
  TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1); //fast PWM, non-inverting, TOP at 0xFF
  TCCR0B |= (1 << CS01); //prescaler 8

  //setup pins for DC motor control
  DDRD |= (1 << DDD5) | (1 << DDD6) | (1 << DDD7) | (1 << DDD4); //ENABLE_A, ENABLE_B, IN4, IN3
  DDRB |= (1 << DDB2) | (1 << DDB1); //IN1, IN2

  //wall is to the left
  OCR2B = 37; //OCR2B = 14 for wall on the right
  //startup motors for wall following
  robotForward(185, 185);
  _delay_ms(1000);
}

void loop() {
  //pwmServo();
  //servoOneDirection();
  //danceRobot();
  //_delay_ms(1000);
  wallFollowing();
}
