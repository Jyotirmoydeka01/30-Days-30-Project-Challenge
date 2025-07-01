# Day 6 – 555 Timer Variable Frequency Generator

This project uses the 555 timer IC in astable mode to create a square wave. A 100k potentiometer is used to vary the output frequency, and an LED visually represents the pulse rate.

---

## 🔧 Components Used
- 555 Timer IC
- 100k Potentiometer
- 10µF Capacitor
- LED
- 220Ω Resistor
- Breadboard & Jumper Wires

---

## ⚙️ Working Principle
The 555 timer is configured in astable mode, meaning it continuously switches between high and low states. The potentiometer adjusts the resistance, which changes the charge/discharge cycle of the capacitor, effectively controlling the blinking speed of the LED.

### 📐 Frequency Formula:
f = 1.44 / ((R1 + 2*R2) * C)

---

## 📸 Project Demo

| Circuit Diagram | Final Output |
|-----------------|---------------|
| ![Circuit](./Circuit_Diagram.png) | ![Demo](./Demo_Image.jpg) |

---

## 🧠 What I Learned
- Using the 555 timer in astable mode  
- How RC timing affects square wave generation  
- Tuning output frequency using a potentiometer  
- Visualizing square wave through LED blinking
