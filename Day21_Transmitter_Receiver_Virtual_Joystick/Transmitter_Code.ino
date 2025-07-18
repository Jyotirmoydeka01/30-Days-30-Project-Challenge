#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL;
RF24 radio(9, 10);

struct Data_to_be_sent {
  byte ch1;
  byte ch2;
  byte ch3;
  byte ch4;
  byte ch5;
  byte ch6;
  byte ch7;
};

Data_to_be_sent sent_data;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(my_radio_pipe);

  sent_data.ch1 = 127;
  sent_data.ch2 = 127;
  sent_data.ch3 = 127;
  sent_data.ch4 = 127;
  sent_data.ch5 = 0;
  sent_data.ch6 = 0;
  sent_data.ch7 = 0;

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
}

void loop() {
  sent_data.ch1 = map(analogRead(A0), 0, 1023, 0, 255);
  sent_data.ch2 = map(analogRead(A1), 0, 1023, 0, 255);
  sent_data.ch3 = map(analogRead(A2), 0, 1023, 0, 255);
  sent_data.ch4 = map(analogRead(A3), 0, 1023, 0, 255);
  sent_data.ch5 = !digitalRead(2);
  sent_data.ch6 = !digitalRead(3);
  sent_data.ch7 = !digitalRead(4);

  radio.write(&sent_data, sizeof(Data_to_be_sent));

  // Serial debug print
  Serial.print("CH1: "); Serial.print(sent_data.ch1);
  Serial.print(" | CH2: "); Serial.print(sent_data.ch2);
  Serial.print(" | CH3: "); Serial.print(sent_data.ch3);
  Serial.print(" | CH4: "); Serial.print(sent_data.ch4);
  Serial.print(" | CH5: "); Serial.print(sent_data.ch5);
  Serial.print(" | CH6: "); Serial.print(sent_data.ch6);
  Serial.print(" | CH7: "); Serial.println(sent_data.ch7);

  delay(20);
}
