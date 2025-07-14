#include <Servo.h>
#include <U8g2lib.h>
#include <Wire.h>

// OLED: Software I2C
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(
  U8G2_R0, /* clock=*/ A5, /* data=*/ A4, /* reset=*/ U8X8_PIN_NONE);

// Pins
const int servoPin = 4;
const int leftButtonPin = 3;
const int rightButtonPin = 2;

// Servo + PID
Servo myServo;
bool pidActive = false;
float Kp = 1.0, Ki = 0.0, Kd = 0.0;
float setpoint = 90;
float input = 90;
float output = 0;
float previousError = 0;
float integral = 0;

// Timing
unsigned long lastTime = 0;
const int interval = 30;

void setup() {
  u8g2.begin();

  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);
  myServo.attach(servoPin);
  myServo.write(90);  // Center position

  Serial.begin(9600); // For receiving PID from ESP32

  updateOLED();  // Initial OLED display
}

void loop() {
  handleButtons();
  receivePIDfromSerial();

  unsigned long now = millis();
  if (pidActive && (now - lastTime >= interval)) {
    lastTime = now;
    runPID();
  }
}

void handleButtons() {
  static bool leftPressed = false;
  static bool rightPressed = false;

  if (digitalRead(leftButtonPin) == LOW && !leftPressed) {
    leftPressed = true;
    setpoint = 0;
    pidActive = true;
    previousError = 0;
    integral = 0;
  }
  if (digitalRead(leftButtonPin) == HIGH && leftPressed) {
    setpoint = 90;
    leftPressed = false;
    pidActive = true;
    previousError = 0;
    integral = 0;
  }

  if (digitalRead(rightButtonPin) == LOW && !rightPressed) {
    rightPressed = true;
    setpoint = 180;
    pidActive = true;
    previousError = 0;
    integral = 0;
  }
  if (digitalRead(rightButtonPin) == HIGH && rightPressed) {
    setpoint = 90;
    rightPressed = false;
    pidActive = true;
    previousError = 0;
    integral = 0;
  }
}

void runPID() {
  float error = setpoint - input;

  // ✅ Stop when close enough
  if (abs(error) < 1.0) {
    myServo.write((int)setpoint);
    input = setpoint;
    output = 0;
    pidActive = false;
    updateOLED();
    return;
  }

  // PID calculations
  integral += error * (interval / 1000.0);
  float derivative = (error - previousError) / (interval / 1000.0);
  output = Kp * error + Ki * integral + Kd * derivative;

  float newAngle = input + output;
  newAngle = constrain(newAngle, 0, 180);

  myServo.write((int)newAngle);
  input = newAngle;
  previousError = error;

  updateOLED();
}

void receivePIDfromSerial() {
  static String inputString = "";
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      parsePIDString(inputString);
      inputString = "";
    } else {
      inputString += inChar;
    }
  }
}

void parsePIDString(String data) {
  data.trim();  // Remove whitespace
  int firstComma = data.indexOf(',');
  int secondComma = data.indexOf(',', firstComma + 1);

  if (firstComma > 0 && secondComma > firstComma) {
    String kpStr = data.substring(0, firstComma);
    String kiStr = data.substring(firstComma + 1, secondComma);
    String kdStr = data.substring(secondComma + 1);

    float newKp = kpStr.toFloat();
    float newKi = kiStr.toFloat();
    float newKd = kdStr.toFloat();

    Kp = newKp;
    Ki = newKi;
    Kd = newKd;

    updateOLED();
  }
}

void updateOLED() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setCursor(0, 12);
    u8g2.print("Kp: "); u8g2.print(Kp, 2);
    u8g2.setCursor(0, 24);
    u8g2.print("Ki: "); u8g2.print(Ki, 2);
    u8g2.setCursor(0, 36);
    u8g2.print("Kd: "); u8g2.print(Kd, 2);
    u8g2.setCursor(0, 48);
    u8g2.print("Setpoint: "); u8g2.print((int)setpoint);
    u8g2.setCursor(0, 60);
    u8g2.print("Angle: "); u8g2.print((int)input);
  } while (u8g2.nextPage());
}
