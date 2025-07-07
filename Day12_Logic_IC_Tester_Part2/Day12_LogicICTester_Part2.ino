#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button pins
#define BUTTON1_PIN PA0  // Scroll
#define BUTTON2_PIN PA1  // Select


#define IN1 PA2 //input 1
#define IN2 PA3 // input 2
#define OUT1 PA4// output1 

#define IN3 PA5 //input 3
#define IN4 PA6 // input 4
#define OUT2 PA7// output 2 


#define IN5 PB0 //input 5
#define IN6 PB1 // input 6
#define OUT3 PB3// output 3

#define IN7 PB4 //input 7
#define IN8 PB5 // input 8
#define OUT4 PB8// output 4 




const char* icList[] = {"7400 NAND", "7402 NOR", "7404 NOT", "7408 AND", "7432 OR", "7486 XOR"};

const char* icWiring[] = {
  "7400 NAND:\n1: PA2  2: PA3\n3: PA4  4: PA5\n5: PA6  6: PA7\n7: GND  8: PB0\n9: PB1  10: PB3\n11: PB4  12: PB5\n13: PB8  14:VCC",
  "7402 NOR:\n1: PA2  2: PA3\n3: PA4  4: PA5\n5: PA6  6: PA7\n7: GND  8: PB0\n9: PB1  10: PB3\n11: PB4  12: PB5\n13: PB8  14:VCC",
  "7404 NOT:\n1: PA2  2: PA3\n3: PA4  4: PA5\n5: PA6  6: PA7\n7: GND  8: PB0\n9: PB1  10: PB3\n11: PB4  12: PB5\n13: PB8  14:VCC",
  "7408 AND:\n1: PA2  2: PA3\n3: PA4  4: PA5\n5: PA6  6: PA7\n7: GND  8: PB0\n9: PB1  10: PB3\n11: PB4  12: PB5\n13: PB8  14:VCC",
  "7432 OR:\n1: PA2  2: PA3\n3: PA4  4: PA5\n5: PA6  6: PA7\n7: GND  8: PB0\n9: PB1  10: PB3\n11: PB4  12: PB5\n13: PB8  14:VCC",
  "7486 XOR:\n1: PA2  2: PA3\n3: PA4  4: PA5\n5: PA6  6: PA7\n7: GND  8: PB0\n9: PB1  10: PB3\n11: PB4  12: PB5\n13: PB8  14:VCC"
};

int totalICs = sizeof(icList) / sizeof(icList[0]);


bool test7402() {
  int inputs[4][2] = {
    {0, 0},
    {0, 1},
    {1, 0},
    {1, 1}
  };
  int expected[4] = {1, 0, 0, 0};

  bool allPassed = true;

  for (int i = 0; i < 4; i++) {
    digitalWrite(IN1, inputs[i][0]);
    digitalWrite(IN2, inputs[i][1]);

    digitalWrite(IN3, inputs[i][0]);
    digitalWrite(IN4, inputs[i][1]);

    digitalWrite(IN5, inputs[i][0]);
    digitalWrite(IN6, inputs[i][1]);

    digitalWrite(IN7, inputs[i][0]);
    digitalWrite(IN8, inputs[i][1]);

    delay(5);  // Let signals settle

    int o1 = digitalRead(OUT1);
    int o2 = digitalRead(OUT2);
    int o3 = digitalRead(OUT3);
    int o4 = digitalRead(OUT4);

    if (o1 != expected[i]) {
      Serial.print("FAIL: OUT1 for input "); Serial.println(i);
      allPassed = false;
    }

    if (o2 != expected[i]) {
      Serial.print("FAIL: OUT2 for input "); Serial.println(i);
      allPassed = false;
    }

    if (o3 != expected[i]) {
      Serial.print("FAIL: OUT3 for input "); Serial.println(i);
      allPassed = false;
    }

    if (o4 != expected[i]) {
      Serial.print("FAIL: OUT4 for input "); Serial.println(i);
      allPassed = false;
    }
  }

  return allPassed;
}


int currentIndex = 0;
int topIndex = 0;
const int itemsPerPage = 4;

bool button1Pressed = false;
bool button2Pressed = false;

enum DisplayState { MENU, SELECTED_IC, WIRING_INFO, TESTING };//different states for selected pin
DisplayState currentState = MENU;//initial in main menu


