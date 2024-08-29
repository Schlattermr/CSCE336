/****************************************************************************
*
*   Copyright (c) 2011 Carrick Detweiler
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*
****************************************************************************/


#include <avr/io.h>
#include <Arduino.h>

/**
 * Define a dot as a zero and dash as a 1
 **/
#define DOT 0
#define DASH 1

#define PINB_Reg (*((volatile uint8_t *) 0x23))
#define DDRB_Reg (*((volatile uint8_t *) 0x24))
#define PORTB_Reg (*((volatile uint8_t *) 0x25))
#define PIND_Reg (*((volatile uint8_t *) 0x29))
#define DDRD_Reg (*((volatile uint8_t *) 0x2a))
#define PORTD_Reg (*((volatile uint8_t *) 0x2b))
#define BUTTON 5

int buttonState = HIGH;
int tempButtonState = HIGH;
//measure time of press
unsigned long startTimeLow = 0;
unsigned long endTimeLow = 0;
unsigned long totalTimeLow = 0;
//measure time not pressed
unsigned long startTimeHigh = 0;
unsigned long endTimeHigh = 0;
unsigned long totalTimeHigh = 0;

String morseCharacter = "";

/**
 * These macros convert dots and dashes into a binary representation.
 * The upper three bits indicate the number of dots and dashes.  The
 * other bits contain the dots (0) and dashes (1).  The LSB is the
 * first dot/dash to send and so forth. 
 **/
#define MORSE_IN1(a)           ((1<<5) | (a<<0))
#define MORSE_IN2(a,b)         ((2<<5) | (a<<0) | (b<<1))
#define MORSE_IN3(a,b,c)       ((3<<5) | (a<<0) | (b<<1) | (c<<2))
#define MORSE_IN4(a,b,c,d)     ((4<<5) | (a<<0) | (b<<1) | (c<<2) | (d<<3))
#define MORSE_IN5(a,b,c,d,e)   ((5<<5) | (a<<0) | (b<<1) | (c<<2) | (d<<3) | (e<<4))

/**
 * This is a table of all of the morse code strings for a-z.  If you
 * are passed a letter c you can access it in here by using
 * morseTable[c-'a'].  The upper three bits indicate the number of dots
 * and dashes in the code for that character.  The LSB is the first
 * dot/dash, next bit is the second, etc.
 **/
uint8_t morseTable[26] =
  {MORSE_IN2(DOT,DASH), //A
   MORSE_IN4(DASH,DOT,DOT,DOT), //B
   MORSE_IN4(DASH,DOT,DASH,DOT), //C
   MORSE_IN3(DASH,DOT,DOT), //D
   MORSE_IN1(DOT), //E
   MORSE_IN4(DOT,DOT,DASH,DOT), //F
   MORSE_IN3(DASH,DASH,DOT), //G
   MORSE_IN4(DOT,DOT,DOT,DOT), //H
   MORSE_IN2(DOT,DOT), //I
   MORSE_IN4(DOT,DASH,DASH,DASH), //J
   MORSE_IN3(DASH,DOT,DASH), //K
   MORSE_IN4(DOT,DASH,DOT,DOT), //L
   MORSE_IN2(DASH,DASH), //M
   MORSE_IN2(DASH,DOT), //N
   MORSE_IN3(DASH,DASH,DASH), //O
   MORSE_IN4(DOT,DASH,DASH,DOT), //P
   MORSE_IN4(DASH,DASH,DOT,DASH), //Q
   MORSE_IN3(DOT,DASH,DOT), //R
   MORSE_IN3(DOT,DOT,DOT), //S
   MORSE_IN1(DASH), //T
   MORSE_IN3(DOT,DOT,DASH), //U
   MORSE_IN4(DOT,DOT,DOT,DASH), //V
   MORSE_IN3(DOT,DASH,DASH), //W
   MORSE_IN4(DASH,DOT,DOT,DASH), //X
   MORSE_IN4(DASH,DOT,DASH,DASH), //Y
   MORSE_IN4(DASH,DASH,DOT,DOT)}; //Z

/**
 * Init all of the LEDs and test them
 **/ 
void LEDInit(void){
 //Set pinmode for LEDs to output 
  DDRB_Reg |= (1 << RED_LED);
  DDRB_Reg |= (1 << GREEN_LED);
  DDRB_Reg |= (1 << BLUE_LED);

  //Turn all off
  PORTB_Reg &= ~(1 << RED_LED);   //clear output
  PORTB_Reg &= ~(1 << GREEN_LED); //clear output
  PORTB_Reg &= ~(1 << BLUE_LED);  //clear output

  //Test LEDs
  Serial.println("Testing LEDs...");
  PORTB_Reg |= (1 << RED_LED);    //set output
  delay(400);
  PORTB_Reg &= ~(1 << RED_LED);   //clear output
  PORTB_Reg |= (1 << GREEN_LED);  //set output
  delay(400);
  PORTB_Reg &= ~(1 << GREEN_LED); //clear output
  PORTB_Reg |= (1 << BLUE_LED);   //set output
  delay(400);
  PORTB_Reg &= ~(1 << BLUE_LED);   //clear output
  Serial.println("Finished LED testing!");
}

