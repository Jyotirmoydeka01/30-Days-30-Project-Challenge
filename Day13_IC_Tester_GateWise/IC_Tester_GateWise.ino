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
#define OUT3 PB9// output 3

#define IN7 PB8 //input 7
#define IN8 PB5 // input 8
#define OUT4 PB10// output 4 




const char* icList[] = {"7400 NAND", "7402 NOR", "7404 NOT", "7408 AND", "7432 OR", "7486 XOR"};

const char* icWiring[] = {
  "7400 NAND:\n1: PA2  2: PA3\n3: PA4  4: PA5\n5: PA6  6: PA7\n7: GND  8: PB3\n9: PB0  10: PB1\n11: PB9  12: PB8\n13: PB5  14:VCC",
  "7402 NOR:\n1: PA2  2: PA3\n3: PA4  4: PA5\n5: PA6  6: PA7\n7: GND  8: PB3\n9: PB0  10: PB1\n11: PB9  12: PB8\n13: PB5  14:VCC",
  "7404 NOT:\n1: PA2  2: PA4\n3: PA3  4: PA7\n5: PA5  6: PA9\n7: GND  8: P10\n9: PA6  10: PB8\n11: PB0  12: PB5\n13: PB1  14:VCC",
  "7408 AND:\n1: PA2  2: PA3\n3: PA4  4: PA5\n5: PA6  6: PA7\n7: GND  8: PB3\n9: PB0  10: PB1\n11: PB9  12: PB8\n13: PB5  14:VCC",
  "7432 OR:\n1: PA2  2: PA3\n3: PA4  4: PA5\n5: PA6  6: PA7\n7: GND  8: PB3\n9: PB0  10: PB1\n11: PB9  12: PB8\n13: PB5  14:VCC",
  "7486 XOR:\n1: PA2  2: PA3\n3: PA4  4: PA5\n5: PA6  6: PA7\n7: GND  8: PB3\n9: PB0  10: PB1\n11: PB9  12: PB8\n13: PB5  14:VCC"
};//wiring diagram

int totalICs = sizeof(icList) / sizeof(icList[0]);//totaL IC number


