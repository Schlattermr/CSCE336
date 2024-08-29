/*
 * CSCE236 Embedded Systems Spring 2024
 * HW4
 */

const int blueLed = 7;
const int button = 5;

//These are the functions that we know exist in obfs.cpp
void startOne();
void startTwo();
void startThree();
void startFour();
void setMem(char i);
void runLoop();

void setup() { 
  Serial.begin(9600);  
  pinMode(blueLed, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  digitalWrite(blueLed, HIGH);
  delay(1000);
  digitalWrite(blueLed, LOW);
  delay(1000);
                  
  Serial.println("Starting up...");
  
  //This order does not work to start it up
  //startThree();
  //startTwo();
  //startFour();
  //startOne();

  digitalWrite(blueLed, HIGH);
  delay(500);
  digitalWrite(blueLed, LOW);

  //Write _YOUR_ initials to memory and determine where they are put
  //Note you should only do one at a time since they may overwrite each other 
  setMem('m');
  setMem('s');

 //The serial port gets messed up, so this won't print
 Serial.println("Finished setup");
}

void loop() {
  //Figure out how long this function takes to run with
  //and without the button pressed.
  unsigned long startTime = 0;
  unsigned long elapsedTime = 0;

  if (digitalRead(button) == HIGH) {
    startTime = millis();
    runLoop();
    elapsedTime = millis() - startTime; 
    Serial.println("Time taken when button is unpressed: " + String(elapsedTime) + " milliseconds");
  } else {
    startTime = millis();
    runLoop();
    elapsedTime = millis() - startTime;
    Serial.println("Time taken when button is pressed: " + String(elapsedTime) + " milliseconds");    
  }
}
