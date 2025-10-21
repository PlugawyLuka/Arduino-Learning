# digitalWrite() - Complete Reference Guide

## Overview

`digitalWrite()` sets a digital pin to **HIGH** (5V) or **LOW** (0V). It's the fundamental function for controlling outputs like LEDs, relays, and sending digital signals.

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
9. [Safety Considerations](#safety-considerations)

---

## Basic Syntax

```cpp
digitalWrite(pin, value)
```

### Parameters
- **pin**: Pin number (digital pins 0-13, analog pins A0-A5 can also be used as digital)
- **value**: 
  - `HIGH` (or `1`) → 5V output
  - `LOW` (or `0`) → 0V (GND)

### Returns
Nothing (void function)

### Prerequisites
**Must call `pinMode(pin, OUTPUT)` first!**

---

## How It Works

### Internal Process

```cpp
digitalWrite(3, HIGH);
```

**What happens step-by-step:**
1. Arduino looks up which PORT register controls pin 3 (e.g., PORTD on Uno)
2. Sets the corresponding bit in that register to `1`
3. Pin **sources current** (provides 5V)
4. If LED connected: current flows through LED → lights up

```cpp
digitalWrite(3, LOW);
```

**What happens:**
1. PORT register bit set to `0`
2. Pin **sinks to ground** (0V)
3. LED turns off (no voltage difference)

### Memory Representation

```
Arduino Uno - Digital Pin 3:
┌─────────────────────────────────┐
│ PORTD Register (8 bits)         │
│ [D7][D6][D5][D4][D3][D2][D1][D0]│
│                  ↑               │
│                Pin 3             │
└─────────────────────────────────┘

digitalWrite(3, HIGH) → Set bit 3 to 1
digitalWrite(3, LOW)  → Set bit 3 to 0
```

---

## Electrical Behavior

### HIGH State

```cpp
digitalWrite(ledPin, HIGH);
```

**Electrical characteristics:**
- **Voltage output:** 5V (on 5V boards) or 3.3V (on 3.3V boards)
- **Current capacity:** ~40mA per pin (max)
- **Total current:** 200mA for all pins combined (max)
- **Internal resistance:** ~25Ω (varies by pin)

**Voltage levels:**
- HIGH = anything above ~3V
- LOW = anything below ~1.5V  
- Between 1.5V-3V = **undefined** (unreliable)

**Circuit diagram:**
```
      5V
       │
       ├─── Pin (HIGH)
       │
      LED
       │
    Resistor (220Ω)
       │
      GND
```

### LOW State

```cpp
digitalWrite(ledPin, LOW);
```

**Electrical characteristics:**
- **Voltage output:** 0V (connected to GND)
- **Sink capacity:** Can sink ~40mA per pin
- **Behavior:** Acts like a wire to ground

**Circuit diagram:**
```
Pin (LOW) ─── GND
       │
      LED (off)
       │
    Resistor
       │
      5V (no current flows)
```

---

## Real-World Examples

### Example 1: Blinking LED (Your Project)

```cpp
void setup() {
  pinMode(13, OUTPUT);  // Built-in LED
}

void loop() {
  digitalWrite(13, HIGH);  // Turn on
  delay(1000);             // Wait 1 second
  digitalWrite(13, LOW);   // Turn off
  delay(1000);             // Wait 1 second
}
```

### Example 2: LED Level Meter

```cpp
// Turn ON LEDs up to level
for (int n = 0; n < outputValue; n++) {
  digitalWrite(ledPins[n], HIGH);
}

// Turn OFF LEDs above level
for (int n = outputValue; n < pinCount; n++) {
  digitalWrite(ledPins[n], LOW);
}
```

### Example 3: Controlling a Relay

```cpp
const int relayPin = 7;

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);  // Relay off initially
}

void loop() {
  digitalWrite(relayPin, HIGH);  // Turn on relay (activates connected device)
  delay(5000);                   // Run for 5 seconds
  digitalWrite(relayPin, LOW);   // Turn off relay
  delay(5000);                   // Wait 5 seconds
}
```

### Example 4: Motor Control (via Transistor)

```cpp
const int motorPin = 9;

void setup() {
  pinMode(motorPin, OUTPUT);
}

void loop() {
  digitalWrite(motorPin, HIGH);  // Motor on
  delay(2000);
  digitalWrite(motorPin, LOW);   // Motor off
  delay(2000);
}
```

### Example 5: Button-Controlled LED

```cpp
const int buttonPin = 2;
const int ledPin = 13;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW) {  // Button pressed (INPUT_PULLUP inverted)
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
```

---

## Common Use Cases

### 1. LED Control

```cpp
digitalWrite(ledPin, HIGH);  // On
digitalWrite(ledPin, LOW);   // Off
```

### 2. Relay/Solenoid Control

```cpp
digitalWrite(relayPin, HIGH);  // Activate
digitalWrite(relayPin, LOW);   // Deactivate
```

### 3. Logic Signal to Other Devices

```cpp
// Send HIGH signal to start another Arduino
digitalWrite(triggerPin, HIGH);
delayMicroseconds(10);
digitalWrite(triggerPin, LOW);
```

### 4. Buzzer Control

```cpp
digitalWrite(buzzerPin, HIGH);  // Sound on
delay(500);
digitalWrite(buzzerPin, LOW);   // Sound off
```

### 5. Shift Register Control

```cpp
digitalWrite(dataPin, HIGH);    // Send bit
digitalWrite(clockPin, HIGH);   // Clock pulse
digitalWrite(clockPin, LOW);
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
- INPUT mode can't source much current (~1-2mA)
- LED appears dim or off

**Fix:**
```cpp
void setup() {
  pinMode(13, OUTPUT);  // Always set mode first!
}
```

---

### Mistake 2: Writing to INPUT Pin

```cpp
pinMode(ledPin, INPUT);      // Set as input
digitalWrite(ledPin, HIGH);  // Won't light LED properly
```

**Why it fails:**
- INPUT pins can't provide enough current for LEDs
- LED appears very dim

**Fix:**
```cpp
pinMode(ledPin, OUTPUT);  // Use OUTPUT for LEDs
```

---

### Mistake 3: No Current-Limiting Resistor

```cpp
// Circuit: Pin → LED → GND (NO RESISTOR!)
digitalWrite(ledPin, HIGH);
```

**Danger:**
- LED draws excessive current (20-40mA+)
- Pin might overheat or be damaged permanently
- LED might burn out from overcurrent

**Electrical calculation:**
```
Without resistor:
Current = V / R_led ≈ 5V / 20Ω ≈ 250mA (WAY TOO HIGH!)

With 220Ω resistor:
Current = V / (R + R_led) ≈ 5V / 240Ω ≈ 20mA (SAFE)
```

**Fix:**
```
Pin → 220Ω Resistor → LED + (anode)
                     LED - (cathode) → GND
```

---

### Mistake 4: Short Circuit

```cpp
pinMode(pin, OUTPUT);
digitalWrite(pin, HIGH);  // Pin outputs 5V
// Now wire pin directly to GND → DISASTER!
```

**What happens:**
- Pin tries to output 5V
- GND is 0V
- **Massive current flows** (limited only by internal resistance)
- **Can burn out the pin or entire microcontroller**

**Protection:**
- Never connect OUTPUT pin directly to GND or 5V
- Always use current-limiting components

---

### Mistake 5: Forgetting to Turn OFF

```cpp
// Turn on LED but never turn off
for (int n = 0; n < outputValue; n++) {
  digitalWrite(ledPins[n], HIGH);
}
// If outputValue decreases, high LEDs stay on!
```

**Fix:**
```cpp
// Add loop to turn OFF unused LEDs
for (int n = outputValue; n < pinCount; n++) {
  digitalWrite(ledPins[n], LOW);
}
```

---

### Mistake 6: Using digitalWrite() for PWM

```cpp
// WRONG: Trying to set brightness with digitalWrite
digitalWrite(ledPin, 128);  // Doesn't work! Becomes HIGH (non-zero)
```

**Fix:**
```cpp
analogWrite(ledPin, 128);  // Use analogWrite for brightness (0-255)
```

---

## Troubleshooting

### LED Doesn't Light Up

**Check 1: Is pinMode() set to OUTPUT?**
```cpp
pinMode(ledPin, OUTPUT);  // Must be in setup()
```

**Check 2: Is digitalWrite(HIGH) being called?**
```cpp
digitalWrite(ledPin, HIGH);
```

**Check 3: LED orientation**
- Long leg (anode) → resistor → pin
- Short leg (cathode) → GND

**Check 4: Wiring**
- Resistor present? (220Ω-1kΩ)
- Connections secure?

**Debug code:**
```cpp
void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println("Turning LED ON");
  digitalWrite(ledPin, HIGH);
  delay(1000);
  
  Serial.println("Turning LED OFF");
  digitalWrite(ledPin, LOW);
  delay(1000);
}
```

---

### LED is Dim

**Possible causes:**
1. **Resistor too large** (1kΩ+ makes LED dim)
2. **Pin in INPUT mode** (can't source enough current)
3. **Weak power supply**
4. **Bad connection**

**Test:**
```cpp
// Measure voltage at pin with multimeter
digitalWrite(ledPin, HIGH);
// Should read ~5V

