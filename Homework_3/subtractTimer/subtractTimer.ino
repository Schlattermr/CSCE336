/*--------------------------------------------------------------------
Name:   Matthew Schlatter
Date:   2/22/24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   subtractTimer.ino
HW/Lab: Homework 3

Purp: 

Doc:  homework 3 intro video, "Use verbose output in the Arduino IDE"
      article on Arduino website, 

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/

volatile int8_t result8 = 0;
volatile int16_t result16 = 0;
volatile int32_t result32 = 0;
volatile float result_f = 0;

void setup() {
}

void loop() {

  int8_t a, b;
  int16_t c, d;
  int32_t e, f;
  float g, h;

  a = (int8_t) (rand() & 0xFF);
  b = (int8_t) (rand() & 0xFF);
  asm volatile("nop");
  result8 = a - b;
  asm volatile("nop");
  result8 = a / b;

  c = (int16_t) (rand() & 0xFFFF);
  d = (int16_t) (rand() & 0xFFFF);
  asm volatile("nop");
  result16 = c - d;
  asm volatile("nop");
  result16 = c / d;

  e = (int32_t) (rand() & 0xFFFFFFFF);
  f = (int32_t) (rand() & 0xFFFFFFFF);
  asm volatile("nop");
  result32 = e - f;
  asm volatile("nop");
  result32 = e / f;

  g = (float) (rand());
  h = (float) (rand());
  asm volatile("nop");
  result_f = g - h;
  asm volatile("nop");
  result_f = g / h;
  
}
