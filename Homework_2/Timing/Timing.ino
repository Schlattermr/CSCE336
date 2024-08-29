/*--------------------------------------------------------------------
Name: Matthew Schlatter
Date: 2/12/2024
Course: CSCE 336 Embedded Systems (Spring 2024)
File: Timing.ino
HW/Lab: Homework 2

Purp:   Measures time of different counters on Atmega 328P processor.
        Measures time on 8-bit, 16-bit, and 32-bit counters

Doc:    millis() and micros() Arduino websites

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor 
code. I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/
uint8_t counter8 = 0xff;
uint16_t counter16 = 0xffff;
uint32_t counter32 = 0xffffff;
unsigned long startTime = 0;
unsigned long endTime = 0;

void eightBitTimer() {
  digitalWrite(LED_BUILTIN, HIGH);
  
  startTime = micros();
  
  for(counter8 = 0; counter8 < 255; counter8++){ 
    asm volatile("nop"); 
  }

  endTime = micros();

  digitalWrite(LED_BUILTIN, LOW);

  Serial.print("The 8 bit counter took ");
  Serial.print(endTime - startTime);
  Serial.println(" microseconds");
}

void sixteenBitTimer() {
  digitalWrite(LED_BUILTIN, HIGH);
  
  startTime = millis();
  
  for(counter16 = 0; counter16 < 65535; counter16++){ 
    asm volatile("nop");
  }

  endTime = millis();

  digitalWrite(LED_BUILTIN, LOW);

  Serial.print("The 16 bit counter took ");
  Serial.print(endTime - startTime);
  Serial.println(" milliseconds");
}

void thirtyTwoBitTimer() {
  digitalWrite(LED_BUILTIN, HIGH);
  
  startTime = millis();
  
  for(counter32 = 0; counter32 < 16777215; counter32++){ 
    asm volatile("nop");
  }

  endTime = millis();

  digitalWrite(LED_BUILTIN, LOW);

  Serial.print("The 32 bit counter took ");
  Serial.print(endTime - startTime);
  Serial.println(" milliseconds");
}

void sixThreeTwoSeconds() {
  digitalWrite(LED_BUILTIN, HIGH);

  startTime = millis();
  
  for(counter32 = 0; counter32 < 8388608; counter32++){ 
    asm volatile("nop");
  }

  endTime = millis();

  //flash (off-on-off)
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.print("The 32 bit counter took ");
  Serial.print(endTime - startTime);
  Serial.println(" milliseconds");
  delay(1000);

  digitalWrite(LED_BUILTIN, HIGH);
  
  startTime = millis();

  //additional for loop to lengthen 16 bit counter so it isn't just 25ms
  for(int i = 0; i < 90; i++) {
    for(counter16 = 0; counter16 < 65535; counter16++){ 
      asm volatile("nop");
    }
  }
  
  endTime = millis();

  //flash (off-on-off)
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.print("The 16 bit counter took ");
  Serial.print(endTime - startTime);
  Serial.println(" milliseconds");
  delay(1000);

  digitalWrite(LED_BUILTIN, HIGH);
  
  startTime = millis();

  //additional for loop to lengthen 8 bit counter so it isn't just 100 microseconds
  for(int i = 0; i < 30000; i++) {
    for(counter8 = 0; counter8 < 255; counter8++){ 
      asm volatile("nop");
    }
  }
  
  endTime = millis();
  
  //flash (off-on-off)
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.print("The 8 bit counter took ");
  Serial.print(endTime - startTime);
  Serial.println(" milliseconds");
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  delay(2000);
  eightBitTimer();
  sixteenBitTimer();
  thirtyTwoBitTimer();
  delay(2000);
  sixThreeTwoSeconds();
  delay(10000);
}
