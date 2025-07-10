#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SIGNAL PA0

float Vi = 3.3;
int Rk = 10000; // 10k known resistor
float Vo;

const int numSamples = 5;
float rxSamples[numSamples];
int sampleIndex = 0;

void setup() {
  Serial.begin(9600);
  pinMode(SIGNAL, INPUT);
  
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.setCursor(0, 30);
  display.setTextSize(1);
  display.print("Smart Resistor Finder");
  display.display();
  delay(2000);
}

void loop() {
  // Read analog value and compute resistance
  float analogValue = analogRead(SIGNAL);
  Vo = analogValue * (Vi / 4095.0);
  float mul = (Vo > 0) ? ((Vi / Vo) - 1) : 0;  // Avoid divide-by-zero
  float Rx = Rk * mul;

  // Store in sample array
  rxSamples[sampleIndex] = Rx;
  sampleIndex = (sampleIndex + 1) % numSamples;

  // Compute average
  float sum = 0;
  for (int i = 0; i < numSamples; i++) {
    sum += rxSamples[i];
  }
  float avgRx = sum / numSamples;

  // Display the result
  display.clearDisplay();
  display.setCursor(10, 30);
  display.setTextSize(1);
  display.print("R: ");
  if (avgRx > 1000) {
    display.print(avgRx / 1000.0, 2);
    display.print(" K ohm");
    Serial.print(avgRx / 1000.0, 2);
    Serial.println(" K");
  } else {
    display.print(avgRx, 0);
    display.print(" ohm");
    Serial.print(avgRx, 0);
    Serial.println(" ohm");
  }
  display.display();

  delay(500);
}
