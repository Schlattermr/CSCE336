/*--------------------------------------------------------------------
Name: Matthew Schlatter
Date: 2/12/2024
Course: CSCE 336 Embedded Systems (Spring 2024)
File: SerialRead.ino
HW/Lab: Homework 2

Purp:   Blinks LED for the number of times that is specified by user 
        in the serial monitor, and blinks fast for f and slow for s.

Doc:    Serial.read() starter code from arduino website

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor 
code. I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/
int incomingByte = 0;
int numOfBlinks = 0;
int blinksCompleted = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    
    incomingByte = Serial.read(); 
    blinksCompleted = 0;

    if((incomingByte - '0') >= 0) { //convert ASCII to int
      Serial.print("I received: ");
      if((char)incomingByte == 's' || (char)incomingByte == 'f' || ((incomingByte - '0') >= 0 && (incomingByte - '0') <= 9)) { //all inputs that will do something
        Serial.println((char)incomingByte);
      } else { //other inputs that would not do anything
        Serial.print((char)incomingByte);
        Serial.println(" (no operation for input)");
      }
      
    }

    if(incomingByte >= '0' && incomingByte <= '9') {
      numOfBlinks = incomingByte - '0'; 
      for(int i = 0; i < numOfBlinks; i++) {
        digitalWrite(LED_BUILTIN, HIGH); 
        delay(600);
        digitalWrite(LED_BUILTIN, LOW); 
        delay(600); 
        blinksCompleted++;
      }
    } else if (incomingByte == 's') { //s for a slower blink
      for(int i = 0; i < numOfBlinks; i++) {
        digitalWrite(LED_BUILTIN, HIGH); 
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW); 
        delay(1000); 
        blinksCompleted++;
      }
    } else if (incomingByte == 'f') { //f for a faster blink
      for(int i = 0; i < numOfBlinks; i++) {
        digitalWrite(LED_BUILTIN, HIGH); 
        delay(250);
        digitalWrite(LED_BUILTIN, LOW); 
        delay(250); 
        blinksCompleted++;
      }
    } else {
        digitalWrite(LED_BUILTIN, LOW); 
    }

    //so it doesn't keep looping and keep constantly blinking
    if(blinksCompleted >= numOfBlinks) {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
