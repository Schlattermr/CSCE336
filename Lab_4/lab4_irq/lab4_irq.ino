/*--------------------------------------------------------------------
Name:   Matthew Schlatter and Caleb Poggemeyer
Date:   4/2/24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   lab4_irq.ino
HW/Lab: Lab 4, Real Time Events

Purp: program for Lab 4 to illustrate Real Time Events 

Doc:  class materials

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/

#include <Arduino.h>
#include <util/delay.h>

//LED pin 9 or Port B1
#define LED_PIN PINB1

//Pin the button is on Pin 3 or Port D3
#define BUTTON PIND3

/**
 * Setups up the timer interrupt to happen every 100 milliseconds.
 **/
void setupTimerInterrupt(){
   //Reset to defaults
  TCCR1A = TCCR1B = 0;
  // CTC with OCR1A as top
  TCCR1B |= (1 << WGM12);
  //Set clock to clk/1024
  TCCR1B |= (1 << CS12) | (1 << CS10);
  //Enable interrupt on match with OCR1A
  TIMSK1 = (1 << OCIE1A);
  
  //LAB CODE -- FILL IN
  //Configure OCR1A (the counter top) to generate an interrupt
  //every 500ms.
  OCR1A = 7813;
}

void toggleLED(){
  static uint8_t on = 0;
  if(on){
    PORTB |= 1 << LED_PIN; /* Turn led on Pin 9 */
  }else{
    PORTB &= ~(1 << LED_PIN); /* Turn led off Pin 9 */
  }
  on = !on;
}

/**
 * This function is called whenever the timer interrupt match 1A
 * is generated.
 **/
ISR(TIMER1_COMPA_vect){
  //LAB CODE
  //UNCOMMENT THIS LINE TO TOGGLE ON INTERRUPT
  toggleLED(); 
}

//  ISR(INT1_vect) {
//  PORTB |= (1 << LED_PIN);
//}

void setup() {
  Serial.begin(9600);
  Serial.print("Starting up...");

  //Setup the LED
  DDRB |= (1 << LED_PIN); /* Set LED as output */
  PORTB &= ~(1 << LED_PIN); /* Turn led off Pin 9 */
  
  //Setup the button
  DDRD &= ~(1 << BUTTON); /* Input Button on Pin 3 */
  PORTD |= (1 << BUTTON); /* Enable pullup on Pin 3 */
  
  Serial.println("startup complete.");
  setupTimerInterrupt();  
}

void loop() {
  //static unsigned long lastTimeMS = 0;
  //unsigned long currentTimeMS = 0;
  //Manually toggle the LED every 1/2 seconds
  //Comment this out when enabling the interrupt version
  //currentTimeMS = millis();
  //if((currentTimeMS - lastTimeMS) >= 500){
  //  Serial.println(currentTimeMS - lastTimeMS);
  //  toggleLED();
  //  lastTimeMS = currentTimeMS;
  //}

  //cli and sei
  //if(PIND & (1 << BUTTON)){
  //  PORTB &= ~(1 << LED_PIN);
  //  cli(); //turns iterrupts off
  //} else {
  //  sei(); //turns interrupts on
  //}

  //EICRA = 0; ////low level of INT1 generates an interrupt request
  //EIMSK |= (1 << INT1);

  //if the button is not pressed, keep LED off
  if(PIND & (1 << BUTTON)){
    PORTB &= ~(1 << LED_PIN);
    _delay_us(2);
  }

  //If the button is pressed, print the ADC values
  if(!(PIND & (1 << BUTTON))){
    Serial.println("--------------------------------------------");
    Serial.println("--------------------------------------------");
    Serial.println("--------------------------------------------");
    Serial.print("The reading of analog port A0 is value: "); Serial.println(analogRead(A0));
    Serial.print("The reading of analog port A1 is value: "); Serial.println(analogRead(A1));
    Serial.print("The reading of analog port A2 is value: "); Serial.println(analogRead(A2));
    Serial.print("The reading of analog port A3 is value: "); Serial.println(analogRead(A3));
    Serial.print("The reading of analog port A4 is value: "); Serial.println(analogRead(A4));
    Serial.print("The reading of analog port A5 is value: "); Serial.println(analogRead(A5));
    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    for(uint8_t i = 0; i < 20; i++){
        Serial.print("The reading of digital pin ");
        Serial.print(i);
        Serial.print(" has a value of: ");
        Serial.println(digitalRead(i));
    }
  }
}