// Check resistor value with multimeter
// 220Ω-470Ω = bright
// 1kΩ+ = dim
```

---

### LED Won't Turn Off

**Check 1: Is LOW being written?**
```cpp
digitalWrite(ledPin, LOW);
```

**Check 2: Is there a conflicting HIGH elsewhere?**
```cpp
// Bad: Setting HIGH in two places
digitalWrite(ledPin, HIGH);
// ...later...
digitalWrite(ledPin, LOW);  // Gets overwritten by HIGH again
```

**Debug:**
```cpp
Serial.print("Setting pin ");
Serial.print(ledPin);
Serial.println(" to LOW");
digitalWrite(ledPin, LOW);
delay(2000);  // Long delay to observe
```

---

### Multiple LEDs Behave Strangely

**Possible cause: Power supply insufficient**
- Each LED draws ~15-20mA
- 10 LEDs = 150-200mA
- Arduino USB port: 500mA max
- Arduino 5V pin: 200mA total max

**Solution:**
- Use external power supply for many LEDs
- Or use transistors/shift registers to drive LEDs

---

## Advanced Techniques

### Efficient Multi-LED Control

```cpp
// Instead of multiple digitalWrite calls:
digitalWrite(ledPins[0], HIGH);
digitalWrite(ledPins[1], HIGH);
digitalWrite(ledPins[2], HIGH);

