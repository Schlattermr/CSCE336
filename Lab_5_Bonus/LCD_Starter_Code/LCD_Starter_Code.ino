/*--------------------------------------------------------------------
Name:   Matthew Schlatter
Date:   May 9 2024
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   LCD_Starter_Code.cpp
HW/Lab: Bonus Lab #5

Purp: Demonstrates the use of Adafruit 1.8" (128x160 pixels) TFT Display 
      using the Adafruit GFX and ST7735 libraries to be used to make an
      Etch-a-Sketch and Pong Game. 

Doc:  Used seesaw_shield18_test.ino to get initialization values, 
      and uses the Adafruit GFX, Adafruit ST7735, and Adafruit
      BusIO Libraries

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/

#include <Arduino.h>         // jlf added for vs_code
#include <Wire.h>            // jlf added for vs_code
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include <avr/wdt.h>
#include "pong.h"

#define TFT_CS    10
#define TFT_RST   9 
#define TFT_DC    8
#define VRx_CHANNEL A0
#define VRy_CHANNEL A1
#define SW_PIN A2

// Create class named tft of type Adafruit_ST7735
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Function Prototypes 
void Display_Menu(void);

// Setup for LCD Display
uint16_t TFT_height = tft.width();  //128 
uint16_t TFT_width = tft.height();  //160
uint16_t color = ST77XX_RED;
uint16_t box_width = 4;
uint16_t box_height = 4;
ball_t pongBall = createBall(80, 64, 3, 3, 4);
paddle myPaddle = createPaddle(155, 90, 0, 30, 5);

void setup() {
  Serial.begin(9600);
  Serial.println("Welcome to CSCE 336 LCD Starter Code!");

  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab

  Serial.println("Display Initialized");

  // Display Welcome Screen
  tft.setTextWrap(false);
  tft.setRotation(1); // Rotate view 90 degrees
  Display_Menu();

  // ADC setup
  PRR &= ~(1 << PRADC);
  ADMUX = (1 << REFS0); // set voltage reference to AVcc
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // enable ADC, division factor 128

  // set joystick pins as input
  DDRC &= ~(1 << DDC0); // VRx
  DDRC &= ~(1 << DDC1); // VRy
  DDRC &= ~(1 << DDC2); // SW
  // set button with pullup resistor
  PORTC |= (1 << PORTC2);
}

void loop() {
  int16_t vryValue = readAnalogInput(1);

  // clear last positions
  tft.fillCircle(pongBall.position.x, pongBall.position.y, pongBall.radius, ST77XX_WHITE);
  tft.fillRect(myPaddle.position.x, myPaddle.position.y, myPaddle.width, myPaddle.height, ST77XX_WHITE);

  if(vryValue > 612) {
    myPaddle = createPaddle(myPaddle.position.x, myPaddle.position.y, -2, myPaddle.height, myPaddle.width);
  } else if (vryValue < 412) {
    myPaddle = createPaddle(myPaddle.position.x, myPaddle.position.y, 2, myPaddle.height, myPaddle.width);
  } else {
    myPaddle = createPaddle(myPaddle.position.x, myPaddle.position.y, 0, myPaddle.height, myPaddle.width);
  }
  pongBall = moveBall(pongBall, myPaddle);
  myPaddle = movePaddle(myPaddle);

  if(pongBall.velocity.x == 0 && pongBall.velocity.y == 0) {
    gameOver();
  }

  // draw new positions
  tft.fillCircle(pongBall.position.x, pongBall.position.y, pongBall.radius, ST77XX_RED);
  tft.fillRect(myPaddle.position.x, myPaddle.position.y, myPaddle.width, myPaddle.height, ST77XX_RED);
  delay(20);
  
//  etch-a-sketch code
//  int16_t vrxValue = readAnalogInput(0);
//  int16_t vryValue = readAnalogInput(1);
//  int16_t swState = PINC & (1 << PINC2);
//  
//  // Check joystick direction
//  if (vrxValue < 412) {
//    Serial.println("Joystick pushed Right");
//    TFT_width += 8;
//  } else if (vrxValue > 612) {
//    Serial.println("Joystick pushed Left");
//    TFT_width -= 8;
//  } else if (vryValue < 412) {
//    Serial.println("Joystick pushed Down");
//    TFT_height += 8;
//  } else if (vryValue > 612) {
//    Serial.println("Joystick pushed Up");
//    TFT_height -= 8;
//  } else if(swState == LOW) {
//    Serial.println("Button pressed");
//    color = ST77XX_WHITE;
//  } else {
//    Serial.println("Joystick released");
//    color = ST77XX_BLUE;
//  }
//
//  tft.fillRect(TFT_width/2, TFT_height/2, box_width, box_height, color);
//  delay(200);
}

void Display_Menu(void){
  tft.fillScreen(ST77XX_WHITE);
  tft.setCursor(0, 20);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(3);
  tft.println("   PONG");
  delay(1000);
  tft.fillScreen(ST77XX_WHITE);
  return;
}

void gameOver() {
  tft.fillScreen(ST77XX_WHITE);
  tft.setCursor(0, 20);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(3);
  tft.println("GAME OVER");
  tft.println("YOU LOSED");
  tft.println(" ");
  tft.setTextSize(2);
  tft.println("  Go Big Red!");
  while(true){
    int16_t swState = PINC & (1 << PINC2);
    if(swState == LOW) { // restart the game when the button is pressed
      tft.fillScreen(ST77XX_WHITE);
      pongBall = createBall(80, 64, 3, 3, 4);
      myPaddle = createPaddle(155, 90, 0, 30, 5);
      return;
    }
  }
}
