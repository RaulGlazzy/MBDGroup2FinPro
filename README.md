# Rocky: Bio-Inspired Quadruped Robot 🕷️🤖

**Universitas Indonesia - Department of Electrical Engineering Computer Engineering KKI** **Microprocessor Based Design Final Project**

An embedded systems project demonstrating quadrupedal locomotion. To fulfill the rigorous requirements of Microprocessor Based Design, "Rocky" is built utilizing pure bare-metal AVR Assembly (`.S`) to handle its core logic and computations, instead of just relying on a standard C++ (`.ino`) sketch. 

---

##  Team Members (Group E)
* **Muhammad Fairuz Dzaki** (2406368864)
* **Raul Fadila Bagus Sumaryada** (2406450466)
* **Ziyadzharif Alfarabi Kurniawan** (2406369053)
* **Evandra Rasya Fadhillah** (2406450352)

---

##  Features
* **Bare-Metal Assembly Integration:** Instead of just writing a basic `.ino` file, this project utilizes a custom `.S` AVR Assembly file to process data and logic at the microprocessor register level.
* **Sequential Creep Gait:** Achieves stable, bio-inspired forward locomotion by moving one limb at a time, ensuring the center of gravity remains balanced.
* **Hybrid ABI Architecture:** Blends the ease of Arduino C++ libraries for hardware abstraction (`Servo.h`, `LiquidCrystal.h`) with the raw performance of AVR Assembly via standard `extern "C"` linkage.
* **Dynamic Facial Expressions:** A 16x2 Alphanumeric LCD acts as the robot's face, reacting in real-time to the current phase of the gait cycle (e.g., sleeping `( - _ - )` while resting, `( ^ _ ^ )` while stepping).

---

##  Hardware & Pinout

### Components Required
* 1x Arduino Uno R3 (ATmega328P)
* 4x SG90 Micro Servos (4.8V)
* 1x 16x2 Alphanumeric LCD (Parallel)
* 1x Breadboard & Jumper Wires (for 5V/GND power distribution)

### Wiring Map
| Component | Pin / Wire | Arduino Uno Pin |
| :--- | :--- | :--- |
| **Front Left Servo** | PWM (Orange) | Digital 6 |
| **Front Right Servo**| PWM (Orange) | Digital 7 |
| **Rear Left Servo** | PWM (Orange) | Digital 8 |
| **Rear Right Servo** | PWM (Orange) | Digital 9 |
| **16x2 LCD** | RS | Digital 12 |
| **16x2 LCD** | EN (E) | Digital 11 |
| **16x2 LCD** | D4, D5, D6, D7 | Digital 5, 4, 3, 2 |

*(Note: All servos and the LCD VDD/A pins must be powered from the Arduino 5V pin via a breadboard rail. All grounds must be tied to the Arduino GND).*

---

##  Software Architecture: Why `.S` instead of just `.ino`?

In standard Arduino projects, everything is written in a C++ `.ino` file. However, to demonstrate true mastery of the ATmega328P architecture, this project is split into two interoperable layers:

1. **`helper.S` (AVR Assembly):** This is where the core logic lives. Instead of letting C++ do the math, the system passes variables into the CPU registers (e.g., `r24`). The assembly code utilizes native ATmega328P instructions (`muls`, `add`, `brne`) to compute logic, clamp values, and optimize data before returning it. 
   
2. **`sketch.ino` (C++ / HAL):** Acts purely as a Hardware Abstraction Layer (HAL). It manages the background hardware timers (50Hz PWM generation) and handles the complex HD44780 LCD communication protocol, while delegating all core calculations to the `.S` file.

---

##  How to Run / Simulate

**Using Wokwi (Web Simulator):**
1. Create a new Arduino Uno project in Wokwi.
2. Paste the provided schematics wokwi design to wire the servos, LCD, and breadboard.
3. Paste the `sketch.ino` code into the main tab, and create a new tab named `helper.S` for the assembly logic.
4. Click the **Play** button to watch the simulation run. 

**Using Arduino IDE (Physical Hardware):**
1. Clone this repository to your local machine.
3. Ensure the `helper.S` file is in the same sketch folder.
4. Connect your Arduino Uno via USB.
5. Select `Arduino Uno` from the **Tools > Board** menu.
6. Click **Upload** and ensure your external power supply/breadboard is turned on.
