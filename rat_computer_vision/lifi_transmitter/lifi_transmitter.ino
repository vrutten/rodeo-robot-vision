/*
* First RF sketch using nRF24L01+ radios and Arduino (Uno or Micro)
*/

#include <SPI.h>    // Serial Peripheral Interface bus library
#include "RF24.h"   // Library for nRF24L01s

/****************** User Config ***************************/

/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus pins and pins 7 & 8 */

// RF24 radio(7,8);  // For Arduino UNO
RF24 radio(2, 3);  // For Arduino Micro

/* Create a binary address for read/write pipes */
byte addresses[][6] = {"1Node", "2Node"};


bool role = 1; // Local state variable that controls whether this node is sending 1 or receiving 0

void setup() {
//  radio.powerDown();
  Serial.begin(115200);
  radio.begin();

  // Set the Power Level low
  radio.setPALevel(RF24_PA_LOW);

  // Set radio rate and channel
  radio.setDataRate(RF24_250KBPS);

  // Set the channel (Frequency): 108 = 2.508 GHz (above WiFi)
  radio.setChannel(100);

  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

  // Pause briefly to let everything "settle"
  delay(1000);

  // Stop the radio listening for data
  radio.stopListening();      
  radio.setPayloadSize(8);

  if (!radio.write(0, sizeof(byte))) {
    Serial.println("failed");
  }
  else {
    Serial.println("sent");
  }

}

  byte transmitted_signal;
  int start = 1;


void loop() {
    delay(100);
  /****************** Transmitting Role ***************************/
                              // First, stop listening so we can talk.

  

  char inByte = 'c';
  if (Serial.available()) { // only send data back if data has been sent
    char inByte = Serial.read();
    if (inByte == 'w') {
       if (transmitted_signal == 1) {
          return;
      }
      transmitted_signal = 1; //forward
    } else if (inByte == 'a') {
       if (transmitted_signal == 3) {
         return;
      }
      transmitted_signal = 3; //left
    } else if (inByte == 's') {
       if (transmitted_signal == 2) {
        return;
      }
      transmitted_signal = 2; //back
    } else if (inByte == 'd') {
        if (transmitted_signal == 4) {
          return;
      }
      transmitted_signal = 4; //right
    } else if (inByte == 'c') {
        if (transmitted_signal == 0) {
          return;
      }
      transmitted_signal = 0;
    }
  }  else if (start != 1) {
    return;
  }
  Serial.println(transmitted_signal);
  
  radio.stopListening();  
  Serial.println(F("Now sending"));
  if (!radio.write(&transmitted_signal, sizeof(byte))) {
    Serial.println("failed");
  }
  else {
    Serial.println("sent");
  }

  start = 0;
  // Try again 100 ms later



}

// FIN