// Use a loop:
for (int i = 0; i < 3; i++) {
  digitalWrite(ledPins[i], HIGH);
}
```

### State Variable Pattern

```cpp
bool ledState = LOW;

void loop() {
  // Toggle LED state
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
  delay(500);
}
```

### Non-Blocking Blink (Using millis)

```cpp
unsigned long previousMillis = 0;
const long interval = 1000;
bool ledState = LOW;

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
  
  // Code here runs without blocking!
}
```

### Direct Port Manipulation (Advanced)

```cpp
// Faster than digitalWrite() but less portable
PORTD = B11111111;  // Set pins 0-7 HIGH simultaneously
PORTD = B00000000;  // Set pins 0-7 LOW simultaneously

// Equivalent to:
for (int i = 0; i < 8; i++) {
  digitalWrite(i, HIGH);  // Slower
}
```

---

## Safety Considerations

### Current Limits

**Per pin:**
- Absolute maximum: 40mA
- Recommended: 20mA
- Total for all pins: 200mA

**Calculation for LED:**
```cpp
// V = IR (Ohm's Law)
// Current = Voltage / Resistance

Resistor = (5V - LED_voltage) / desired_current
         = (5V - 2V) / 0.020A
         = 150Ω (use 220Ω standard value)
```

### Protecting the Arduino

**1. Always use resistors with LEDs**
```
Pin → 220Ω → LED → GND
```

**2. Use transistors for high-current loads**
```cpp
// For motors, relays, high-power LEDs:
// Pin → Transistor base (with resistor) → High current load
```

**3. Use diodes with inductive loads**
```
// For relays, solenoids, motors:
// Add flyback diode across coil
```

**4. Never short circuit outputs**
```cpp
// DON'T: Connect OUTPUT pin directly to GND or 5V
```

---

## Comparison: digitalWrite() vs. analogWrite()

| Feature | digitalWrite() | analogWrite() |
|---------|----------------|---------------|
| Output | Digital (ON/OFF) | PWM (0-255) |
| Pins | All digital pins | Only PWM pins (~3,5,6,9,10,11) |
| Voltage | 5V or 0V | Average voltage 0-5V |
| Use case | LED on/off, relays | LED dimming, motor speed |
| Speed | Very fast | Moderate (PWM frequency) |

**Example:**
```cpp
// Digital: LED fully on or off
digitalWrite(ledPin, HIGH);  // 5V, bright

// PWM: LED at 50% brightness
analogWrite(ledPin, 128);    // ~2.5V average, dimmed
```

---

## Quick Reference

| Task | Code |
|------|------|
| Turn LED on | `digitalWrite(pin, HIGH);` |
| Turn LED off | `digitalWrite(pin, LOW);` |
| Toggle LED | `digitalWrite(pin, !digitalRead(pin));` |
| Blink LED | `digitalWrite(pin, HIGH); delay(500); digitalWrite(pin, LOW); delay(500);` |
| Multiple LEDs on | `for (int i = 0; i < count; i++) digitalWrite(pins[i], HIGH);` |

---

## Summary

- **Purpose:** Set digital pin to HIGH (5V) or LOW (0V)
- **Syntax:** `digitalWrite(pin, value)`
- **Prerequisites:** Must call `pinMode(pin, OUTPUT)` first
- **Current limit:** 40mA per pin (20mA recommended)
- **Use for:** LEDs, relays, digital signals
- **Don't use for:** PWM (use analogWrite instead)
- **Safety:** Always use current-limiting resistors with LEDs

---

## Checklist: Before Using digitalWrite()

- [ ] Called `pinMode(pin, OUTPUT)` in setup()
- [ ] Using current-limiting resistor (220Ω-1kΩ for LEDs)
- [ ] Pin number correct (0-13, A0-A5)
- [ ] Load current < 40mA per pin
- [ ] Total current < 200mA for all pins
- [ ] Not short-circuiting pin to GND or 5V
- [ ] LED polarity correct (long leg to resistor/pin)

---

**Remember:** digitalWrite() is simple but powerful. Always configure with pinMode() first, and protect your Arduino with proper resistors! 💡