// 7400 IC 
bool test7400(char* resultMsg) {
  int inputs[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
  int expected[4] = {1, 1, 1, 0}; // NAND

  bool allPassed = true;
  strcpy(resultMsg, "");

  for (int g = 0; g < 4; g++) {
    int in1, in2, outPin;
    if (g == 0) {
      in1 = IN1;
      in2 = IN2;
      outPin = OUT1;
    }
    else if (g == 1) {
      in1 = IN3;
      in2 = IN4;
      outPin = OUT2;
    }
    else if (g == 2) {
      in1 = IN5;
      in2 = IN6;
      outPin = OUT3;
    }
    else {
      in1 = IN7;
      in2 = IN8;
      outPin = OUT4;
    }

    for (int i = 0; i < 4; i++) {
      digitalWrite(in1, inputs[i][0]);
      digitalWrite(in2, inputs[i][1]);
      delay(100);
      int out = digitalRead(outPin);
      if (out != expected[i]) {
        allPassed = false;
        char buf[32];
        sprintf(buf, "❌ Gate %d failed IN(%d,%d)\n", g + 1, inputs[i][0], inputs[i][1]);
        strcat(resultMsg, buf);
        break;
      }
    }
  }

  return allPassed;
}


bool test7402(char* resultMsg) {
  int inputs[4][2] = {{0,0}, {0,1}, {1,0}, {1,1}};
  int expected[4] = {1, 0, 0, 0}; // NOR

  bool allPassed = true;
  strcpy(resultMsg, "");

  for (int g = 0; g < 4; g++) {
    int in1, in2, outPin;
    if (g == 0) { in1 = IN1; in2 = IN2; outPin = OUT1; }
    else if (g == 1) { in1 = IN3; in2 = IN4; outPin = OUT2; }
    else if (g == 2) { in1 = IN5; in2 = IN6; outPin = OUT3; }
    else { in1 = IN7; in2 = IN8; outPin = OUT4; }

    for (int i = 0; i < 4; i++) {
      digitalWrite(in1, inputs[i][0]);
      digitalWrite(in2, inputs[i][1]);
      delay(100);
      int out = digitalRead(outPin);
      if (out != expected[i]) {
        allPassed = false;
        char buf[32];
        sprintf(buf, "❌ Gate %d failed IN(%d,%d)\n", g+1, inputs[i][0], inputs[i][1]);
        strcat(resultMsg, buf);
        break;
      }
    }
  }

  return allPassed;
}


bool test7404(char* resultMsg) {
  int inputs[2] = {0, 1};
  int expected[2] = {1, 0}; // NOT

  bool allPassed = true;
  strcpy(resultMsg, "");

  int inPins[6] = {IN1, IN2, IN3, IN4, IN5, IN6};
  int outPins[6] = {OUT1, OUT2, OUT3, OUT4, IN7, IN8}; // You can repurpose remaining pins

  for (int g = 0; g < 6; g++) {
    for (int i = 0; i < 2; i++) {
      digitalWrite(inPins[g], inputs[i]);
      delay(100);
      int out = digitalRead(outPins[g]);
      if (out != expected[i]) {
        allPassed = false;
        char buf[32];
        sprintf(buf, "❌ Inverter %d failed IN(%d)\n", g+1, inputs[i]);
        strcat(resultMsg, buf);
        break;
      }
    }
  }

  return allPassed;
}

bool test7408(char* resultMsg) {
  int inputs[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
  int expected[4] = {0 , 0, 0, 1}; // AND

  bool allPassed = true;
  strcpy(resultMsg, "");

  for (int g = 0; g < 4; g++) {
    int in1, in2, outPin;
    if (g == 0) {
      in1 = IN1;
      in2 = IN2;
      outPin = OUT1;
    }
    else if (g == 1) {
      in1 = IN3;
      in2 = IN4;
      outPin = OUT2;
    }
    else if (g == 2) {
      in1 = IN5;
      in2 = IN6;
      outPin = OUT3;
    }
    else {
      in1 = IN7;
      in2 = IN8;
      outPin = OUT4;
    }

    for (int i = 0; i < 4; i++) {
      digitalWrite(in1, inputs[i][0]);
      digitalWrite(in2, inputs[i][1]);
      delay(100);
      int out = digitalRead(outPin);
      if (out != expected[i]) {
        allPassed = false;
        char buf[32];
        sprintf(buf, "❌ Gate %d failed IN(%d,%d)\n", g + 1, inputs[i][0], inputs[i][1]);
        strcat(resultMsg, buf);
        break;
      }
    }
  }

  return allPassed;
}

bool test7432(char* resultMsg) {
  int inputs[4][2] = {{0,0}, {0,1}, {1,0}, {1,1}};
  int expected[4] = {0, 1, 1, 1}; // OR

  bool allPassed = true;
  strcpy(resultMsg, "");

  for (int g = 0; g < 4; g++) {
    int in1, in2, outPin;
    if (g == 0) { in1 = IN1; in2 = IN2; outPin = OUT1; }
    else if (g == 1) { in1 = IN3; in2 = IN4; outPin = OUT2; }
    else if (g == 2) { in1 = IN5; in2 = IN6; outPin = OUT3; }
    else { in1 = IN7; in2 = IN8; outPin = OUT4; }

    for (int i = 0; i < 4; i++) {
      digitalWrite(in1, inputs[i][0]);
      digitalWrite(in2, inputs[i][1]);
      delay(100);
      int out = digitalRead(outPin);
      if (out != expected[i]) {
        allPassed = false;
        char buf[32];
        sprintf(buf, "❌ Gate %d failed IN(%d,%d)\n", g+1, inputs[i][0], inputs[i][1]);
        strcat(resultMsg, buf);
        break;
      }
    }
  }

  return allPassed;
}
bool test7486(char* resultMsg) {
  int inputs[4][2] = {{0,0}, {0,1}, {1,0}, {1,1}};
  int expected[4] = {0, 1, 1, 0}; // XOR

  bool allPassed = true;
  strcpy(resultMsg, "");

  for (int g = 0; g < 4; g++) {
    int in1, in2, outPin;
    if (g == 0) { in1 = IN1; in2 = IN2; outPin = OUT1; }
    else if (g == 1) { in1 = IN3; in2 = IN4; outPin = OUT2; }
    else if (g == 2) { in1 = IN5; in2 = IN6; outPin = OUT3; }
    else { in1 = IN7; in2 = IN8; outPin = OUT4; }

    for (int i = 0; i < 4; i++) {
      digitalWrite(in1, inputs[i][0]);
      digitalWrite(in2, inputs[i][1]);
      delay(100);
      int out = digitalRead(outPin);
      if (out != expected[i]) {
        allPassed = false;
        char buf[32];
        sprintf(buf, "❌ Gate %d failed IN(%d,%d)\n", g+1, inputs[i][0], inputs[i][1]);
        strcat(resultMsg, buf);
        break;
      }
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
  pinMode(OUT1, INPUT);
  pinMode(OUT2, INPUT);
  pinMode(OUT3, INPUT);
  pinMode(OUT4, INPUT);


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

      
      char message[128] = "";

      if (currentIndex == 0) pass = test7400(message);
      else if (currentIndex == 1) pass = test7402(message);
      else if (currentIndex == 2) pass = test7404(message);
      else if (currentIndex == 3) pass = test7408(message);
      else if (currentIndex == 4) pass = test7432(message);
      else if (currentIndex == 5) pass = test7486(message);

      showTestResult(pass, message);





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

void showTestResult(bool pass, const char* failMessage) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  if (pass) {
    display.println("IC Test Passed!");
  } else {
    display.println("IC Test Failed:");
    display.setCursor(0, 16);
    int y = 16;
    for (int i = 0; failMessage[i] != '\0'; i++) {
      if (failMessage[i] == '\n') {
        y += 8;
        display.setCursor(0, y);
      } else {
        display.write(failMessage[i]);
      }
    }
  }

  display.setCursor(0, 56);
  display.println("Scroll for menu");
  display.display();
}
