#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Message buffer
String incomingMsg = "";

void setup() {
  // Start I2C for OLED
  Wire.begin();

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);  // Halt if display doesn't initialize
  }

  // Display startup message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Waiting for");
  display.println("Bluetooth...");
  display.display();

  // Initialize Serial (PA9 = TX, PA10 = RX for default Serial)
  Serial.begin(9600);  // HC-05 default baud rate is 9600
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();

    // If newline received, show message
    if (c == '\n' || c == '\r') {
      if (incomingMsg.length() > 0) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("Received:");
        display.setTextSize(2);
        display.setCursor(0, 16);
        display.println(incomingMsg);
        display.display();
        incomingMsg = "";  // Clear buffer after displaying
      }
    } else {
      incomingMsg += c;
    }
  }
}
