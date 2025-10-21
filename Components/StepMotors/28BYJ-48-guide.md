# 28BYJ-48 Stepper Motor - Complete Guide

## Overview

The **28BYJ-48** is a small, geared **unipolar stepper motor** commonly used with the **ULN2003 driver board**. It uses **4-wire control** and is NOT compatible with STEP/DIR (3-pin) projects without code conversion.

---

## Table of Contents

1. [What You Have](#what-you-have)
2. [Key Differences: 3-Pin vs 4-Pin](#key-differences-3-pin-vs-4-pin)
3. [Wiring Guide](#wiring-guide)
4. [Control Methods](#control-methods)
5. [Code Examples](#code-examples)
6. [Adapting STEP/DIR Projects](#adapting-stepdir-projects)
7. [Troubleshooting](#troubleshooting)
8. [Specifications](#specifications)

---

## What You Have

### Motor: 28BYJ-48
```
Type: Unipolar stepper motor with internal gear reduction
Voltage: 5V or 12V (most common: 5V version)
Gear Ratio: 64:1 (internal gearing)
Steps per Revolution (with gears): 2048 steps
Steps per Revolution (motor only): 32 steps × 64 = 2048
Step Angle: 5.625° / 64 = 0.087890625°
Frequency: 100Hz (max)
Torque: ~300 gf·cm (at 5V)
Current per phase: ~240mA
Weight: 30g
Price: Very cheap (~$2-3)
```

### Controller: ULN2003 Driver Board
```
Type: Darlington transistor array
Control Pins: 4 (IN1, IN2, IN3, IN4)
LEDs: 4 (show coil activity)
Motor Connector: 5-pin socket for 28BYJ-48
Voltage: 5-12V
Logic Level: 5V (Arduino compatible)
Color: Usually blue PCB
```

---

## Key Differences: 3-Pin vs 4-Pin

### 3-Pin Stepper System (STEP/DIR - A4988/DRV8825)

**Control Method:**
```
Arduino Pin 1 → STEP   (pulse once per step)
Arduino Pin 2 → DIR    (HIGH = clockwise, LOW = counter-clockwise)
Arduino Pin 3 → ENABLE (optional, LOW = enabled)
```

**Hardware:**
```
Motor Type: Bipolar stepper
Driver: A4988, DRV8825, TMC2208, etc.
Voltage: 8-35V (motor voltage)
Control: Pulse/Direction interface
Library: None needed (simple digitalWrite)
Used in: CNC machines, 3D printers
```

**Example Code:**
```cpp
// 3-pin STEP/DIR control
digitalWrite(dirPin, HIGH);  // Set direction

for (int i = 0; i < 200; i++) {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(500);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(500);
}
```

---

### 4-Pin Stepper System (28BYJ-48 - ULN2003)

**Control Method:**
```
Arduino Pin 1 → IN1 (Coil A)
Arduino Pin 2 → IN2 (Coil B)
Arduino Pin 3 → IN3 (Coil C)
Arduino Pin 4 → IN4 (Coil D)
```

**Hardware:**
```
Motor Type: Unipolar stepper with gear reduction
Driver: ULN2003 (Darlington array)
Voltage: 5V (or 12V for 12V version)
Control: Direct coil sequencing
Library: Stepper.h or AccelStepper
Used in: Arduino projects, displays, light loads
```

**Example Code:**
```cpp
// 4-pin direct coil control
#include <Stepper.h>

Stepper myStepper(2048, 8, 10, 9, 11);

void setup() {
  myStepper.setSpeed(10);  // RPM
}

void loop() {
  myStepper.step(2048);  // 1 full revolution
}
```

---

### Comparison Table

| Feature | 3-Pin (STEP/DIR) | 4-Pin (28BYJ-48) |
|---------|------------------|------------------|
| **Arduino Pins** | 2-3 | 4 |
| **Control Interface** | Pulse/Direction | Coil Sequencing |
| **Motor Type** | Bipolar | Unipolar |
| **Voltage** | 8-35V | 5-12V |
| **Library Needed** | No | Yes (Stepper.h) |
| **Code Complexity** | Simple | Moderate |
| **Speed** | Fast (1000+ RPM) | Slow (max ~15 RPM) |
| **Torque** | High | Low-Medium |
| **Precision** | 200-400 steps/rev | 2048 steps/rev |
| **Price** | $5-10 | $2-3 |
| **Best For** | CNC, 3D printers | Arduino projects |
| **Interchangeable** | ❌ NO | ❌ NO |

---

## Wiring Guide

### Physical Connections

```
Arduino Uno          ULN2003 Driver       28BYJ-48 Motor
─────────────        ──────────────       ──────────────
Digital Pin 8 ─────→ IN1
Digital Pin 9 ─────→ IN2
Digital Pin 10 ────→ IN3
Digital Pin 11 ────→ IN4

5V ────────────────→ + (Power)
GND ───────────────→ - (Ground)
                     
                     Motor Socket ──────→ [5-wire connector]
                                          Plug motor in
```

### Breadboard Layout

```
    Arduino                    ULN2003 Board
    ┌─────┐                    ┌──────────┐
    │     │                    │  ○ ○ ○ ○ │ ← LEDs
    │  8 ─┼────────────────────┼→ IN1     │
    │  9 ─┼────────────────────┼→ IN2     │
    │ 10 ─┼────────────────────┼→ IN3     │
    │ 11 ─┼────────────────────┼→ IN4     │
    │     │                    │          │
    │ 5V ─┼────────────────────┼→  +      │
    │GND ─┼────────────────────┼→  -      │
    └─────┘                    │          │
                               │ [Socket] │ ← Motor plug
                               └──────────┘
                                    │
                               28BYJ-48 Motor
                               (5-wire cable)
```

### Pin Assignment Options

**Standard (recommended):**
```cpp
const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;
```

**Alternative (any 4 digital pins):**
```cpp
const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;
```

**Note:** Avoid pins 0 and 1 (used for Serial communication)

---

## Control Methods

### Method 1: Arduino Stepper Library (Easiest)

**Pros:**
- Built into Arduino IDE (no installation)
- Simple to use
- Reliable

**Cons:**
- Blocking code (stops program during movement)
- Basic features only
- No acceleration/deceleration

**When to use:** Simple projects, learning, quick prototypes

---

### Method 2: AccelStepper Library (Advanced)

**Pros:**
- Non-blocking control
- Acceleration and deceleration
- Multiple motors simultaneously
- Advanced positioning features

**Cons:**
- Requires library installation
- More complex code
- Steeper learning curve

**When to use:** Complex projects, smooth motion, multiple motors

---

### Method 3: Manual Coil Control (Educational)

**Pros:**
- Complete understanding of motor operation
- No library dependency
- Maximum control

**Cons:**
- Most complex code
- Easy to make mistakes
- Must implement step sequencing manually

**When to use:** Learning how steppers work, custom applications

---

## Code Examples

### Example 1: Basic Control (Stepper Library)

````cpp
// filepath: c:\Users\Luka\Code\Luka\Arduino\StepMotors\basic-control.ino
/*
  28BYJ-48 Basic Control
  Rotates motor clockwise then counter-clockwise
  Uses built-in Stepper library
*/

#include <Stepper.h>

// Motor has 2048 steps per revolution (with gear reduction)
const int stepsPerRevolution = 2048;

// Initialize stepper on pins 8, 10, 9, 11
// Pin order: IN1, IN3, IN2, IN4 (note: IN3 and IN2 swapped!)
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  // Set motor speed in RPM (revolutions per minute)
  myStepper.setSpeed(10);  // 10 RPM (slow but smooth)
  
  Serial.begin(9600);
  Serial.println("28BYJ-48 Stepper Motor - Basic Control");
}

void loop() {
  Serial.println("Rotating clockwise 1 full revolution...");
  myStepper.step(stepsPerRevolution);  // Positive = clockwise
  delay(1000);
  
  Serial.println("Rotating counter-clockwise 1 full revolution...");
  myStepper.step(-stepsPerRevolution);  // Negative = counter-clockwise
  delay(1000);
  
  Serial.println("Rotating 180 degrees clockwise...");
  myStepper.step(stepsPerRevolution / 2);  // Half revolution
  delay(1000);
  
  Serial.println("Rotating 90 degrees counter-clockwise...");
  myStepper.step(-stepsPerRevolution / 4);  // Quarter revolution
  delay(1000);
}