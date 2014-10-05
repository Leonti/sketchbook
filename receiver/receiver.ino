#include <SPI.h> 
#include "nRF24L01.h"
#include "RF24.h"

#define RF_SETUP 0x17

// Set up nRF24L01 radio on SPI pin for CE, CSN
RF24 radio(8,9);

int relay = 4;

const uint64_t id = 0xF0F0F0F0E4LL;

void setup(void) {
  
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);

  radio.begin();
   
  // Enable this seems to work better
  radio.enableDynamicPayloads();
  // RF24_1MBPS RF24_250KBPS
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(76);
  radio.setRetries(15,15);
  
  radio.openReadingPipe(1, id);

  radio.startListening();
  
  delay(1000);  
}

void loop(void) {

  while(radio.available()) {
    uint8_t len = radio.getDynamicPayloadSize();
    char receivePayload[32];
    radio.read(receivePayload, len);
    receivePayload[len] = 0;
    
    if (!strcmp(receivePayload, "on")) {
      digitalWrite(relay, HIGH);      
    } else {
      digitalWrite(relay, LOW);
    }
  }
}
