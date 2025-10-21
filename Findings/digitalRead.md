# digitalRead() - Complete Reference Guide

## Overview

`digitalRead()` reads the state of a digital pin and returns **HIGH** (5V) or **LOW** (0V). It's the fundamental function for reading buttons, switches, and digital sensors.

---

## Table of Contents

1. [Basic Syntax](#basic-syntax)
2. [How It Works](#how-it-works)
3. [Electrical Behavior](#electrical-behavior)
4. [Real-World Examples](#real-world-examples)
5. [Common Use Cases](#common-use-cases)
6. [Common Mistakes](#common-mistakes)
7. [Troubleshooting](#troubleshooting)
8. [Advanced Techniques](#advanced-techniques)
9. [Reading Reliability](#reading-reliability)

---

## Basic Syntax

```cpp
digitalRead(pin)
```

### Parameters
- **pin**: Pin number to read (digital pins 0-13, analog pins A0-A5 can also be used as digital)

### Returns
- `HIGH` (1) if pin voltage > ~3V
- `LOW` (0) if pin voltage < ~1.5V

### Prerequisites
**Must call `pinMode(pin, INPUT)` or `pinMode(pin, INPUT_PULLUP)` first!**

---

## How It Works

### Internal Process

```cpp
int state = digitalRead(2);
```

**What happens step-by-step:**
1. Arduino reads the PORT input register (PIN register)
2. Checks the bit corresponding to pin 2
3. Returns `HIGH` (1) if bit is set, `LOW` (0) if cleared
4. Voltage threshold: > 3V = HIGH, < 1.5V = LOW

### Memory Representation

```
Arduino Uno - Digital Pin 2:
┌─────────────────────────────────┐
│ PIND Register (8 bits)          │
│ [D7][D6][D5][D4][D3][D2][D1][D0]│
│                      ↑           │
│                    Pin 2         │
└─────────────────────────────────┘

digitalRead(2) → Read bit 2
Returns: HIGH (1) or LOW (0)
```

---

## Electrical Behavior

### Voltage Thresholds

```
5V ─────────────────────────── HIGH (reads as 1)
          ↑
       ~3V threshold
          ↓
       ~1.5V threshold
          ↑
0V ─────────────────────────── LOW (reads as 0)

Between 1.5V-3V = UNDEFINED (unreliable)
```

### Input Impedance

**INPUT mode:**
- Very high impedance (~100MΩ)
- Doesn't draw current from source
- Acts like a "listening antenna"

**INPUT_PULLUP mode:**
- 20kΩ resistor to 5V
- Pulls pin HIGH when nothing connected
- Current when LOW: 5V / 20kΩ = 0.25mA

---

### Reading with INPUT Mode

```cpp
pinMode(buttonPin, INPUT);
int state = digitalRead(buttonPin);
```

**Circuit requirement:**
```
      5V
       │
      10kΩ (external pull-up resistor)
       │
Pin ───┴──── Button ──── GND

Button not pressed: Pin = HIGH (pulled to 5V)
Button pressed:     Pin = LOW (connected to GND)
```

**Problem without pullup:**
```
Pin ────  (floating, nothing connected)

Result: Reads random HIGH/LOW (picks up electrical noise)
```

---

### Reading with INPUT_PULLUP Mode

```cpp
pinMode(buttonPin, INPUT_PULLUP);
int state = digitalRead(buttonPin);
```

**Circuit (no external resistor needed):**
```
     5V (internal)
       │
      20kΩ (internal pull-up)
       │
Pin ───┴──── Button ──── GND

Button not pressed: Pin = HIGH (pulled to 5V internally)
Button pressed:     Pin = LOW (connected to GND)
```

**Note:** Logic is inverted!
- HIGH = button NOT pressed
- LOW = button pressed

---

## Real-World Examples

### Example 1: Reading a Button

```cpp
const int buttonPin = 2;
const int ledPin = 13;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // Button input
  pinMode(ledPin, OUTPUT);           // LED output
  Serial.begin(9600);
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  
  Serial.print("Button state: ");
  Serial.println(buttonState);  // 1 (HIGH) or 0 (LOW)
  
  if (buttonState == LOW) {  // Pressed (INPUT_PULLUP inverted)
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  
  delay(10);
}
```

---

### Example 2: Toggle LED on Button Press

```cpp
const int buttonPin = 2;
const int ledPin = 13;
bool ledState = LOW;
bool lastButtonState = HIGH;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  
  // Detect button press (HIGH to LOW transition)
  if (buttonState == LOW && lastButtonState == HIGH) {
    ledState = !ledState;  // Toggle
    digitalWrite(ledPin, ledState);
    delay(50);  // Simple debounce
  }
  
  lastButtonState = buttonState;
}
```

---

### Example 3: Reading Multiple Buttons

```cpp
const int buttonPins[] = {2, 3, 4, 5};
const int numButtons = 4;

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void loop() {
  for (int i = 0; i < numButtons; i++) {
    int state = digitalRead(buttonPins[i]);
    
    if (state == LOW) {  // Button pressed
      Serial.print("Button ");
      Serial.print(i);
      Serial.println(" pressed");
    }
  }
  
  delay(100);
}
```

---

### Example 4: Motion Sensor (PIR)

```cpp
const int pirPin = 2;
const int ledPin = 13;

void setup() {
  pinMode(pirPin, INPUT);      // PIR sensor
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  
  Serial.println("Warming up sensor...");
  delay(2000);  // PIR needs calibration time
  Serial.println("Ready!");
}

void loop() {
  int motion = digitalRead(pirPin);
  
  if (motion == HIGH) {
    Serial.println("Motion detected!");
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  
  delay(100);
}
```

---

### Example 5: Limit Switch

```cpp
const int limitPin = 2;
const int motorPin = 9;

void setup() {
  pinMode(limitPin, INPUT_PULLUP);  // Limit switch
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int limitState = digitalRead(limitPin);
  
  if (limitState == LOW) {  // Switch activated
    Serial.println("Limit reached! Stopping motor.");
    digitalWrite(motorPin, LOW);  // Stop motor
  } else {
    digitalWrite(motorPin, HIGH);  // Run motor
  }
}
```

---

## Common Use Cases

### 1. Button Input

```cpp
pinMode(buttonPin, INPUT_PULLUP);
int pressed = digitalRead(buttonPin);
if (pressed == LOW) {
  // Button pressed
}
```

### 2. Switch Reading

```cpp
pinMode(switchPin, INPUT_PULLUP);
int position = digitalRead(switchPin);
// HIGH = switch open, LOW = switch closed
```

### 3. Sensor Input (PIR, Reed, etc.)

```cpp
pinMode(sensorPin, INPUT);
int detected = digitalRead(sensorPin);
if (detected == HIGH) {
  // Event detected
}
```

### 4. Encoder Reading

```cpp
pinMode(encoderA, INPUT_PULLUP);
pinMode(encoderB, INPUT_PULLUP);
int stateA = digitalRead(encoderA);
int stateB = digitalRead(encoderB);
// Determine rotation direction
```

### 5. Communication Signals

```cpp
pinMode(dataPin, INPUT);
int bit = digitalRead(dataPin);
// Read data bit from another device
```

---

## Common Mistakes

### Mistake 1: Forgetting pinMode()

```cpp
// WRONG: No pinMode() call
void setup() {
  Serial.begin(9600);
}

void loop() {
  int state = digitalRead(2);  // Pin direction undefined!
  Serial.println(state);
}
```

**Why it fails:**
- Pin might be in OUTPUT mode
- Reads unreliable values
- Could damage circuit if OUTPUT pin drives against another OUTPUT

**Fix:**
```cpp
void setup() {
  pinMode(2, INPUT_PULLUP);  // Always set mode first!
  Serial.begin(9600);
}
```

---

### Mistake 2: No Pull-up/Pull-down Resistor

```cpp
// PROBLEM: Button floating when not pressed
pinMode(buttonPin, INPUT);  // No pullup!

void loop() {
  int state = digitalRead(buttonPin);
  // Reads random HIGH/LOW when button not pressed!
}
```

**Why it fails:**
- INPUT mode is high impedance
- Pin acts like antenna, picks up noise
- Reads unpredictable values

**Fix Option 1: Use INPUT_PULLUP**
```cpp
pinMode(buttonPin, INPUT_PULLUP);
```

**Fix Option 2: Add external resistor**
```cpp
pinMode(buttonPin, INPUT);
// Circuit: Pin ──── 10kΩ ──── GND (pull-down)
// Or:      Pin ──── 10kΩ ──── 5V  (pull-up)
```

---

### Mistake 3: Confusing INPUT_PULLUP Logic

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

### Mistake 4: No Debouncing

```cpp
// PROBLEM: Mechanical switches "bounce"
void loop() {
  if (digitalRead(buttonPin) == LOW) {
    counter++;  // Increments multiple times per press!
  }
}
```

**Why it fails:**
- Mechanical contacts bounce 5-50ms
- Reads multiple presses for one physical press

**Fix: Simple delay debounce**
```cpp
if (digitalRead(buttonPin) == LOW) {
  counter++;
  delay(50);  // Wait for bounce to settle
  while (digitalRead(buttonPin) == LOW);  // Wait for release
}
```

**Fix: Better debounce (non-blocking)**
```cpp
bool lastState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void loop() {
  int reading = digitalRead(buttonPin);
  
  if (reading != lastState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW) {
      // Stable LOW reading = button pressed
      counter++;
    }
  }
  
  lastState = reading;
}
```

---

### Mistake 5: Reading Too Fast

```cpp
// WASTEFUL: Reading thousands of times per second
void loop() {
  int state = digitalRead(buttonPin);
  Serial.println(state);  // No delay!
}
```

**Problems:**
- Floods Serial Monitor
- Wastes CPU time
- Hard to see changes

**Fix:**
```cpp
void loop() {
  int state = digitalRead(buttonPin);
  Serial.println(state);
  delay(100);  // Read 10 times per second
}
```

---

### Mistake 6: Wrong Pin Mode for Sensor

```cpp
// Some sensors need INPUT, not INPUT_PULLUP
pinMode(pirPin, INPUT_PULLUP);  // WRONG for PIR sensor!
int motion = digitalRead(pirPin);
```

**Why it fails:**
- PIR sensors provide their own voltage output
- Internal pullup interferes with sensor signal

**Fix:**
```cpp
pinMode(pirPin, INPUT);  // Use INPUT for active sensors
```

---

## Troubleshooting

### Button Reads Random Values

**Check 1: Is pull-up/pull-down configured?**
```cpp
pinMode(buttonPin, INPUT_PULLUP);  // Add this!
```

**Check 2: Circuit correct?**
```
With INPUT_PULLUP:
Pin ──── Button ──── GND

With INPUT + external pull-down:
5V ──── Button ──── Pin ──── 10kΩ ──── GND
```

**Debug:**
```cpp
void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  int state = digitalRead(buttonPin);
  Serial.print("Pin state: ");
  Serial.println(state);  // Should be stable HIGH when not pressed
  delay(100);
}
```

---

### Button Always Reads HIGH (or LOW)

**Problem 1: Wrong circuit**
```cpp
// With INPUT_PULLUP:
// Button between pin and 5V (WRONG!)
// Should be: Button between pin and GND
```

**Problem 2: Button not connected**
- Check wiring
- Button might be damaged

**Problem 3: Wrong pin**
```cpp
pinMode(2, INPUT_PULLUP);
int state = digitalRead(3);  // Reading wrong pin!
```

**Debug:**
```cpp
void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  Serial.print("Button (pin ");
  Serial.print(buttonPin);
  Serial.print("): ");
  Serial.println(digitalRead(buttonPin));
  
  // Manually connect pin to GND with wire
  // Should read LOW
  
  delay(500);
}
```

---

### Multiple Triggers for Single Press

**Cause: No debouncing**

**Solution 1: Simple delay**
```cpp
if (digitalRead(buttonPin) == LOW) {
  doSomething();
  delay(200);  // Debounce delay
  while (digitalRead(buttonPin) == LOW);  // Wait for release
}
```

**Solution 2: State machine**
```cpp
bool lastButtonState = HIGH;

void loop() {
  bool currentState = digitalRead(buttonPin);
  
  if (currentState == LOW && lastButtonState == HIGH) {
    doSomething();  // Only triggers once per press
  }
  
  lastButtonState = currentState;
  delay(10);
}
```

---

### Sensor Not Responding

**Check 1: Correct pinMode()**
```cpp
// Active sensors (PIR, ultrasonic):
pinMode(sensorPin, INPUT);  // Not INPUT_PULLUP

// Passive sensors (reed switch):
pinMode(sensorPin, INPUT_PULLUP);  // Needs pullup
```

**Check 2: Sensor powered?**
```cpp
// Many sensors need 5V and GND connections
// Check power wiring
```

**Check 3: Sensor warm-up time?**
```cpp
// PIR sensors need 30-60 seconds to stabilize
void setup() {
  pinMode(pirPin, INPUT);
  Serial.println("Warming up...");
  delay(60000);  // 60 seconds
  Serial.println("Ready!");
}
```

**Debug:**
```cpp
void loop() {
  int raw = digitalRead(sensorPin);
  Serial.print("Raw sensor: ");
  Serial.println(raw);
  
  // Should change between HIGH/LOW when triggered
  
  delay(100);
}
```

---

## Advanced Techniques

### Debouncing with millis()

```cpp
const int buttonPin = 2;
int buttonState = HIGH;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void loop() {
  int reading = digitalRead(buttonPin);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      
      if (buttonState == LOW) {
        Serial.println("Button pressed (debounced)");
      }
    }
  }
  
  lastButtonState = reading;
}
```

---

### Edge Detection (Rising/Falling)

```cpp
int lastState = HIGH;

void loop() {
  int currentState = digitalRead(buttonPin);
  
  // Detect falling edge (HIGH to LOW)
  if (currentState == LOW && lastState == HIGH) {
    Serial.println("Button pressed (falling edge)");
  }
  
  // Detect rising edge (LOW to HIGH)
  if (currentState == HIGH && lastState == LOW) {
    Serial.println("Button released (rising edge)");
  }
  
  lastState = currentState;
  delay(10);
}
```

---

### Long Press Detection

```cpp
const int buttonPin = 2;
unsigned long pressStartTime = 0;
bool buttonWasPressed = false;
const unsigned long longPressTime = 1000;  // 1 second

void loop() {
  int buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW) {  // Button pressed
    if (!buttonWasPressed) {
      pressStartTime = millis();
      buttonWasPressed = true;
    }
    
    unsigned long pressDuration = millis() - pressStartTime;
    
    if (pressDuration >= longPressTime) {
      Serial.println("Long press detected!");
      delay(100);  // Prevent multiple triggers
    }
  } else {  // Button released
    if (buttonWasPressed) {
      unsigned long pressDuration = millis() - pressStartTime;
      
      if (pressDuration < longPressTime) {
        Serial.println("Short press");
      }
      
      buttonWasPressed = false;
    }
  }
}
```

---

### Double-Click Detection

```cpp
const int buttonPin = 2;
unsigned long lastPressTime = 0;
unsigned long doubleClickWindow = 300;  // 300ms
int clickCount = 0;

void loop() {
  static int lastState = HIGH;
  int currentState = digitalRead(buttonPin);
  
  // Detect button press (falling edge)
  if (currentState == LOW && lastState == HIGH) {
    unsigned long currentTime = millis();
    
    if (currentTime - lastPressTime < doubleClickWindow) {
      clickCount++;
      
      if (clickCount == 2) {
        Serial.println("Double-click detected!");
        clickCount = 0;
      }
    } else {
      clickCount = 1;
    }
    
    lastPressTime = currentTime;
    delay(50);  // Debounce
  }
  
  // Reset if too much time passed
  if (millis() - lastPressTime > doubleClickWindow && clickCount > 0) {
    if (clickCount == 1) {
      Serial.println("Single-click");
    }
    clickCount = 0;
  }
  
  lastState = currentState;
}
```

---

### Reading Multiple Inputs Efficiently

```cpp
const byte inputPins[] = {2, 3, 4, 5, 6};
const int numInputs = 5;
byte lastStates[numInputs];

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < numInputs; i++) {
    pinMode(inputPins[i], INPUT_PULLUP);
    lastStates[i] = HIGH;
  }
}

void loop() {
  for (int i = 0; i < numInputs; i++) {
    byte currentState = digitalRead(inputPins[i]);
    
    if (currentState != lastStates[i]) {
      if (currentState == LOW) {
        Serial.print("Input ");
        Serial.print(i);
        Serial.println(" activated");
      }
      
      lastStates[i] = currentState;
    }
  }
  
  delay(10);
}
```

---

### Interrupt-Based Reading

```cpp
const int buttonPin = 2;  // Must be interrupt pin (2 or 3 on Uno)
volatile bool buttonPressed = false;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);
  Serial.begin(9600);
}

void loop() {
  if (buttonPressed) {
    Serial.println("Button pressed (via interrupt)");
    buttonPressed = false;
  }
  
  // Main code runs without constantly checking button!
}

void buttonISR() {
  buttonPressed = true;
}
```

---

## Reading Reliability

### Factors Affecting Reads

**1. Electrical noise**
- Long wires act as antennas
- Solution: Keep wires short, use shielded cable

**2. Contact bounce**
- Mechanical switches bounce 5-50ms
- Solution: Software or hardware debouncing

**3. Floating inputs**
- No pull-up/pull-down = unreliable
- Solution: Always use INPUT_PULLUP or external resistor

**4. Voltage levels**
- Marginal voltages (1.5V-3V) are undefined
- Solution: Ensure clean HIGH (>4V) or LOW (<1V) signals

---

### Best Practices

**1. Always use pull-up or pull-down**
```cpp
pinMode(pin, INPUT_PULLUP);  // Preferred for buttons
```

**2. Debounce mechanical switches**
```cpp
delay(50);  // Simple but effective
```

**3. Use edge detection for actions**
```cpp
// Don't:
if (digitalRead(buttonPin) == LOW) {
  counter++;  // Increments continuously while held
}

// Do:
if (currentState == LOW && lastState == HIGH) {
  counter++;  // Increments once per press
}
```

**4. Add delays for Serial debugging**
```cpp
Serial.println(digitalRead(pin));
delay(100);  // Don't flood Serial Monitor
```

**5. Check voltage with multimeter**
- HIGH should measure ~5V
- LOW should measure ~0V
- In-between = problem!

---

## Comparison: digitalRead() vs. analogRead()

| Feature | digitalRead() | analogRead() |
|---------|---------------|--------------|
| Return value | HIGH (1) or LOW (0) | 0-1023 (10-bit) |
| Pins | Any digital pin | Only analog pins (A0-A5) |
| Speed | Very fast (~5 μs) | Slower (~100 μs) |
| Use case | Buttons, switches | Potentiometers, sensors |
| Voltage resolution | Binary (on/off) | Continuous (0-5V) |

**Example:**
```cpp
// Digital: Is button pressed?
int pressed = digitalRead(buttonPin);  // Returns 0 or 1

// Analog: What's the pot position?
int position = analogRead(potPin);     // Returns 0-1023
```

---

## Quick Reference

| Task | Code | Expected Return |
|------|------|-----------------|
| Read button (INPUT_PULLUP) | `digitalRead(buttonPin)` | HIGH (not pressed), LOW (pressed) |
| Read button (INPUT) | `digitalRead(buttonPin)` | LOW (not pressed), HIGH (pressed) with pullup |
| Read sensor (PIR) | `digitalRead(pirPin)` | HIGH (motion), LOW (no motion) |
| Read switch | `digitalRead(switchPin)` | HIGH (open), LOW (closed) |
| Check if HIGH | `if (digitalRead(pin) == HIGH)` | true/false |
| Check if LOW | `if (digitalRead(pin) == LOW)` | true/false |

---

## Summary

- **Purpose:** Read digital pin state (HIGH or LOW)
- **Syntax:** `digitalRead(pin)`
- **Returns:** `HIGH` (1) or `LOW` (0)
- **Prerequisites:** Must call `pinMode(pin, INPUT)` or `pinMode(pin, INPUT_PULLUP)` first
- **Voltage threshold:** > 3V = HIGH, < 1.5V = LOW
- **Use for:** Buttons, switches, digital sensors
- **Always use:** Pull-up or pull-down resistor (internal or external)
- **Debounce:** Mechanical switches need debouncing
- **Speed:** Very fast (~5 μs per read)

---

## Checklist: Before Using digitalRead()

- [ ] Called `pinMode(pin, INPUT)` or `pinMode(pin, INPUT_PULLUP)` in setup()
- [ ] Using pull-up or pull-down (internal or external)
- [ ] Understanding INPUT_PULLUP inverts logic (HIGH=not pressed)
- [ ] Pin number correct
- [ ] Debouncing for mechanical switches
- [ ] Not reading too fast (add delay if printing)
- [ ] Circuit wired correctly

---

**Remember:** digitalRead() is simple but requires proper pull-up/pull-down configuration. Always use INPUT_PULLUP for buttons—it's the easiest and most reliable! 🔘