/*--------------------------------------------------------------------
Name:   Matthew Schlatter
Date:   3/19/24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   Decryption.ino
HW/Lab: Homework 4

Purp: Decrypt different messages using XOR logical bitwise operation.

Doc:  online ASCII table

Academic Integrity Statement: I certify that, while others may have 
assisted me in brainstorming, debugging, and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual it is also a violation of the honor code.
--------------------------------------------------------------------*/
#include <Arduino.h>

byte encryptedMessage1[] = {0xef, 0xc3, 0xc2, 0xcb, 0xde, 0xcd, 0xd8, 0xd9, 0xc0, 0xcd, 0xd8, 0xc5, 0xc3,
                            0xc2, 0xdf, 0x8d, 0x8c, 0x8c, 0xf5, 0xc3, 0xd9, 0x8c, 0xc8, 0xc9, 0xcf, 0xde,
                            0xd5, 0xdc, 0xd8, 0xc9, 0xc8, 0x8c, 0xd8, 0xc4, 0xc9, 0x8c, 0xef, 0xff, 0xef,
                            0xe9, 0x9f, 0x9f, 0x9a, 0x8c, 0xc4, 0xc5, 0xc8, 0xc8, 0xc9, 0xc2, 0x8c, 0xc1,
                            0xc9, 0xdf, 0xdf, 0xcd, 0xcB, 0xc9, 0x8c, 0xcd, 0xc2, 0xc8, 0x8c, 0xcd, 0xCf,
                            0xc4, 0xc5, 0xc9, 0xda, 0xc9, 0xc8, 0x8c, 0xde, 0xc9, 0xdd, 0xd9, 0xc5, 0xde,
                            0xc9, 0xc8, 0x8c, 0xca, 0xd9, 0xc2, 0xcf, 0xd8, 0xc5, 0xc3, 0xc2, 0xcd, 0xc0,
                            0xc5, 0xd8, 0xd5, 0x8f};
byte key1[] = {0xac};
byte decryptedMessage1[sizeof(encryptedMessage1)];

byte encryptedMessage2[] = {0xf8, 0xb7, 0x46, 0x8c, 0xb2, 0x46, 0xdf, 0xac, 0x42, 0xcb, 0xba, 0x03, 0xc7,
                           0xba, 0x5a, 0x8c, 0xb3, 0x46, 0xc2, 0xb8, 0x57, 0xc4, 0xff, 0x4a, 0xdf, 0xff,
                           0x12, 0x9a, 0xff, 0x41, 0xc5, 0xab, 0x50, 0x82, 0xff, 0x03, 0xe5, 0xab, 0x03,
                           0xc3, 0xb1, 0x4f, 0xd5, 0xff, 0x40, 0xc3, 0xb1, 0x57, 0xcd, 0xb6, 0x4d, 0xdf,
                           0xff, 0x4f, 0xc9, 0xab, 0x57, 0xc9, 0xad, 0x50, 0x80, 0xff, 0x53, 0xc9, 0xad,
                           0x4a, 0xc3, 0xbb, 0x50, 0x80, 0xff, 0x42, 0xc2, 0xbb, 0x03, 0xdf, 0xaf, 0x42,
                           0xcf, 0xba, 0x50};
byte key2[3] = {0xac, 0xdf, 0x23};
byte decryptedMessage2[sizeof(encryptedMessage2)];

byte encryptedMessage3[] = {0x35, 0xdf, 0x00, 0xca, 0x5d, 0x9e, 0x3d, 0xdb, 0x12, 0xca, 0x5d, 0x9e, 0x32,
                            0xc8, 0x16, 0xcc, 0x12, 0xd9, 0x16, 0x90, 0x53, 0xf8, 0x01, 0xd7, 0x16, 0xd0,
                            0x17, 0xd2, 0x0a, 0x90, 0x53, 0xf9, 0x1c, 0xd1, 0x17, 0x90, 0x53, 0xf9, 0x1c,
                            0xd1, 0x17, 0x9e};
byte key3[2];
byte decryptedMessage3[sizeof(encryptedMessage3)];

byte decryptByte(byte encryptedByte, byte key) {
  return encryptedByte ^ key;
}

void decryptMessage(byte *encryptedMessage, byte *key, byte *decryptedMessage, int messageLength, int keyLength) {
  for(int i = 0; i < messageLength; i++) {
    if(keyLength == 1) {
      decryptedMessage[i] = decryptByte(encryptedMessage[i], key[0]);
    } else {
      decryptedMessage[i] = decryptByte(encryptedMessage[i], key[i % keyLength]);
    }
  }
}

void printDecryption(byte *decryptedMessage, int messageLength) {
  Serial.print("Decrypted message: ");
  for (int i = 0; i < messageLength; i++) {
    Serial.write(decryptedMessage[i]);
  }
  Serial.println("");
}

void guessKey(byte *key) {
  for(int i = 0; i < 255; i++) { // iterate over possible values for the first byte
    key[0] = i;
    for(int j = 0; j < 255; j++) { // iterate over possible values for the second byte
      key[1] = j;
      decryptMessage(encryptedMessage3, key, decryptedMessage3, sizeof(encryptedMessage3), 2);
      
      // Check if the decrypted message contains only letters a-z and A-Z, periods, or spaces
      bool is_printable = true;
      for (int k = 0; k < sizeof(decryptedMessage3); k++) {
        byte decryptedByte = decryptedMessage3[k];
        if (!(decryptedByte == 32 || decryptedByte == 46 ||  
           (decryptedByte >= 65 && decryptedByte <= 90) || 
           (decryptedByte >= 97 && decryptedByte <= 122))) {
          is_printable = false;
          break;
        }
      }
      if (is_printable) {
        printDecryption(decryptedMessage3, sizeof(decryptedMessage3));
        Serial.print("Key: ");
        Serial.print(key[0], HEX);
        Serial.print(", ");
        Serial.println(key[1], HEX);
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  
  decryptMessage(encryptedMessage1, key1, decryptedMessage1, sizeof(encryptedMessage1), sizeof(key1));
  printDecryption(decryptedMessage1, sizeof(decryptedMessage1));

  decryptMessage(encryptedMessage2, key2, decryptedMessage2, sizeof(encryptedMessage2), sizeof(key2));
  printDecryption(decryptedMessage2, sizeof(decryptedMessage2));
  
  guessKey(key3);
}

void loop() {
}
