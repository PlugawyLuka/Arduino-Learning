# constrain() - Complete Reference Guide

## Overview

`constrain()` is an Arduino function that **limits a value to stay within a minimum and maximum range**. It's essential for preventing errors like array out-of-bounds and invalid sensor readings.

---

## Table of Contents

1. [Basic Syntax](#basic-syntax)
2. [How It Works](#how-it-works)
3. [Real-World Examples](#real-world-examples)
4. [Why You Need It](#why-you-need-it)
5. [Common Use Cases](#common-use-cases)
6. [Comparison with Alternatives](#comparison-with-alternatives)
7. [Common Mistakes](#common-mistakes)
8. [Troubleshooting](#troubleshooting)
9. [Advanced Techniques](#advanced-techniques)

---

## Basic Syntax

```cpp
constrain(value, min, max)
```

### Parameters
- **value**: The number you want to limit (int or long)
- **min**: The lowest allowed value
- **max**: The highest allowed value

### Returns
- If `value` is between `min` and `max` → returns `value` unchanged
- If `value` is less than `min` → returns `min`
- If `value` is greater than `max` → returns `max`

### Return Type
Same type as input (int or long)

---

## How It Works

### Example 1: Value Within Range
```cpp
int result = constrain(50, 0, 100);
// result = 50 (50 is between 0 and 100, so unchanged)
```

### Example 2: Value Too Low
```cpp
int result = constrain(-10, 0, 100);
// result = 0 (clamped to minimum)
```

### Example 3: Value Too High
```cpp
int result = constrain(150, 0, 100);
// result = 100 (clamped to maximum)
```

### Visual Representation

```
Input range:    -∞ ←──── [min] ←──── value ──→ [max] ────→ +∞
                     
Output:         min        min ← value → max      max
```

---

## Why You Need It

### Problem: Array Out-of-Bounds

```cpp
int ledPins[10];  // Indices 0-9

int index = map(sensorValue, 0, 1023, 0, 10);
// When sensorValue = 1023: index becomes 10 (invalid!)

digitalWrite(ledPins[index], HIGH);  // CRASH!
```

### Solution: Use constrain()

```cpp
int index = map(sensorValue, 0, 1023, 0, 10);
index = constrain(index, 0, 9);  // Max is 9 (last valid index)

digitalWrite(ledPins[index], HIGH);  // Safe!
```

### What Happens

**Test case 1: Pot at minimum (0)**
```cpp
map(0, 0, 1023, 0, 10) = 0
constrain(0, 0, 9) = 0  ✓ Safe
```

**Test case 2: Pot at middle (512)**
```cpp
map(512, 0, 1023, 0, 10) = 5
constrain(5, 0, 9) = 5  ✓ Safe
```

**Test case 3: Pot at maximum (1023)**
```cpp
map(1023, 0, 1023, 0, 10) = 10  ⚠️ Too high!
constrain(10, 0, 9) = 9  ✓ Fixed to safe value
```

---

## Real-World Examples

### Example 1: LED Level Meter (Your Project)

```cpp
void loop() {
  sensorValue = analogRead(A0);
  outputValue = map(sensorValue, 0, 1023, 0, 10);
  
  // Prevent array overflow
  outputValue = constrain(outputValue, 0, 9);
  
  // Now safe to use as array index
  for (int n = 0; n < outputValue; n++) {
    digitalWrite(ledPins[n], HIGH);
  }
}
```

### Example 2: Temperature Sensor Validation

```cpp
int temp = readTemperature();

// Sensor valid range: -40°C to 125°C
temp = constrain(temp, -40, 125);

Serial.print("Temperature: ");
Serial.println(temp);
```

### Example 3: PWM Brightness Control

```cpp
int brightness = userInput;

// PWM only accepts 0-255
brightness = constrain(brightness, 0, 255);

analogWrite(ledPin, brightness);
```

### Example 4: Servo Angle Limits

```cpp
int angle = calculateAngle();

// Servo mechanical limits: 0-180 degrees
angle = constrain(angle, 0, 180);

myServo.write(angle);
```

### Example 5: Game Boundaries

```cpp
// Player position
int playerX = playerX + velocity;
int playerY = playerY + jumpSpeed;

// Keep player on screen
playerX = constrain(playerX, 0, screenWidth - playerWidth);
playerY = constrain(playerY, 0, screenHeight - playerHeight);
```

---

## Common Use Cases

### 1. Protecting Array Access

```cpp
int myArray[10];
int index = getSomeValue();

index = constrain(index, 0, 9);  // Valid indices: 0-9
myArray[index] = 42;  // Safe!
```

### 2. Limiting Sensor Readings

```cpp
int sensorValue = analogRead(A0);

// Only care about middle range
sensorValue = constrain(sensorValue, 200, 800);
```

### 3. Preventing Invalid PWM Values

```cpp
int brightness = calculation();
brightness = constrain(brightness, 0, 255);
analogWrite(ledPin, brightness);
```

### 4. Enforcing Valid User Input

```cpp
int userAge = Serial.parseInt();

// Realistic age range
userAge = constrain(userAge, 0, 120);
```

### 5. Keeping Values Within Physical Limits

```cpp
int motorSpeed = calculateSpeed();

// Motor rated for 0-100% duty cycle
motorSpeed = constrain(motorSpeed, 0, 100);
```

---

## Comparison with Alternatives

### constrain() vs. min() / max()

**Single limit (max only):**
```cpp
value = min(value, 100);  // Can't exceed 100
```

**Single limit (min only):**
```cpp
value = max(value, 0);    // Can't go below 0
```

**Both limits:**
```cpp
// Using constrain (cleaner)
value = constrain(value, 0, 100);

// Using min/max (nested, harder to read)
value = max(0, min(value, 100));
```

### constrain() vs. if Statements

**Using if:**
```cpp
if (value < 0) {
  value = 0;
}
if (value > 100) {
  value = 100;
}
```

**Using constrain:**
```cpp
value = constrain(value, 0, 100);  // One line, clearer intent
```

**When to use if instead:**
- When you need different actions for each case
- When you want to log violations

```cpp
if (value < 0) {
  Serial.println("Error: negative value!");
  value = 0;
} else if (value > 100) {
  Serial.println("Warning: value capped");
  value = 100;
}
```

### constrain() vs. map() with Safe Range

**Option 1: map() to safe range**
```cpp
outputValue = map(sensorValue, 0, 1023, 0, 9);  // Max is 9
```

**Option 2: map() then constrain()**
```cpp
outputValue = map(sensorValue, 0, 1023, 0, 10);
outputValue = constrain(outputValue, 0, 9);
```

**Which is better?**
- Option 1: Simpler, no need for constrain()
- Option 2: More explicit about array size (pinCount)

---

## Common Mistakes

### Mistake 1: Wrong Order of min/max

```cpp
// WRONG: min is larger than max
constrain(value, 100, 0);  // Will always return 0!

// RIGHT:
constrain(value, 0, 100);
```

**Why it fails:**
```cpp
// If value = 50:
if (50 < 100) return 100  // min
// Result: always returns "min" (100)
```

### Mistake 2: Off-By-One with Arrays

```cpp
int arr[10];  // Indices 0-9

// WRONG: max is 10 (invalid index)
int index = constrain(value, 0, 10);

// RIGHT: max is 9 (last valid index)
int index = constrain(value, 0, 9);
```

### Mistake 3: Not Using the Result

```cpp
// WRONG: constrain doesn't modify in-place
constrain(value, 0, 100);  // Does nothing!

// RIGHT: Assign the result
value = constrain(value, 0, 100);
```

### Mistake 4: Using Wrong Data Type

```cpp
float value = 3.7;

// constrain() returns int, truncates decimal
value = constrain(value, 0, 10);  // value becomes 3 (not 3.7)

// For floats, use manual if statements
if (value < 0.0) value = 0.0;
if (value > 10.0) value = 10.0;
```

### Mistake 5: Constraining Before Calculation

```cpp
// WRONG: Constraining input instead of output
sensorValue = constrain(sensorValue, 0, 9);
outputValue = map(sensorValue, 0, 1023, 0, 10);  // map() gets wrong input

// RIGHT: Constrain the result
sensorValue = analogRead(A0);
outputValue = map(sensorValue, 0, 1023, 0, 10);
outputValue = constrain(outputValue, 0, 9);
```

---

## Troubleshooting

### Debug: Print Before and After

```cpp
Serial.print("Before constrain: ");
Serial.println(outputValue);

outputValue = constrain(outputValue, 0, 9);

Serial.print("After constrain: ");
Serial.println(outputValue);
```

**Example output:**
```
Before constrain: 10
After constrain: 9
```

### Check If Constraining is Happening

```cpp
int original = outputValue;
outputValue = constrain(outputValue, 0, 9);

if (original != outputValue) {
  Serial.print("Warning: Value was constrained from ");
  Serial.print(original);
  Serial.print(" to ");
  Serial.println(outputValue);
}
```

### Verify Range is Correct

```cpp
const int MIN_INDEX = 0;
const int MAX_INDEX = pinCount - 1;

outputValue = constrain(outputValue, MIN_INDEX, MAX_INDEX);

Serial.print("Constrained to range [");
Serial.print(MIN_INDEX);
Serial.print(", ");
Serial.print(MAX_INDEX);
Serial.println("]");
```

---

## Advanced Techniques

### Using Variables for Limits

```cpp
const int MIN_SPEED = 0;
const int MAX_SPEED = 255;

int speed = calculateSpeed();
speed = constrain(speed, MIN_SPEED, MAX_SPEED);
```

**Benefits:**
- Self-documenting code
- Easy to change limits
- No magic numbers

### Constraining to Array Size Automatically

```cpp
int ledPins[] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int pinCount = sizeof(ledPins) / sizeof(ledPins[0]);

int index = someCalculation();
index = constrain(index, 0, pinCount - 1);  // Always safe!
```

### Deadband / Threshold

```cpp
// Ignore small movements (joystick deadband)
int joystickX = analogRead(A0) - 512;  // Center at 0

if (abs(joystickX) < 20) {
  joystickX = 0;  // Ignore small drift
}

joystickX = constrain(joystickX, -512, 512);
```

### Soft Limits with Warning

```cpp
int temp = readSensor();

const int WARN_TEMP = 80;
const int MAX_TEMP = 100;

if (temp > WARN_TEMP) {
  Serial.println("Warning: Temperature high!");
}

temp = constrain(temp, 0, MAX_TEMP);
```

---

## Implementation Details

### How constrain() Works Internally

```cpp
// Arduino source code (simplified)
long constrain(long x, long a, long b) {
  if (x < a) {
    return a;  // Too low, return min
  } else if (x > b) {
    return b;  // Too high, return max
  } else {
    return x;  // Within range, return unchanged
  }
}
```

### Performance

- **Very fast** (3 comparisons max)
- No floating point math
- Inline function (no overhead)

---

## Real-World Analogy

Think of `constrain()` like a **thermostat:**

- You set a min (e.g., 65°F) and max (e.g., 75°F)
- If temperature drops to 60°F → heater brings it back to 65°F (minimum)
- If temperature rises to 80°F → AC brings it down to 75°F (maximum)
- If temperature is 70°F → nothing changes (within range)

`constrain()` is a "digital thermostat" for values!

---

## Quick Reference

| Scenario | Code | Result |
|----------|------|--------|
| Value in range | `constrain(50, 0, 100)` | `50` |
| Value too low | `constrain(-10, 0, 100)` | `0` |
| Value too high | `constrain(150, 0, 100)` | `100` |
| Array safety | `constrain(index, 0, size-1)` | Valid index |
| PWM range | `constrain(val, 0, 255)` | 0-255 |

---

## Summary

- **Purpose:** Keep values within safe min/max range
- **Syntax:** `constrain(value, min, max)`
- **Returns:** Limited value (always between min and max)
- **Use Case:** Protecting array access, validating inputs
- **Key Point:** Always assign result: `value = constrain(value, ...)`
- **Best Practice:** Use named constants for limits

---

## Checklist: When to Use constrain()

- [ ] Accessing arrays with calculated indices
- [ ] Processing sensor data with known valid ranges
- [ ] Sending values to hardware with limits (PWM, servos)
- [ ] Implementing game boundaries
- [ ] Validating user input
- [ ] Preventing division by zero (constrain denominator > 0)
- [ ] Ensuring physical safety limits

---

**Remember:** `constrain()` is your safety net. Use it whenever a value *could* go out of bounds, even if you think it won't. Defensive programming prevents bugs! 🛡️