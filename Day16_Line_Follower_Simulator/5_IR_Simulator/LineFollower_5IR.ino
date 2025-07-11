#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int frame = 0;
unsigned long lastUpdate = 0;
int animationOffset = 0;
bool movingForward = true;

// Track definitions with visual representations
const char* tracks[] = {
  // Straight
  "00100\n"
  "00100\n"
  "00100\n"
  "00100\n"
  "00100",

  // Slight Left
  "00100\n"
  "00100\n"
  "01100\n"
  "01000\n"
  "01000",

  // Turn Left
  "00100\n"
  "01000\n"
  "10000\n"
  "00000\n"
  "00000",

  // Sharp Left
  "00001\n"
  "00010\n"
  "00100\n"
  "01000\n"
  "10000",

  // Stop
  "00000\n"
  "00000\n"
  "00000\n"
  "00000\n"
  "00000",

  // Full Line
  "11111\n"
  "11111\n"
  "11111\n"
  "11111\n"
  "11111",

  // Sharp Right
  "10000\n"
  "01000\n"
  "00100\n"
  "00010\n"
  "00001",

  // Slight Right
  "00100\n"
  "00100\n"
  "00110\n"
  "00010\n"
  "00010",

  // Turn Right
  "00100\n"
  "00010\n"
  "00001\n"
  "00000\n"
  "00000"
};

String directions[] = {
  "Straight",
  "Slight Left",
  "Turn Left",
  "Sharp Left",
  "STOP!",
  "Full Line",
  "Sharp Right",
  "Slight Right",
  "Turn Right"
};

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(10, 28);
  display.println("Advanced Line Follower");
  display.display();
  delay(1500);
}

void drawTrack(int patternIndex) {
  String trackData = tracks[patternIndex];
  int row = 0;
  int col = 0;
  
  for (int i = 0; i < trackData.length(); i++) {
    char c = trackData[i];
    if (c == '\n') {
      row++;
      col = 0;
    } else {
      int x = 34 + col * 12;  // Centered with spacing
      int y = 10 + row * 8;   // Vertical spacing
      
      if (c == '1') {
        // Draw track segment
        display.fillRect(x, y, 8, 4, SSD1306_WHITE);
        
        // Draw track borders
        display.drawPixel(x, y-1, SSD1306_WHITE);
        display.drawPixel(x+7, y-1, SSD1306_WHITE);
        display.drawPixel(x, y+4, SSD1306_WHITE);
        display.drawPixel(x+7, y+4, SSD1306_WHITE);
      }
      col++;
    }
  }
}

void drawRobot(int directionIndex) {
  // Robot body (centered)
  int robotX = 64 - 15;
  int robotY = 50;
  
  // Animated position
  robotY -= animationOffset;
  
  display.fillRect(robotX, robotY, 30, 10, SSD1306_WHITE);
  
  // Draw wheels
  display.fillRect(robotX-3, robotY, 3, 10, SSD1306_WHITE);
  display.fillRect(robotX+30, robotY, 3, 10, SSD1306_WHITE);
  
  // Draw direction indicator
  int centerX = robotX + 15;
  int centerY = robotY + 5;
  
  switch(directionIndex) {
    case 0: // Straight
      display.drawLine(centerX, centerY-4, centerX, centerY-8, SSD1306_BLACK);
      display.fillTriangle(
        centerX-3, centerY-6,
        centerX+3, centerY-6,
        centerX, centerY-10, SSD1306_BLACK);
      break;
      
    case 1: // Slight Left
      display.drawLine(centerX-2, centerY-4, centerX-5, centerY-7, SSD1306_BLACK);
      display.fillTriangle(
        centerX-6, centerY-7,
        centerX-8, centerY-5,
        centerX-5, centerY-10, SSD1306_BLACK);
      break;
      
    case 2: // Turn Left
      display.drawLine(centerX-3, centerY-1, centerX-8, centerY-1, SSD1306_BLACK);
      display.fillTriangle(
        centerX-8, centerY-3,
        centerX-8, centerY+1,
        centerX-12, centerY-1, SSD1306_BLACK);
      break;
      
    case 3: // Sharp Left
      display.drawLine(centerX-3, centerY+2, centerX-8, centerY+2, SSD1306_BLACK);
      display.fillTriangle(
        centerX-8, centerY,
        centerX-8, centerY+4,
        centerX-12, centerY+2, SSD1306_BLACK);
      break;
      
    case 4: // Stop
      display.fillRect(centerX-6, centerY-4, 12, 8, SSD1306_BLACK);
      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(1);
      display.setCursor(centerX-4, centerY-3);
      display.print("STP");
      break;
      
    case 6: // Sharp Right
      display.drawLine(centerX+3, centerY+2, centerX+8, centerY+2, SSD1306_BLACK);
      display.fillTriangle(
        centerX+8, centerY,
        centerX+8, centerY+4,
        centerX+12, centerY+2, SSD1306_BLACK);
      break;
      
    case 7: // Slight Right
      display.drawLine(centerX+2, centerY-4, centerX+5, centerY-7, SSD1306_BLACK);
      display.fillTriangle(
        centerX+6, centerY-7,
        centerX+8, centerY-5,
        centerX+5, centerY-10, SSD1306_BLACK);
      break;
      
    case 8: // Turn Right
      display.drawLine(centerX+3, centerY-1, centerX+8, centerY-1, SSD1306_BLACK);
      display.fillTriangle(
        centerX+8, centerY-3,
        centerX+8, centerY+1,
        centerX+12, centerY-1, SSD1306_BLACK);
      break;
  }
}

void loop() {
  if (millis() - lastUpdate > 1500) {
    frame = (frame + 1) % (sizeof(directions) / sizeof(directions[0]));
    lastUpdate = millis();
    movingForward = !movingForward;
  }

  // Animate robot movement
  animationOffset = (millis() / 50) % 10;
  if (!movingForward) animationOffset = 9 - animationOffset;
  if (frame == 4) animationOffset = 0; // No movement when stopped

  display.clearDisplay();
  
  // Draw track
  drawTrack(frame);
  
  // Draw robot
  drawRobot(frame);
  
  // Draw status info
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  
  // Direction title
  display.setCursor(5, 0);
  display.print("Dir:");
  
  // Current direction
  display.setTextSize(1);
  display.setCursor(40, 0);
  display.print(directions[frame]);
  
  // Frame counter
  display.setCursor(100, 0);
  display.print(frame + 1);
  display.print("/");
  display.print(sizeof(directions) / sizeof(directions[0]));
  
  // Motion indicator
  display.setCursor(5, 55);
  display.print(movingForward ? "FWD" : "REV");
  
  // Speed indicator
  display.setCursor(100, 55);
  display.print("SPD:");
  display.print(frame == 4 ? 0 : 50 + animationOffset * 5);
  
  display.display();
}