void setup() {
  Serial.begin(9600);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  // INPUTS
pinMode(IN1, OUTPUT);  digitalWrite(IN1, LOW);
pinMode(IN2, OUTPUT);  digitalWrite(IN2, LOW);
pinMode(IN3, OUTPUT);  digitalWrite(IN3, LOW);
pinMode(IN4, OUTPUT);  digitalWrite(IN4, LOW);
pinMode(IN5, OUTPUT);  digitalWrite(IN5, LOW);
pinMode(IN6, OUTPUT);  digitalWrite(IN6, LOW);
pinMode(IN7, OUTPUT);  digitalWrite(IN7, LOW);
pinMode(IN8, OUTPUT);  digitalWrite(IN8, LOW);

// OUTPUTS
pinMode(OUT1, INPUT_PULLDOWN);
pinMode(OUT2, INPUT_PULLDOWN);
pinMode(OUT3, INPUT_PULLDOWN);
pinMode(OUT4, INPUT_PULLDOWN);


  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(35, 10);//set position for text
  display.print("IC TESTER");//print this line
  display.setCursor(35, 30);
  display.print(" v 3.0");
  display.display();
  delay(3000);//wait 3 sec
  display.clearDisplay();

  showMenu();
}

void loop() {
  // Scroll Button
  if (digitalRead(BUTTON1_PIN) == LOW && !button1Pressed) {
    button1Pressed = true;

    // Scroll always takes you back to the main menu
    currentState = MENU;
    currentIndex = (currentIndex + 1) % totalICs;

    if (currentIndex >= topIndex + itemsPerPage) {
      topIndex++;
    }

    if (currentIndex == 0) {
      topIndex = 0;
    }

    showMenu();
    delay(150); // debounce
  }
  if (digitalRead(BUTTON1_PIN) == HIGH) {
    button1Pressed = false;
  }

  // Select Button
  if (digitalRead(BUTTON2_PIN) == LOW && !button2Pressed) {
    button2Pressed = true;

    if (currentState == MENU) {
      currentState = SELECTED_IC;
      showSelectedIC(icList[currentIndex]);

    } else if (currentState == SELECTED_IC) {
      currentState = WIRING_INFO;
      showWiringInfo(currentIndex);

    } else if (currentState == WIRING_INFO) {
      currentState = TESTING;

      bool pass = false;

      if (currentIndex == 1) { // 7402 NOR
        pass = test7402();
      }
      // You can add more tests like this:
      // else if (currentIndex == 0) pass = test7400();
      // else if (currentIndex == 2) pass = test7404();
      // etc.

      showTestResult(pass);
    }

    delay(150); // debounce
  }
  if (digitalRead(BUTTON2_PIN) == HIGH) {
    button2Pressed = false;
  }
}


// Show Menu List
void showMenu() {
  display.clearDisplay();
  display.setTextSize(1);

  for (int i = 0; i < itemsPerPage; i++) {
    int icIndex = topIndex + i;
    if (icIndex >= totalICs) break;

    int y = i * 16;
    display.setCursor(0, y);
    if (icIndex == currentIndex) {
      display.print(">");
    } else {
      display.print(" ");
    }

    display.setCursor(10, y);
    display.print(icList[icIndex]);
  }

  display.display();
}

// Action on Select
void showSelectedIC(const char* icName) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Selected:");
  display.setCursor(60, 0);
  display.println(icName);
  display.setCursor(0, 30);
  display.println("Select to conitinue");
  display.setCursor(0, 45);
  display.println("Scroll for menu");


  display.display();
  // Show for 2 seconds
  // Return to menu
}

void showWiringInfo(int index) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  // Split the text manually for 128x64 display
  const char* wiring = icWiring[index];
  int y = 0;
  for (int i = 0; wiring[i] != '\0'; i++) {
    if (wiring[i] == '\n') {
      y += 8;
      display.setCursor(0, y);
    } else {
      display.write(wiring[i]);
    }
  }

  display.display();
}

void showTestResult(bool pass) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  if (pass) {
    display.println("IC Test Passed!");
  } else {
    display.println("IC Test Failed!");
    display.setCursor(0, 20);
    display.println("Check output pins");
  }

  display.setCursor(0, 50);
  display.println("Scroll for menu");
  display.display();
}
