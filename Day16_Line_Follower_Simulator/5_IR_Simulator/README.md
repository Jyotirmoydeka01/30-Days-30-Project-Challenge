# 5-IR Sensor Pattern Simulator

This project simulates a line follower robot using **5 IR sensors**. Predefined patterns mimic the input of 5 sensors (like in real line followers), and the OLED shows the corresponding robot action.

## 📋 Pattern Examples

| IR Pattern | Action         |
|------------|----------------|
| 00100      | Go Straight    |
| 01100      | Slight Left    |
| 00010      | Turn Left      |
| 00001      | Sharp Left     |
| 00000      | Stop           |
| 11111      | Full Line      |
| 10000      | Sharp Right    |
| 00110      | Slight Right   |
| 00011      | Turn Right     |

Each pattern is displayed with:
- A visual representation of the 5 IR sensors
- The corresponding robot movement

## 🧰 Components

- STM32 Blue Pill / Arduino Nano
- OLED Display (SSD1306, I2C)
- Jumper Wires
- Breadboard

## 🖼️ Demo

![5-IR Demo](Demo_Image_5IR.jpg)

---

> 🔄 Patterns loop automatically every 1.5 seconds. Ideal for visualizing how a robot interprets IR data!
