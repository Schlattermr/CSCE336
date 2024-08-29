#define BUTTON_AD0 PINC0
#define BUTTON_PIN8 PINB0
#define OCR1A_PWM PINB1
#define OCR1B_PWM PINB2

void setup() {
  Serial.begin(9600);
  DDRB |= (1 << OCR1B_PWM); //set OCR1B as output
  TCCR1A = ((1 << COM1B1) | (1 << WGM11) | (1 << WGM10)); //Fast PWM with OCR1A as top
                                                          //Non-inverting mode for OCR1B
  TCCR1B = ((1 << WGM13) | (1 << WGM12) | (1 << CS12) | (1 << CS10)); //1024 prescaler
  TCCR1C = 0;
  TCNT1 = 0;
  
  OCR1A = 313; //20ms pulse width
  OCR1B = 16; //left for 1ms
}

void loop() {
  Serial.println("Move servo right");
  OCR1B = 8;
  delay(500);
  Serial.println("Move servo middle");
  OCR1B = 22;
  delay(500);
  Serial.println("Move servo left");
  OCR1B = 37;
  delay(500);
  Serial.println("Move servo middle");
  OCR1B = 22;
  delay(500);
}
