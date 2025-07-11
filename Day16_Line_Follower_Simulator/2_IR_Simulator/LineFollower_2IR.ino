#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// IR sensor pins
#define IR_LEFT PA0
#define IR_RIGHT PA1

void setup() {
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(10, 28);
  display.println("2-IR Line Follower");
  display.display();
  delay(1500);
}

void loop() {
  int leftIR = digitalRead(IR_LEFT);
  int rightIR = digitalRead(IR_RIGHT);
  String action = "";

   // Determine action
  if (leftIR == HIGH && rightIR == HIGH) action = "Forward";
  else if (leftIR == HIGH && rightIR == LOW) action = "Turn Left";
  else if (leftIR == LOW && rightIR == HIGH) action = "Turn Right";
  else action = "Stop";

  // OLED display
  display.clearDisplay();

  // Show IR values visually
  display.setCursor(0, 0);
  display.print("IR L:");
  display.print(leftIR);
  display.print(" R:");
  display.print(rightIR);

 display.drawRect(30,20,20,20,SSD1306_WHITE);
 if (leftIR) display.fillRect(30,20,20,20,SSD1306_WHITE);

 display.drawRect(80,20,20,20,SSD1306_WHITE);
 if(rightIR) display.fillRect(80,20,20,20,SSD1306_WHITE);

 display.setCursor(20,50);
 display.setTextSize(1);
  display.print("Action: ");
  display.print(action);

  display.display();
  delay(300);
 

  
  

}
