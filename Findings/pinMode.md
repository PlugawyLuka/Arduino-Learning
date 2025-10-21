# pinMode() - Complete Reference Guide

## Overview

`pinMode()` configures a digital pin to behave as an **INPUT** or **OUTPUT**. It's the first function you must call before reading sensors or controlling outputs.

---

## Table of Contents

1. [Basic Syntax](#basic-syntax)
2. [The Three Modes](#the-three-modes)
3. [How It Works Internally](#how-it-works-internally)
4. [Real-World Examples](#real-world-examples)
5. [Common Use Cases](#common-use-cases)
6. [Common Mistakes](#common-mistakes)
7. [Troubleshooting](#troubleshooting)
8. [Advanced Techniques](#advanced-techniques)

---

## Basic Syntax

```cpp
pinMode(pin, mode)
```

### Parameters
- **pin**: Arduino pin number (0-13 for digital, A0-A5 for analog)
- **mode**: One of three values:
  - `OUTPUT` - Pin sends signals (controls LEDs, motors, etc.)
  - `INPUT` - Pin reads signals (buttons, sensors)
  - `INPUT_PULLUP` - Input with internal resistor enabled

### Returns
Nothing (void function)

### Where to Call
**Always in `setup()` function** (runs once at startup)

---

## The Three Modes

### 1. OUTPUT Mode

```cpp
pinMode(ledPin, OUTPUT);
```

**Use for:**
- LEDs
- Motors (via transistor/relay)
- Sending signals to other devices
- Relays, buzzers, etc.

**Behavior:**
- Can write `HIGH` (5V) or `LOW` (0V)
- Can provide ~40mA current per pin
- Pin becomes "active" - can source or sink current

**Electrical state:**
```
OUTPUT + HIGH:
Pin ──→ 5V (can source current)

OUTPUT + LOW:
Pin ──→ GND (can sink current)
```

**Example:**
```cpp
void setup() {
  pinMode(13, OUTPUT);  // Configure built-in LED
}

void loop() {
  digitalWrite(13, HIGH);  // Turn on
  delay(1000);
  digitalWrite(13, LOW);   // Turn off
  delay(1000);
}
```

---

### 2. INPUT Mode

```cpp
pinMode(buttonPin, INPUT);
```

**Use for:**
- Push buttons (with external pull-up/pull-down resistor)
- Sensors with pull-up resistors
- Reading signals from other devices

**Behavior:**
- **High impedance** (doesn't affect circuit)
- Reads voltage: > 3V = HIGH, < 1.5V = LOW
- **Floating state:** If nothing connected, reads random values!

**Electrical state:**
```
INPUT:
Pin ─┬─ (high impedance, just "listens")
     │
   Nothing connected = FLOATING (unreliable)
```

**Example:**
```cpp
const int buttonPin = 2;

void setup() {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int state = digitalRead(buttonPin);
  Serial.println(state);  // Reads HIGH or LOW
  delay(100);
}
```

**Problem with INPUT:**
```cpp
// Button between pin 2 and GND (no pull-up resistor)
pinMode(2, INPUT);
int state = digitalRead(2);  // Often reads HIGH randomly (floating!)
```

When button is not pressed, pin "floats" (picks up electrical noise from surroundings).

---

### 3. INPUT_PULLUP Mode

```cpp
pinMode(buttonPin, INPUT_PULLUP);
```

**What's different:**
- Activates **internal 20kΩ resistor** pulling pin to 5V
- Pin defaults to HIGH when nothing connected
- Button connects pin to GND → reads LOW when pressed

**Electrical equivalent:**
```
     5V
      │
     20kΩ (internal pullup)
      │
Pin ──┴──── Button ──── GND

Button not pressed: Pin = HIGH (pulled to 5V)
Button pressed:     Pin = LOW (connected to GND)
```

**Why use it:**
- No external resistor needed (saves components)
- More reliable than INPUT (no floating)
- Standard for buttons

**Example:**
```cpp
const int buttonPin = 2;
const int ledPin = 13;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // Internal pullup
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW) {  // Pressed (inverted logic!)
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
```

**Note:** Logic is inverted!
- HIGH = button NOT pressed
- LOW = button pressed

---

## How It Works Internally

### What pinMode() Does

When you call `pinMode()`, the Arduino configures the **microcontroller's data direction register (DDR)**:

```cpp
pinMode(3, OUTPUT);
```

**Behind the scenes:**
1. Arduino looks up which PORT pin 3 belongs to (e.g., PORTD bit 3 on Uno)
2. Sets that bit in the DDR (Data Direction Register) to `1` (output)
3. Pin can now **source or sink current**

```cpp
pinMode(2, INPUT);
```

**What happens:**
1. Sets DDR bit to `0` (input)
2. Pin becomes **high impedance** (doesn't provide voltage, just listens)
3. Can detect HIGH (5V) or LOW (0V) from external sources

### Register-Level View (Arduino Uno)

```
Digital pins 0-7 → PORTD
Digital pins 8-13 → PORTB

pinMode(3, OUTPUT):
DDRD |= (1 << 3);  // Set bit 3 to 1 (output)

pinMode(3, INPUT):
DDRD &= ~(1 << 3); // Set bit 3 to 0 (input)

pinMode(3, INPUT_PULLUP):
DDRD &= ~(1 << 3); // Input
PORTD |= (1 << 3); // Enable pullup
```

---

## Real-World Examples

### Example 1: LED Control (Your Project)

```cpp
int ledPins[] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int pinCount = 10;

void setup() {
  // Set all LED pins as OUTPUT
  for (int i = 0; i < pinCount; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  // Control LEDs with digitalWrite()
  digitalWrite(ledPins[0], HIGH);
}
```

### Example 2: Button with External Pull-Down

```cpp
const int buttonPin = 2;

void setup() {
  pinMode(buttonPin, INPUT);  // External pull-down resistor required
  
  // Circuit:
  // 5V ─── Button ─── Pin 2 ─── 10kΩ resistor ─── GND
}

void loop() {
  int state = digitalRead(buttonPin);
  // HIGH when pressed, LOW when not pressed
}
```

### Example 3: Button with Internal Pull-Up

```cpp
const int buttonPin = 2;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // No external resistor needed!
  
  // Circuit:
  // Pin 2 ─── Button ─── GND
}

void loop() {
  int state = digitalRead(buttonPin);
  // LOW when pressed, HIGH when not pressed (inverted!)
}
```

### Example 4: Sensor Input

```cpp
const int sensorPin = A0;

void setup() {
  pinMode(sensorPin, INPUT);  // Read analog sensor
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(sensorPin);
  Serial.println(value);
}
```

### Example 5: Motor Control via Transistor

```cpp
const int motorPin = 9;

void setup() {
  pinMode(motorPin, OUTPUT);  // Control transistor base
}

void loop() {
  digitalWrite(motorPin, HIGH);  // Motor on
  delay(2000);
  digitalWrite(motorPin, LOW);   // Motor off
  delay(2000);
}
```

---

## Common Use Cases

### OUTPUT Mode Use Cases

1. **LED control**
   ```cpp
   pinMode(13, OUTPUT);
   digitalWrite(13, HIGH);
   ```

2. **Relay control**
   ```cpp
   pinMode(relayPin, OUTPUT);
   digitalWrite(relayPin, HIGH);  // Activate relay
   ```

3. **Buzzer/Speaker**
   ```cpp
   pinMode(buzzerPin, OUTPUT);
   tone(buzzerPin, 1000);  // 1kHz sound
   ```

4. **Motor via driver**
   ```cpp
   pinMode(motorPin, OUTPUT);
   analogWrite(motorPin, 128);  // 50% speed
   ```

### INPUT Mode Use Cases

1. **Button with external pull-up**
   ```cpp
   pinMode(buttonPin, INPUT);
   ```

2. **Digital sensor (motion detector)**
   ```cpp
   pinMode(pirPin, INPUT);
   ```

3. **Receiving serial data**
   ```cpp
   pinMode(rxPin, INPUT);
   ```

### INPUT_PULLUP Use Cases

1. **Button (most common)**
   ```cpp
   pinMode(buttonPin, INPUT_PULLUP);
   ```

2. **Rotary encoder**
   ```cpp
   pinMode(encoderPinA, INPUT_PULLUP);
   pinMode(encoderPinB, INPUT_PULLUP);
   ```

3. **Limit switch**
   ```cpp
   pinMode(limitPin, INPUT_PULLUP);
   ```

---

## Common Mistakes

### Mistake 1: Forgetting pinMode()

```cpp
// WRONG: No pinMode() call
void setup() {
  // Nothing here
}

void loop() {
  digitalWrite(13, HIGH);  // Might not work! Pin direction undefined
}
```

**Why it fails:**
- Pin might default to INPUT mode
- INPUT mode can't source much current
- LED appears dim or doesn't light

**Fix:**
```cpp
void setup() {
  pinMode(13, OUTPUT);  // Always set mode first!
}
```

---

### Mistake 2: Setting pinMode() in loop()

```cpp
// WASTEFUL: Sets mode 1000 times per second
void loop() {
  pinMode(13, OUTPUT);  // Don't do this!
  digitalWrite(13, HIGH);
}
```

**Why it's bad:**
- Wastes processing time
- Should only configure once

**Fix:**
```cpp
void setup() {
  pinMode(13, OUTPUT);  // Set once in setup
}

void loop() {
  digitalWrite(13, HIGH);  // Just use it in loop
}
```

---

### Mistake 3: Wrong Mode for Task

```cpp
// WRONG: LED on INPUT pin
pinMode(ledPin, INPUT);
digitalWrite(ledPin, HIGH);  // Won't provide enough current
```

**Why it fails:**
- INPUT mode is high impedance
- Can't source the ~15-20mA needed for LED
- LED appears very dim or off

**Fix:**
```cpp
pinMode(ledPin, OUTPUT);  // Use OUTPUT for LEDs
```

---

### Mistake 4: No Pull-Up/Pull-Down with INPUT

```cpp
// PROBLEM: Button floating when not pressed
pinMode(buttonPin, INPUT);

void loop() {
  int state = digitalRead(buttonPin);
  // Reads random HIGH/LOW when button not pressed!
}
```

**Why it fails:**
- INPUT mode is high impedance
- Pin picks up electrical noise (acts like antenna)
- Reads random values

**Fix Option 1: Use INPUT_PULLUP**
```cpp
pinMode(buttonPin, INPUT_PULLUP);
// Pin defaults to HIGH, reads LOW when button pressed
```

**Fix Option 2: Add external pull-down resistor**
```cpp
pinMode(buttonPin, INPUT);
// Circuit: Pin ─── 10kΩ ─── GND
```

---

### Mistake 5: Confusing INPUT_PULLUP Logic

```cpp
pinMode(buttonPin, INPUT_PULLUP);

void loop() {
  if (digitalRead(buttonPin) == HIGH) {  // WRONG!
    Serial.println("Button pressed");
  }
}
```

**Why it's wrong:**
- INPUT_PULLUP inverts logic
- HIGH = button NOT pressed
- LOW = button pressed

**Fix:**
```cpp
if (digitalRead(buttonPin) == LOW) {  // Correct
  Serial.println("Button pressed");
}
```

---

## Troubleshooting

### LED Doesn't Light Up

**Check 1: Is pinMode() set to OUTPUT?**
```cpp
void setup() {
  pinMode(ledPin, OUTPUT);  // Must be here!
}
```

**Check 2: Is pinMode() in setup() or loop()?**
```cpp
// RIGHT:
void setup() {
  pinMode(ledPin, OUTPUT);
}

// WRONG:
void loop() {
  pinMode(ledPin, OUTPUT);  // Move to setup!
}
```

**Debug:**
```cpp
void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  Serial.println("pinMode set to OUTPUT");
}
```

---

### Button Reads Random Values

**Problem: Floating input**
```cpp
pinMode(buttonPin, INPUT);  // No pullup/pulldown
```

**Solutions:**

**Option 1: Use INPUT_PULLUP**
```cpp
pinMode(buttonPin, INPUT_PULLUP);
```

**Option 2: Add external resistor**
```cpp
pinMode(buttonPin, INPUT);
// Add 10kΩ resistor: Pin ─── 10kΩ ─── GND
```

**Debug:**
```cpp
void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  Serial.println(digitalRead(buttonPin));
  // Should be stable HIGH when not pressed
  // Should be LOW when pressed
  delay(100);
}
```

---

### Pin Behavior Unexpected

**Check if mode matches usage:**

| Task | Correct Mode | Wrong Mode |
|------|--------------|------------|
| LED | OUTPUT | INPUT |
| Button | INPUT_PULLUP | OUTPUT |
| Sensor | INPUT | OUTPUT |
| Motor (via transistor) | OUTPUT | INPUT |

**Verification code:**
```cpp
void setup() {
  Serial.begin(9600);
  
  // For LED:
  pinMode(ledPin, OUTPUT);
  Serial.println("LED pin set to OUTPUT");
  
  // For button:
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.println("Button pin set to INPUT_PULLUP");
}
```

---

## Advanced Techniques

### Dynamically Changing Pin Mode

```cpp
const int pin = 7;

void setup() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);  // Output 5V
  delay(1000);
  
  pinMode(pin, INPUT);  // Change to input
  int value = digitalRead(pin);  // Read value
}
```

**Use case:** 
- One-wire protocols (DHT11 sensor)
- Capacitive sensing

---

### Using Analog Pins as Digital

```cpp
void setup() {
  pinMode(A0, OUTPUT);  // A0 can be digital output
  digitalWrite(A0, HIGH);
  
  pinMode(A1, INPUT_PULLUP);  // A1 can be digital input
  int state = digitalRead(A1);
}
```

**Note:** Analog pins A0-A5 can be used as digital pins 14-19.

---

### Port Manipulation (Advanced)

```cpp
// Instead of pinMode() for each pin:
pinMode(0, OUTPUT);
pinMode(1, OUTPUT);
pinMode(2, OUTPUT);
// ...

// Use direct register access:
DDRD = B11111111;  // Set pins 0-7 as OUTPUT simultaneously
```

**Pros:**
- Much faster
- Set multiple pins at once

**Cons:**
- Less portable (Arduino-specific)
- Harder to read

---

### Saving Pin States

```cpp
struct PinConfig {
  int pin;
  int mode;
};

PinConfig pins[] = {
  {13, OUTPUT},
  {12, OUTPUT},
  {2, INPUT_PULLUP}
};

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(pins[i].pin, pins[i].mode);
  }
}
```

---

## Safety Considerations

### Never Change Output Pin Under Load

```cpp
// DANGEROUS:
pinMode(motorPin, OUTPUT);
digitalWrite(motorPin, HIGH);  // Motor running
pinMode(motorPin, INPUT);  // Suddenly high impedance - can damage motor driver
```

**Safe approach:**
```cpp
digitalWrite(motorPin, LOW);  // Turn off first
delay(10);
pinMode(motorPin, INPUT);  // Now safe to change
```

---

### INPUT_PULLUP Current

**Internal pullup is weak (20kΩ):**
```
Current = 5V / 20kΩ = 0.25mA
```

**Don't use INPUT_PULLUP for:**
- Powering LEDs (too weak)
- Driving other circuits

**Do use for:**
- Buttons
- Switches
- Weak pull-up where external resistor not available

---

## Quick Reference

| Task | pinMode() Call | Circuit |
|------|---------------|---------|
| LED | `pinMode(pin, OUTPUT)` | Pin → Resistor → LED → GND |
| Button (internal pullup) | `pinMode(pin, INPUT_PULLUP)` | Pin → Button → GND |
| Button (external pulldown) | `pinMode(pin, INPUT)` | 5V → Button → Pin → 10kΩ → GND |
| Sensor | `pinMode(pin, INPUT)` | Varies by sensor |
| Motor (via transistor) | `pinMode(pin, OUTPUT)` | Pin → Transistor base → Motor |

---

## Summary

- **Purpose:** Configure pin direction (input or output)
- **Syntax:** `pinMode(pin, mode)`
- **Modes:** OUTPUT, INPUT, INPUT_PULLUP
- **Where:** Always in setup() function
- **For OUTPUT:** Control LEDs, motors, relays
- **For INPUT:** Read buttons, sensors
- **For INPUT_PULLUP:** Buttons without external resistor
- **Key Point:** Must call before digitalWrite() or digitalRead()

---

## Checklist: Before Using Pin

- [ ] Called `pinMode()` in setup()
- [ ] Mode matches usage (OUTPUT for LEDs, INPUT_PULLUP for buttons)
- [ ] Not setting pinMode() in loop()
- [ ] For INPUT: Using pullup/pulldown (internal or external)
- [ ] For OUTPUT: Using current-limiting resistor with LED
- [ ] Pin number is valid (0-13, A0-A5)

---

**Remember:** pinMode() is the foundation. Always configure your pins before using them! 📌