/**
 * Sends a long blink (a dash) on the specified led.
 **/
void morseBlinkLong(uint8_t led){
  PORTB_Reg |= (1 << led); //on
  Serial.println("D");
  delay(1000);
  PORTB_Reg &= ~(1 << led); //off
}

/**
 * Sends a short blink (a dot) on the specified led.
 **/
void morseBlinkShort(uint8_t led){
  PORTB_Reg |= (1 << led);
  Serial.println("d");
  delay(500);
  PORTB_Reg &= ~(1 << led);
}

/**
 * Perform a long pause (without turning on the LEDs), which should be
 * about the length of a dash.
 **/
void morseLongPause(void){
  Serial.println("s");
  delay(1000);
}

/**
 * Perform a short pause (without turning on the LEDs), which should
 * be about the length of a dot.
 **/
void morseShortPause(void){
  delay(500);
}

/**
 * Sends the right blink sequence for character c on the specified
 * led.  Note that the valid range for c is 'a'-'z' or 'A'-'Z'.
 **/
void morseBlinkChar(uint8_t led, char c){
  uint8_t i;

  //First make sure it is a valid character
  if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))){
    //Not valid
    return;
  }

  //If it is upper case, convert to lower case
  if(c >= 'A' && c <= 'Z'){
    c = c - 'A';
    c = c + 'a';
  }

  //Get the number of symbols in the morse code for c
  uint8_t numSymbols = morseTable[c-'a'] >> 5;
  //Now send each character
  for(i = 0; i < numSymbols; i++){
    //Send the right length pulse depending on if the i'th bit is a 1 or 0
    if(((morseTable[c-'a']>>i) & 0x1) == DOT){ 
      morseBlinkShort(led); 
    } else { 
      morseBlinkLong(led); 
    } //DASH
    //Do a short pause, needed between symbols, ok for between
    //characters (but not long enough for between characters)
    morseShortPause();
  }
}

/**
 * Sends the specified string (with len characters) as morse code
 * blinks over the specified LED.  Note that the valid range for c is
 * 'a'-'z' or 'A'-'Z'.
 **/
void morseBlinkString(uint8_t led, char *c, uint8_t len){
  for(int i = 0; i < len; i++) {
    morseBlinkChar(led, c[i]);
    morseLongPause();
  }
}

/**
 * Blinks "Hi" on startup of board, with H blinking on the green
 * LED and i blinking on the red LED.
 */
void morseHi(void) {
  morseBlinkChar(GREEN_LED, 'H');
  morseLongPause();
  morseBlinkChar(RED_LED, 'i');
  morseLongPause();
}

/**
 * Determines length of button press and if it is a dash, 
 * dot, or long pause
 */
String determinePress(void) {
  int currentButtonState = digitalRead(BUTTON);

  //debounce
  if (currentButtonState != buttonState) {
    delay(10);
    currentButtonState = digitalRead(BUTTON);
    
    if (currentButtonState != buttonState) {
      buttonState = currentButtonState;
    }
  }

  //start measuring high button
  if(buttonState == HIGH && tempButtonState == HIGH) {
    startTimeHigh = millis();
  }

  //button pressed
  if(buttonState == LOW && tempButtonState == HIGH) {
    endTimeHigh = millis();
    totalTimeHigh = endTimeHigh - startTimeHigh;
    startTimeLow = millis();
    tempButtonState = buttonState;
  } 

  //button released
  if(buttonState == HIGH && tempButtonState == LOW) {
    endTimeLow = millis();
    totalTimeLow = endTimeLow - startTimeLow;
    tempButtonState = buttonState;
  }

  if(totalTimeLow >= 1000) {
    Serial.println("D");
    morseCharacter += "D";
  } else if (totalTimeLow < 1000 && totalTimeLow > 0) {
    Serial.println("d");
    morseCharacter += "d";
  } else if (totalTimeHigh >= 1000) {
    Serial.println("s");
    morseCharacter += "s";
  }

  totalTimeLow = 0;
  totalTimeHigh = 0;
  //check for long pause to end and return full morse character code
  if((morseCharacter[morseCharacter.length() - 1]) == 's') {
    return morseCharacter;
  } else {
    return "";
  }
}

/**
 * Turns r, g, and b LEDs on if input is an r, g, or b respectively,
 * and returns for all other inputs
 */
void rgbInput(String &morseCharacter) {
  
  if(morseCharacter == "dDds") { //red in morse
    digitalWrite(RED_LED, HIGH);
    delay(1000);
    digitalWrite(RED_LED, LOW);
    morseCharacter = ""; //reset for next reading
  } else if(morseCharacter == "DDds") { //green in morse
    digitalWrite(GREEN_LED, HIGH);
    delay(1000);
    digitalWrite(GREEN_LED, LOW);
    morseCharacter = "";
  } else if(morseCharacter == "Dddds") { //blue in morse
    digitalWrite(BLUE_LED, HIGH);
    delay(1000);
    digitalWrite(BLUE_LED, LOW);
    morseCharacter = "";
  } else {
    return;
  }
}