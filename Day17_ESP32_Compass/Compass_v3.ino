#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_light.h>
#include <Preferences.h>
#include <math.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

MPU6050 mpu(Wire);
Preferences prefs;

#define BUTTON_PIN 23
int northOffset = 0;
int lastHeading = -1;

void saveOffset(int offset) {
  prefs.begin("compass", false);
  prefs.putInt("north", offset);
  prefs.end();
}

int loadOffset() {
  prefs.begin("compass", true);
  int val = prefs.getInt("north", 0);
  prefs.end();
  return val;
}

void drawCompass(int heading) {
  display.clearDisplay();

  int cx = SCREEN_WIDTH / 2;
  int cy = SCREEN_HEIGHT / 2;
  int radius = 25;

  display.fillTriangle(cx, cy - radius, cx - 4, cy - radius + 8, cx + 4, cy - radius + 8, SSD1306_WHITE);

  struct DirLabel {
    const char* label;
    int angle;
  };

  DirLabel labels[] = {
    {"N", 0}, {"NE", 45}, {"E", 90}, {"SE", 135},
    {"S", 180}, {"SW", 225}, {"W", 270}, {"NW", 315}
  };

  for (auto& d : labels) {
    float a = (d.angle + heading) % 360 * DEG_TO_RAD;

    int x = cx + cos(a) * (radius + 10) - 3;
    int y = cy + sin(a) * (radius + 10) - 3;

    display.setCursor(x, y);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.print(d.label);
  }

  display.drawCircle(cx, cy, radius, SSD1306_WHITE);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Heading: ");
  display.setCursor(0, 10);
  display.print(heading);
  display.print("\ndeg");
  display.display();
}

void setup() {
  //Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("Compass Init... v 3");
  display.display();

  mpu.begin();
  mpu.calcOffsets();
  delay(500);

  prefs.begin("compass", true);
  bool calibratedBefore = prefs.isKey("north");
  prefs.end();

  if (!calibratedBefore) {
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("Point North...");
    display.display();
    delay(3000);
    mpu.update();
    int raw = int(mpu.getAngleZ()) % 360;
    if (raw < 0) raw += 360;
    saveOffset(raw);
    northOffset = raw;
  } else {
    northOffset = loadOffset();
  }
}

void loop() {
  mpu.update();

  int rawHeading = int(mpu.getAngleZ());
  if (rawHeading < 0) rawHeading += 360;

  if (digitalRead(BUTTON_PIN) == LOW) {
    // Serial.print("button pressed");
    delay(300); // debounce
    int offset = (360 - rawHeading) % 360;
    saveOffset(offset);
    northOffset = offset;


    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("Recalibrated!");
    display.display();
    delay(1000);
  }

  int relativeHeading = (rawHeading - northOffset + 360) % 360;

  // Drift filtering: Only redraw if heading changed by >= 2 degrees
  if (abs(relativeHeading - lastHeading) >= 2) {
    drawCompass(relativeHeading);
    lastHeading = relativeHeading;
  }

  delay(50);
}
