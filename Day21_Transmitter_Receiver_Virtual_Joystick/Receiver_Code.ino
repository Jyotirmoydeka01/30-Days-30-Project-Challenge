#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeIn = 0xE8E8F0F0E1LL;  // Same pipe as transmitter
RF24 radio(9, 10);  // CE and CSN

// Data structure from transmitter
struct Received_data {
  byte ch1;
  byte ch2;
  byte ch3;
  byte ch4;
  byte ch5;
  byte ch6;
  byte ch7;
};

Received_data received_data;

void reset_the_Data() {
  received_data.ch1 = 0;      // throttle
  received_data.ch2 = 127;
  received_data.ch3 = 127;
  received_data.ch4 = 127;
  received_data.ch5 = 0;
  received_data.ch6 = 0;
  received_data.ch7 = 0;
}

unsigned long lastRecvTime = 0;

void setup() {
  Serial.begin(9600);  // Higher baud rate for faster data transmission
  reset_the_Data();

  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, pipeIn);
  radio.startListening();
}

void receive_the_data() {
  while (radio.available()) {
    radio.read(&received_data, sizeof(Received_data));
    lastRecvTime = millis();
  }
}

void loop() {
  receive_the_data();

  // If no signal for 1 sec, reset
  if (millis() - lastRecvTime > 1000) {
    reset_the_Data();
  }

  // Print the received values in CSV format
  Serial.print(received_data.ch1); Serial.print(",");
  Serial.print(received_data.ch2); Serial.print(",");
  Serial.print(received_data.ch3); Serial.print(",");
  Serial.print(received_data.ch4); Serial.print(",");
  Serial.print(received_data.ch5); Serial.print(",");
  Serial.print(received_data.ch6); Serial.print(",");
  Serial.print(received_data.ch7); Serial.println();
  
  delay(10);  // Optional: reduce if needed
}
