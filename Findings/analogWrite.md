# analogWrite() - Complete Reference Guide

## Overview

`analogWrite()` outputs a **PWM (Pulse Width Modulation)** signal that simulates an analog voltage between 0V and 5V. It's used for controlling LED brightness, motor speed, and other variable outputs.

---

## Table of Contents

1. [Basic Syntax](#basic-syntax)
2. [How It Works](#how-it-works)
3. [Understanding PWM](#understanding-pwm)
4. [Real-World Examples](#real-world-examples)
5. [Common Use Cases](#common-use-cases)
6. [Common Mistakes](#common-mistakes)
7. [Troubleshooting](#troubleshooting)
8. [Advanced Techniques](#advanced-techniques)
9. [Performance Considerations](#performance-considerations)

---

## Basic Syntax

```cpp
analogWrite(pin, value)
```

### Parameters
- **pin**: PWM-capable pin number
  - Arduino Uno: **3, 5, 6, 9, 10, 11** (marked with ~ on board)
  - Other pins won't work!
- **value**: Duty cycle from **0 to 255**
  - 0 = Always LOW (0V)
  - 127 = 50% duty cycle (~2.5V average)
  - 255 = Always HIGH (5V)

### Returns
Nothing (void function)

### Prerequisites
**Must call `pinMode(pin, OUTPUT)` first!**

---

## How It Works

### PWM Output

`analogWrite()` doesn't output true analog voltage—it rapidly switches between HIGH (5V) and LOW (0V) to **simulate** an analog voltage.

```cpp
analogWrite(ledPin, 128);  // 50% brightness
```

**What Arduino does:**
1. Turns pin HIGH for 50% of the time
2. Turns pin LOW for 50% of the time
3. Switches so fast LED appears half-bright
4. Frequency: ~490 Hz (490 times per second)

### Visual Representation

```
Value = 0 (0% duty cycle):
████████████████████████████ LOW (0V always)

Value = 64 (25% duty cycle):
█▁▁▁█▁▁▁█▁▁▁█▁▁▁█▁▁▁█▁▁▁█▁▁▁ (~1.25V average)

Value = 128 (50% duty cycle):
██▁▁██▁▁██▁▁██▁▁██▁▁██▁▁██▁▁ (~2.5V average)

Value = 192 (75% duty cycle):
███▁███▁███▁███▁███▁███▁███▁ (~3.75V average)

Value = 255 (100% duty cycle):
████████████████████████████ HIGH (5V always)
```

---

## Understanding PWM

### What is PWM?

**PWM (Pulse Width Modulation)** = varying the width of pulses to control average voltage/power.

**Key terms:**
- **Duty cycle:** Percentage of time signal is HIGH
- **Frequency:** How many times per second it switches (Hz)
- **Period:** Time for one complete cycle

### Arduino Uno PWM Specifications

| Specification | Value |
|---------------|-------|
| Resolution | 8-bit (256 steps: 0-255) |
| PWM pins | 6 pins (3, 5, 6, 9, 10, 11) |
| Frequency | ~490 Hz (pins 3,9,10,11) or ~980 Hz (pins 5,6) |
| Voltage | 0V to 5V |
| Current per pin | Max 40mA (20mA recommended) |

### Duty Cycle Calculation

```cpp
Duty cycle (%) = (value / 255) * 100

Value 0:    0% duty cycle   = 0V average
Value 64:   25% duty cycle  = 1.25V average
Value 128:  50% duty cycle  = 2.5V average
Value 192:  75% duty cycle  = 3.75V average
Value 255:  100% duty cycle = 5V average
```

---

### How PWM Controls Brightness

**Human eye persistence of vision:**
- Eye can't see flicker faster than ~50 Hz
- PWM at 490 Hz appears as smooth dimming
- Brain perceives average brightness

**Example:**
```cpp
analogWrite(ledPin, 64);   // 25% → LED appears dim
analogWrite(ledPin, 128);  // 50% → LED appears medium
analogWrite(ledPin, 192);  // 75% → LED appears bright
```

---

### PWM vs. True Analog

**analogWrite() (PWM):**
```
Output: Rapid switching between 0V and 5V
Result: Average voltage (simulated analog)
Use: LEDs, motors, buzzers
```

**True analog (Arduino doesn't have):**
```
Output: Continuous voltage (e.g., 2.5V exactly)
Result: Steady voltage at any level
Use: Requires external DAC (Digital-to-Analog Converter)
```

---

## Real-World Examples

### Example 1: Fading LED

```cpp
const int ledPin = 9;  // PWM pin

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Fade in (0 to 255)
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(ledPin, brightness);
    delay(5);
  }
  
  // Fade out (255 to 0)
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(ledPin, brightness);
    delay(5);
  }
}
```

---

### Example 2: Potentiometer Controls LED Brightness

```cpp
const int potPin = A0;
const int ledPin = 9;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Read potentiometer (0-1023)
  int sensorValue = analogRead(potPin);
  
  // Map to PWM range (0-255)
  int brightness = map(sensorValue, 0, 1023, 0, 255);
  
  // Set LED brightness
  analogWrite(ledPin, brightness);
  
  Serial.print("Pot: ");
  Serial.print(sensorValue);
  Serial.print(" | Brightness: ");
  Serial.println(brightness);
  
  delay(10);
}
```

---

### Example 3: RGB LED Color Mixing

```cpp
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // Red
  setColor(255, 0, 0);
  delay(1000);
  
  // Green
  setColor(0, 255, 0);
  delay(1000);
  
  // Blue
  setColor(0, 0, 255);
  delay(1000);
  
  // Yellow (red + green)
  setColor(255, 255, 0);
  delay(1000);
  
  // Purple (red + blue)
  setColor(255, 0, 255);
  delay(1000);
  
  // Cyan (green + blue)
  setColor(0, 255, 255);
  delay(1000);
  
  // White (all on)
  setColor(255, 255, 255);
  delay(1000);
}

void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
```

---

### Example 4: Motor Speed Control

```cpp
const int motorPin = 9;  // Connected to motor driver
const int potPin = A0;

void setup() {
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(potPin);
  
  // Map to motor speed (0-255)
  int motorSpeed = map(potValue, 0, 1023, 0, 255);
  
  analogWrite(motorPin, motorSpeed);
  
  Serial.print("Speed: ");
  Serial.println(motorSpeed);
  
  delay(50);
}
```

---

### Example 5: Breathing LED Effect

```cpp
const int ledPin = 9;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Smooth breathing effect using sine wave
  for (int i = 0; i < 360; i++) {
    float rad = i * (PI / 180);
    int brightness = (sin(rad) + 1) * 127.5;  // 0-255 range
    
    analogWrite(ledPin, brightness);
    delay(10);
  }
}
```

---

## Common Use Cases

### 1. LED Brightness Control

```cpp
analogWrite(ledPin, 128);  // 50% brightness
```

### 2. Motor Speed Control

```cpp
int speed = map(sensorValue, 0, 1023, 0, 255);
analogWrite(motorPin, speed);
```

### 3. RGB LED Color

```cpp
analogWrite(redPin, 255);    // Full red
analogWrite(greenPin, 128);  // Half green
analogWrite(bluePin, 0);     // No blue
// Result: Orange color
```

### 4. Servo Control (Use Servo library instead)

```cpp
// Don't use analogWrite() for servos!
// Use Servo library:
#include <Servo.h>
Servo myServo;
myServo.attach(9);
myServo.write(90);  // 90 degree position
```

### 5. Audio Tone Generation

```cpp
// Simple tone (better to use tone() function)
analogWrite(speakerPin, 128);  // 50% duty = square wave
```

### 6. Heating Element Control

```cpp
int temperature = readTemp();
int heaterPower = map(temperature, 20, 30, 255, 0);  // Inverse
heaterPower = constrain(heaterPower, 0, 255);
analogWrite(heaterPin, heaterPower);
```

### 7. Fan Speed Control

```cpp
int temp = readTemperature();
int fanSpeed = map(temp, 25, 35, 0, 255);  // Faster when hotter
fanSpeed = constrain(fanSpeed, 0, 255);
analogWrite(fanPin, fanSpeed);
```

---

## Common Mistakes

### Mistake 1: Using Non-PWM Pin

```cpp
// WRONG: Pin 2 doesn't support PWM
pinMode(2, OUTPUT);
analogWrite(2, 128);  // Won't work! Nothing happens
```

**Arduino Uno PWM pins:** 3, 5, 6, 9, 10, 11 only!

**Fix:**
```cpp
pinMode(9, OUTPUT);   // Use PWM-capable pin
analogWrite(9, 128);  // Works!
```

**How to identify PWM pins:**
- Look for **~** symbol on Arduino board
- Check pinout diagram for your model

---

### Mistake 2: Forgetting pinMode()

```cpp
// WRONG: No pinMode() call
void setup() {
  // Nothing here
}

void loop() {
  analogWrite(9, 128);  // Might not work properly
}
```

**Fix:**
```cpp
void setup() {
  pinMode(9, OUTPUT);  // Always set mode first!
}
```

---

### Mistake 3: Using analogRead() Range

```cpp
// WRONG: analogWrite() uses 0-255, not 0-1023
int sensorValue = analogRead(A0);  // 0-1023
analogWrite(ledPin, sensorValue);  // Value too high!
```

**What happens:**
- Values above 255 are clamped to 255
- LED always at full brightness when pot > 25%

**Fix:**
```cpp
int sensorValue = analogRead(A0);
int brightness = map(sensorValue, 0, 1023, 0, 255);
analogWrite(ledPin, brightness);
```

---

### Mistake 4: Expecting True Analog Output

```cpp
// WRONG ASSUMPTION: analogWrite() outputs steady voltage
analogWrite(9, 128);
// Does NOT output steady 2.5V!
// Outputs rapid pulses averaging 2.5V
```

**Problem:**
- Won't work for precision analog circuits
- May cause issues with some sensors

**Solution:**
- For true analog: Use external DAC chip
- Or: Low-pass filter (resistor + capacitor)

**Simple low-pass filter:**
```
Pin 9 ──── 1kΩ ──┬── To device
                 │
                100μF
                 │
                GND

Smooths PWM to approximate analog voltage
```

---

### Mistake 5: Wrong Value Range

```cpp
// WRONG: Value outside 0-255 range
analogWrite(ledPin, 300);   // Clamped to 255
analogWrite(ledPin, -50);   // Clamped to 0
```

**Best practice: Use constrain()**
```cpp
int value = calculateValue();
value = constrain(value, 0, 255);
analogWrite(ledPin, value);
```

---

### Mistake 6: Using analogWrite() for Servos

```cpp
// WRONG: analogWrite() not designed for servos
analogWrite(servoPin, 90);  // Doesn't position servo correctly
```

**Why it fails:**
- Servos need specific pulse widths (1-2ms)
- PWM frequency is wrong for servos
- Won't move to correct angle

**Fix: Use Servo library**
```cpp
#include <Servo.h>
Servo myServo;

void setup() {
  myServo.attach(9);
  myServo.write(90);  // Correct way to control servo
}
```

---

### Mistake 7: No Current Limiting

```cpp
// DANGER: LED without resistor
pinMode(ledPin, OUTPUT);
analogWrite(ledPin, 255);  // Can damage LED or Arduino pin
```

**Circuit:**
```
Pin ──── LED ──── (no resistor!) ──── GND  ❌ WRONG

Pin ──── 220Ω ──── LED ──── GND  ✓ CORRECT
```

**Even with PWM, always use current-limiting resistor!**

---

## Troubleshooting

### LED Doesn't Dim/Brighten

**Check 1: Using PWM pin?**
```cpp
// Only these pins support PWM:
// 3, 5, 6, 9, 10, 11
```

**Check 2: pinMode() set to OUTPUT?**
```cpp
void setup() {
  pinMode(9, OUTPUT);  // Must be here!
}
```

**Check 3: Value in correct range?**
```cpp
// Should be 0-255
analogWrite(ledPin, 128);  // Good
analogWrite(ledPin, 512);  // Bad (too high, clamped to 255)
```

**Debug:**
```cpp
void setup() {
  pinMode(9, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i <= 255; i += 51) {
    Serial.print("Setting brightness to: ");
    Serial.println(i);
    analogWrite(9, i);
    delay(1000);
  }
}
```

---

### LED Flickers

**Cause 1: Low PWM frequency perceptible**
- Some people can see 490 Hz flicker
- Solution: Increase frequency (advanced, requires register manipulation)

**Cause 2: Poor power supply**
```cpp
// Fix: Add capacitor near Arduino
// 100μF between 5V and GND
```

**Cause 3: Rapid changes in code**
```cpp
// Bad: Changing too fast
void loop() {
  analogWrite(ledPin, random(0, 255));  // Flickers randomly
}

// Good: Smooth transitions
void loop() {
  for (int i = 0; i <= 255; i++) {
    analogWrite(ledPin, i);
    delay(5);
  }
}
```

---

### Motor Whines/Makes Noise

**Cause: PWM frequency too low**
- 490 Hz is audible to motors
- Motor windings vibrate at PWM frequency

**Solution 1: Use higher frequency (advanced)**
```cpp
// Change Timer 1 frequency (pins 9, 10)
// WARNING: Advanced, affects other functions
TCCR1B = (TCCR1B & 0xF8) | 0x01;  // ~31 kHz
```

**Solution 2: Add capacitor across motor terminals**
```
Motor: 100μF capacitor between motor wires
Reduces electrical noise
```

---

### RGB LED Wrong Colors

**Problem: Common anode vs. common cathode**

**Common cathode (most common):**
```cpp
analogWrite(redPin, 255);   // Full brightness
analogWrite(redPin, 0);     // Off
```

**Common anode (inverted):**
```cpp
analogWrite(redPin, 0);     // Full brightness (inverted!)
analogWrite(redPin, 255);   // Off
```

**Fix for common anode:**
```cpp
void setColor(int red, int green, int blue) {
  analogWrite(redPin, 255 - red);      // Invert
  analogWrite(greenPin, 255 - green);  // Invert
  analogWrite(bluePin, 255 - blue);    // Invert
}
```

---

### Value Not Changing Output

**Check: Is value actually changing?**
```cpp
void loop() {
  int brightness = analogRead(A0);
  
  Serial.print("Raw: ");
  Serial.print(brightness);  // Check if reading changes
  
  int mapped = map(brightness, 0, 1023, 0, 255);
  Serial.print(" | Mapped: ");
  Serial.println(mapped);
  
  analogWrite(9, mapped);
  delay(100);
}
```

---

## Advanced Techniques

### Smooth Fading with Easing

```cpp
const int ledPin = 9;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Ease in (starts slow, speeds up)
  for (int i = 0; i <= 255; i++) {
    int eased = (i * i) / 255;  // Quadratic easing
    analogWrite(ledPin, eased);
    delay(5);
  }
  
  delay(500);
  
  // Ease out (starts fast, slows down)
  for (int i = 255; i >= 0; i--) {
    int eased = (i * i) / 255;
    analogWrite(ledPin, eased);
    delay(5);
  }
  
  delay(500);
}
```

---

### Gamma Correction for Perceived Brightness

```cpp
// Human eye perceives brightness logarithmically
const byte gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255
};

void loop() {
  for (int i = 0; i <= 255; i++) {
    int corrected = gamma8[i];  // Apply gamma correction
    analogWrite(ledPin, corrected);
    delay(10);
  }
}
```

**Why gamma correction:**
- Without: Brightness appears to "jump" at low values
- With: Smooth, linear perceived brightness

---

### Non-Blocking Fade

```cpp
const int ledPin = 9;
unsigned long previousMillis = 0;
int brightness = 0;
int fadeAmount = 5;

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= 30) {
    previousMillis = currentMillis;
    
    analogWrite(ledPin, brightness);
    
    brightness += fadeAmount;
    
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;  // Reverse direction
    }
  }
  
  // Other code can run here without blocking!
}
```

---

### PWM Frequency Change (Advanced)

```cpp
void setup() {
  pinMode(9, OUTPUT);
  
  // Increase PWM frequency on pins 9 and 10
  // Timer 1: default ~490 Hz
  
  // For ~31 kHz:
  TCCR1B = (TCCR1B & 0xF8) | 0x01;
  
  // For ~4 kHz:
  // TCCR1B = (TCCR1B & 0xF8) | 0x02;
}

void loop() {
  analogWrite(9, 128);
  // Now running at higher frequency (less noise)
}
```

**Warning:** Changing timer frequency affects:
- `delay()` accuracy
- `millis()` accuracy
- Servo library (uses same timer)

---

### Dithering for More Perceived Colors

```cpp
// Simulate 16-bit PWM using 8-bit PWM + dithering
const int ledPin = 9;

void set16bitPWM(int pin, int value) {
  // value: 0-65535
  int high = value >> 8;       // Upper 8 bits
  int low = value & 0xFF;      // Lower 8 bits
  
  for (int i = 0; i < 256; i++) {
    if (i < low) {
      analogWrite(pin, high + 1);
    } else {
      analogWrite(pin, high);
    }
    delayMicroseconds(10);
  }
}
```

---

### Multiple LEDs with Individual Brightness

```cpp
const int numLEDs = 6;
int ledPins[numLEDs] = {3, 5, 6, 9, 10, 11};
int brightness[numLEDs] = {255, 200, 150, 100, 50, 0};

void setup() {
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  // Set each LED to its brightness
  for (int i = 0; i < numLEDs; i++) {
    analogWrite(ledPins[i], brightness[i]);
  }
  
  // Rotate brightness values
  int temp = brightness[0];
  for (int i = 0; i < numLEDs - 1; i++) {
    brightness[i] = brightness[i + 1];
  }
  brightness[numLEDs - 1] = temp;
  
  delay(200);
}
```

---

## Performance Considerations

### PWM Speed

**analogWrite() is very fast:**
```cpp
analogWrite(pin, value);  // Takes only ~4 microseconds
```

**Can update thousands of times per second:**
```cpp
void loop() {
  analogWrite(9, random(0, 256));
  // Loop runs ~250,000 times per second
  // (limited mainly by random() function)
}
```

---

### PWM Resolution

**Arduino Uno: 8-bit (256 levels)**
```
Brightness steps: 0, 1, 2, 3, ... 253, 254, 255
Voltage resolution: 5V / 256 = ~19.5mV per step
```

**Higher resolution boards:**
- Arduino Due: 12-bit (4096 levels)
- ESP32: 16-bit (65536 levels) on some pins

---

### Simultaneous PWM Outputs

**Arduino Uno can drive all 6 PWM pins simultaneously:**
```cpp
void loop() {
  analogWrite(3, 100);
  analogWrite(5, 150);
  analogWrite(6, 200);
  analogWrite(9, 50);
  analogWrite(10, 75);
  analogWrite(11, 225);
  // All update independently
}
```

---

## Comparison Table

| Feature | analogWrite() | digitalWrite() |
|---------|---------------|----------------|
| Output type | PWM (simulated analog) | Digital (HIGH/LOW) |
| Value range | 0-255 | HIGH/LOW (1/0) |
| Pins | PWM pins only (3,5,6,9,10,11) | All digital pins |
| Use case | Variable brightness/speed | On/off control |
| Frequency | ~490 Hz or ~980 Hz | N/A (steady) |
| Current | Max 40mA per pin | Max 40mA per pin |

---

## Quick Reference

| Value | Duty Cycle | Average Voltage | LED Brightness |
|-------|------------|-----------------|----------------|
| 0 | 0% | 0.0V | Off |
| 32 | 12.5% | 0.625V | Very dim |
| 64 | 25% | 1.25V | Dim |
| 128 | 50% | 2.5V | Medium |
| 192 | 75% | 3.75V | Bright |
| 255 | 100% | 5.0V | Full brightness |

**Common mappings:**
```cpp
// Potentiometer to LED brightness
int brightness = map(analogRead(A0), 0, 1023, 0, 255);
analogWrite(ledPin, brightness);

// Percentage to PWM
int percent = 75;
int pwmValue = (percent * 255) / 100;
analogWrite(ledPin, pwmValue);

// Voltage to PWM
float voltage = 3.3;
int pwmValue = (voltage / 5.0) * 255;
analogWrite(ledPin, pwmValue);
```

---

## Summary

- **Purpose:** Output PWM signal to simulate analog voltage
- **Syntax:** `analogWrite(pin, value)`
- **Value range:** 0-255 (8-bit resolution)
- **PWM pins:** 3, 5, 6, 9, 10, 11 (Arduino Uno)
- **Frequency:** ~490 Hz (most pins) or ~980 Hz (pins 5, 6)
- **Prerequisites:** Must call `pinMode(pin, OUTPUT)` first
- **Use for:** LED dimming, motor speed, RGB colors
- **Not true analog:** Rapid switching between 0V and 5V
- **Current limit:** 40mA max per pin (20mA recommended)
- **Always use:** Current-limiting resistor with LEDs

---

## Checklist: Before Using analogWrite()

- [ ] Pin is PWM-capable (3, 5, 6, 9, 10, 11 on Uno)
- [ ] Called `pinMode(pin, OUTPUT)` in setup()
- [ ] Value is 0-255 range
- [ ] Using `map()` if converting from analogRead() (0-1023)
- [ ] Current-limiting resistor with LEDs (220Ω-1kΩ)
- [ ] Understanding it's PWM, not true analog
- [ ] Load current < 40mA per pin
- [ ] Not using for servos (use Servo library instead)
- [ ] Considering gamma correction for perceived brightness

---

**Remember:** analogWrite() creates PWM, not true analog voltage. It's perfect for LEDs and motors, but always use proper current limiting and the correct pins! 💡