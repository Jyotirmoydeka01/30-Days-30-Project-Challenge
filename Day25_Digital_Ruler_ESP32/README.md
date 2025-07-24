Day 25 – Digital Ruler (ESP32 + Ultrasonic + OLED)
📌 Project Overview
The Digital Ruler measures the distance to an object using an ultrasonic sensor and displays it in real-time on an OLED screen, along with a clean UI.

Components Used
ESP32

Ultrasonic Sensor (HC-SR04)

OLED Display (SSD1306, I2C)

Jumper wires, breadboard

Circuit Diagram

Connections
Component	ESP32 Pin
Ultrasonic TRIG	GPIO 5
Ultrasonic ECHO	GPIO 18
OLED SDA	GPIO 21
OLED SCL	GPIO 22
VCC	3.3V
GND	GND

![Demo](./Full_Setup.jpg)



Code
!Code is provided in the Digital_Ruler_ESP32.ino file.

Future Improvements
Add a graphical moving ruler bar.

Add a buzzer alert for objects closer than a set threshold.

Store min/max distance in ESP32 Preferences.