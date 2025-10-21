# Potentiometer Basics

**Created:** [Date not specified in workspace]  
**Status:** Learning Project ✓  
**Difficulty:** Beginner

---

## Description

A foundational Arduino project exploring potentiometer functionality through three progressive examples: basic analog reading, LED brightness control via PWM, and servo motor position control.

**Key features:**
- Direct analog input reading (0-1023)
- PWM-based LED brightness control
- Servo motor angle control (0-180°)
- Serial Monitor output for debugging
- Three distinct applications of same input

**Purpose:** Understand how potentiometers convert physical rotation into electrical signals, and how Arduino can use this input to control various outputs.

---

## Components

**Required:**
- [x] Arduino Uno
- [x] 10kΩ potentiometer (3-pin)
- [x] LED (any color)
- [x] 220Ω resistor (for LED current limiting)
- [x] Servo motor (standard hobby servo)
- [x] Breadboard
- [x] Jumper wires

**Optional:**
- [ ] Multimeter (to measure potentiometer resistance)

---

## Circuit Configurations

### **Configuration 1: Basic Analog Reading**
```
Potentiometer:
  Pin 1 (one end) → 5V
  Pin 2 (wiper)   → A0
  Pin 3 (other end) → GND

No additional components needed
```

### **Configuration 2: LED Brightness Control**
```
Potentiometer:
  Pin 1 → 5V
  Pin 2 → A0
  Pin 3 → GND

LED Circuit:
  Pin 9 (PWM) → 220Ω resistor → LED (anode) → GND
```

### **Configuration 3: Servo Control**
```
Potentiometer:
  Pin 1 → 5V
  Pin 2 → A0
  Pin 3 → GND

Servo Motor:
  Signal wire (usually orange/yellow) → Pin 9
  Power wire (usually red) → 5V
  Ground wire (usually brown/black) → GND
```

**Important notes:**
- ⚠️ **Potentiometer wiring matters** — Middle pin (wiper) to A0, outer pins to 5V/GND
- ⚠️ **LED needs current limiting resistor** — 220Ω protects LED and Arduino pin
- ⚠️ **Servo polarity critical** — Wrong connection can damage servo
- 💡 If servo jitters, add 100µF capacitor across servo power (5V to GND)

---

## How to Use

### **Example 1: Basic Reading**
**Setup:**
1. Wire potentiometer to A0 as shown above
2. Upload basic reading sketch
3. Open Serial Monitor (9600 baud)

**Operation:**
- Turn potentiometer knob
- Serial Monitor displays values (0-1023)
- Fully left = 0, Fully right = 1023

**Expected behavior:**
- Smooth value changes as you turn knob
- Full range from 0 to 1023
- Real-time response

---

### **Example 2: LED Brightness**
**Setup:**
1. Keep potentiometer connected
2. Add LED circuit to Pin 9
3. Upload LED brightness sketch
4. Open Serial Monitor

**Operation:**
- Turn potentiometer left → LED dims
- Turn potentiometer right → LED brightens
- Serial Monitor shows analog value and PWM output

**Expected behavior:**
- LED brightness proportional to knob position
- Smooth brightness transitions
- No flickering (PWM is fast enough)

---

### **Example 3: Servo Control**
**Setup:**
1. Keep potentiometer connected
2. Connect servo to Pin 9
3. Upload servo control sketch
4. Open Serial Monitor

**Operation:**
- Turn potentiometer left → Servo moves to 0° (one end)
- Turn potentiometer center → Servo moves to 90° (middle)
- Turn potentiometer right → Servo moves to 180° (other end)
- Serial Monitor shows analog value and servo angle

**Expected behavior:**
- Servo follows knob position smoothly
- Full 180° range of motion
- Proportional control

---

## Concepts Used

This project demonstrates these fundamental concepts:

### Analog Input (analogRead)
**How it's used here:** Reading potentiometer position via A0  
**Why it matters:** Foundation for all variable input devices (sensors, knobs, sliders)  
**How it works:**
- Arduino's ADC (Analog-to-Digital Converter) measures voltage at pin
- Returns value 0-1023 (10-bit resolution)
- 0 = 0V, 1023 = 5V, 512 = 2.5V
- Resolution: 5V / 1024 = ~4.9mV per step

**Example:**
```cpp
int sensorValue = analogRead(A0);  // Returns 0-1023
// If potentiometer at middle: sensorValue ≈ 512
```

### Potentiometer as Voltage Divider
**How it's used here:** Variable resistor creates adjustable voltage  
**Why it matters:** Simple way to create user input without buttons  
**How it works:**
```
5V ─┬─ R1 (adjustable) ─┬─ A0 (measures voltage here)
    │                   │
    └─ Total: 10kΩ ─────┘
                         │
                        GND

Wiper position determines voltage at A0:
- Top position: V_A0 ≈ 5V (R1 minimal, R2 maximal)
- Middle: V_A0 ≈ 2.5V (R1 = R2 = 5kΩ)
- Bottom: V_A0 ≈ 0V (R1 maximal, R2 minimal)
```

**Related Finding:** [Voltage Dividers](../Findings/Voltage-Divider.md)

### PWM (Pulse Width Modulation)
**How it's used here:** Controlling LED brightness via `analogWrite()`  
**Why it matters:** Creates "analog-like" output from digital pins  
**How it works:**
- Rapidly switches pin HIGH/LOW (default ~490 Hz on Arduino)
- Duty cycle determines effective voltage
- `analogWrite(255)` = always HIGH (100% duty cycle, full brightness)
- `analogWrite(128)` = 50% duty cycle (half brightness)
- `analogWrite(0)` = always LOW (0% duty cycle, off)

**Example:**
```cpp
int ledBrightness = map(sensorValue, 0, 1023, 0, 255);
analogWrite(9, ledBrightness);  // PWM on pin 9
```

**Note:** Only certain pins support PWM (marked with ~ on Arduino Uno: 3, 5, 6, 9, 10, 11)

### Map Function
**How it's used here:** Converting input range (0-1023) to output ranges (0-255 for PWM, 0-180 for servo)  
**Why it matters:** Scales any value range to any other range - extremely versatile!  
**Syntax:**
```cpp
map(value, fromLow, fromHigh, toLow, toHigh)
```

**Examples in this project:**
```cpp
// Example 2: LED brightness
int ledBrightness = map(sensorValue, 0, 1023, 0, 255);
// Input: 0-1023 → Output: 0-255 (PWM range)

// Example 3: Servo angle
int angle = map(sensorValue, 0, 1023, 0, 180);
// Input: 0-1023 → Output: 0-180 (servo range)

// Edge case handling:
// map(512, 0, 1023, 0, 255) = 127 (middle input → middle output)
```

### Servo Motor Control
**How it's used here:** Servo library provides angle-based positioning  
**Why it matters:** Simple way to create precise mechanical movement  
**How it works:**
```cpp
#include <Servo.h>

Servo myServo;
myServo.attach(9);           // Connect servo to pin 9
myServo.write(90);           // Move to 90° (center position)
```

**PWM signal details:**
- Servo expects pulse every 20ms (50 Hz)
- Pulse width determines angle:
  - 1ms pulse = 0° (one end)
  - 1.5ms pulse = 90° (center)
  - 2ms pulse = 180° (other end)
- Servo library handles timing automatically

### [Current Limiting](../Findings/Current-Limiting.md) _(if extracted)_
**How it's used here:** 220Ω resistor protects LED in Example 2  
**Why it matters:** Prevents LED and pin damage from overcurrent  
**Calculation:**
```
V_supply = 5V
V_led = 2V (forward voltage)
V_resistor = 5V - 2V = 3V
I_target = 13-15mA (safe for LED and Arduino)
R = V / I = 3V / 0.014A = 214Ω → Use 220Ω (standard value)
```

---

## Code Examples

### **Example 1: Basic Analog Reading**
```cpp
const int potPin = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(potPin);
  
  Serial.print("Potentiometer value: ");
  Serial.println(sensorValue);
  
  delay(100);  // Slow down readings for readability
}
```

**Output:**
```
Potentiometer value: 0
Potentiometer value: 127
Potentiometer value: 512
Potentiometer value: 823
Potentiometer value: 1023
```

---

### **Example 2: LED Brightness Control**
```cpp
const int potPin = A0;
const int ledPin = 9;  // Must be PWM pin (~)

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(potPin);           // 0-1023
  int ledBrightness = map(sensorValue, 0, 1023, 0, 255);  // 0-255
  
  analogWrite(ledPin, ledBrightness);  // PWM output
  
  Serial.print("Analog value: ");
  Serial.print(sensorValue);
  Serial.print(" → LED brightness: ");
  Serial.println(ledBrightness);
  
  delay(10);  // Small delay for stability
}
```

**Key points:**
- `analogWrite()` accepts 0-255 (not 0-1023 like `analogRead()`)
- Must use PWM-capable pin (marked with ~)
- No `pinMode()` needed for `analogWrite()` (it handles this)

---

### **Example 3: Servo Motor Control**
```cpp
#include <Servo.h>

const int potPin = A0;
Servo myServo;

void setup() {
  myServo.attach(9);  // Servo on pin 9
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(potPin);           // 0-1023
  int angle = map(sensorValue, 0, 1023, 0, 180);  // 0-180°
  
  myServo.write(angle);  // Move servo to angle
  
  Serial.print("Analog value: ");
  Serial.print(sensorValue);
  Serial.print(" → Servo angle: ");
  Serial.println(angle);
  
  delay(15);  // Small delay for servo to reach position
}
```

**Key points:**
- `#include <Servo.h>` required (built-in library)
- `attach()` in `setup()`, not `pinMode()`
- `write()` accepts 0-180 (degrees)
- Servo moves proportionally to input

---

## What I Learned

**Technical skills:**
- Reading analog input with `analogRead()`
- Understanding 10-bit ADC resolution (0-1023)
- Using PWM for variable output (`analogWrite()`)
- Controlling servo motors with Servo library
- Scaling values with `map()` function

**Concepts understood:**
- **Potentiometer operation** — Variable resistor as voltage divider
- **ADC fundamentals** — Converting analog voltage to digital value
- **PWM principle** — Creating "analog-like" output from digital pin
- **Map function versatility** — Scaling any range to any range
- **Servo control** — Angle-based positioning with pulse width

**Progressive learning:**
1. **Read input** → Understanding analog values
2. **Control output** → Using input to drive LED brightness
3. **Mechanical control** → Moving physical servo with input

**Key insights:**
- Same input (potentiometer) can control different outputs
- `map()` is essential for range conversions
- PWM creates illusion of variable voltage
- Servo library abstracts complex PWM timing

---

## Troubleshooting

### Problem: Potentiometer Readings Erratic/Noisy
**Symptoms:** Values jump around randomly, not smooth  
**Cause:** Poor connections, electrical noise, or floating pin  
**Solution:**
- Check breadboard connections (especially wiper to A0)
- Ensure 5V and GND are firmly connected
- Add software filtering: `int smoothed = (reading1 + reading2 + reading3) / 3;`
- Use shielded cable for A0 if in noisy environment  
**Prevention:** Secure connections, test potentiometer with multimeter

### Problem: LED Not Changing Brightness
**Symptoms:** LED stays at constant brightness regardless of knob  
**Cause:** Using non-PWM pin or wrong pin number  
**Solution:**
- Verify LED connected to PWM pin (3, 5, 6, 9, 10, or 11 on Uno)
- Check code uses `analogWrite()`, not `digitalWrite()`
- Verify `ledPin` variable matches physical connection
- Test with Serial Monitor to see if values are changing  
**Prevention:** Always use pins marked with ~ for PWM

### Problem: Servo Jitters or Doesn't Move Smoothly
**Symptoms:** Servo vibrates, moves erratically, or resets position  
**Cause:** Power supply instability (servos draw significant current)  
**Solution:**
- Add 100µF capacitor across servo power (5V to GND, near servo)
- Use external 5V power supply for servo (share GND with Arduino)
- Increase `delay()` in loop to give servo time to settle
- Check servo connections (especially signal wire)  
**Prevention:** Always use capacitor with servos, consider external power for multiple servos

### Problem: Serial Monitor Shows No Values
**Symptoms:** Serial Monitor blank or shows garbage characters  
**Cause:** Wrong baud rate or Serial not initialized  
**Solution:**
- Verify `Serial.begin(9600)` in `setup()`
- Check Serial Monitor baud rate matches code (bottom-right dropdown)
- Ensure USB cable is data cable (not charge-only)
- Try different USB port  
**Prevention:** Always match baud rates, use quality USB cables

### Problem: Map Function Gives Unexpected Values
**Symptoms:** Output range incorrect (e.g., LED brightness weird)  
**Cause:** Incorrect map parameters or reversed range  
**Solution:**
- Verify input range: `analogRead()` returns 0-1023, not 0-255
- Check output range: PWM is 0-255, servo is 0-180
- Syntax: `map(value, fromLow, fromHigh, toLow, toHigh)`
- Test with known values: `map(512, 0, 1023, 0, 255)` should give ~127  
**Prevention:** Double-check map parameters, test with Serial Monitor

---

## Improvements & Next Steps

**Possible enhancements:**
- [ ] Add multiple LEDs with different brightness curves (easy)
- [ ] Control RGB LED color with 3 potentiometers (medium)
- [ ] Create "dead zone" in center (servo stays still near middle) (easy)
- [ ] Add LCD display to show exact values (medium)
- [ ] Implement exponential/logarithmic scaling for more natural feel (medium)
- [ ] Control multiple servos (pan-tilt mechanism) (medium)

**What I'd do differently:**
- Add capacitor to servo power from the start
- Use external power for servo (more stable)
- Implement software smoothing for readings
- Add visual feedback (LED blinks at min/max positions)

**Future project ideas:**
- Joystick-controlled robot arm (2 servos, 2 potentiometers)
- Volume control interface (potentiometer + audio module)
- Variable speed motor controller
- Custom MIDI controller (potentiometers as knobs)

---

## Related Projects

**Uses same concepts:**
- [10LEDs Level Meter](../10LEDs-level/README.md) — Uses potentiometer and map function
- Any sensor project using analog input

**Next logical steps:**
- [Photoresistor Light Meter](../Photoresistor/README.md) — Replace potentiometer with light sensor
- Joystick project — 2 potentiometers + XY control
- Multi-servo robot arm

**Similar input devices:**
- Joystick (2 potentiometers + button)
- Slide potentiometer (linear instead of rotary)
- Flex sensor (variable resistance)

---

## Files

**Code:**
- `basic-reading.ino` — Example 1: Analog reading
- `led-brightness.ino` — Example 2: PWM LED control
- `servo-control.ino` — Example 3: Servo positioning

**Documentation:**
- `README.md` — This file

---

## Technical Reference

### **Potentiometer Specifications**
- **Resistance:** 10kΩ (common value, others: 1kΩ, 5kΩ, 100kΩ)
- **Type:** Linear taper (resistance changes linearly with rotation)
- **Rotation:** ~270° (some go full 360°, but less common)
- **Pins:** 3 pins (two ends + wiper/center)
- **Power rating:** Typically 0.2W (not critical in this application)

### **Arduino ADC Details**
- **Resolution:** 10-bit (1024 possible values: 0-1023)
- **Reference voltage:** 5V (default on Arduino Uno)
- **Resolution:** 5V / 1024 = 4.88mV per step
- **Input impedance:** 100 MΩ (high - doesn't significantly load potentiometer)
- **Conversion time:** ~100 microseconds

### **PWM Specifications (Arduino Uno)**
- **Frequency:** ~490 Hz (pins 3, 9, 10, 11), ~980 Hz (pins 5, 6)
- **Resolution:** 8-bit (256 levels: 0-255)
- **Voltage:** 0V (analogWrite 0) to 5V (analogWrite 255)
- **Pins:** 3, 5, 6, 9, 10, 11 (marked with ~)

### **Servo Motor Specifications**
- **Operating voltage:** 4.8V - 6V (typically 5V from Arduino)
- **Current draw:** 100-300mA (idle), 500-1000mA (under load)
- **Rotation range:** 180° (standard hobby servo)
- **Control signal:** PWM at 50 Hz (20ms period)
  - 1ms pulse = 0°
  - 1.5ms pulse = 90°
  - 2ms pulse = 180°
- **Torque:** Varies by model (typically 1-3 kg·cm for small servos)

---

## Session History

**Creation date:** Not specified in workspace  
**Status:** Learning project completed  

**Key milestones:**
- Understood analog input fundamentals
- Learned PWM for output control
- Mastered map function for range conversion
- Implemented servo control with library

---

## References

**Concepts documentation:**
- [Voltage Dividers](../Findings/Voltage-Divider.md) — How potentiometer creates variable voltage
- Current Limiting _(to be extracted to Findings/)_ — LED resistor calculation

**Arduino documentation:**
- `analogRead()` — https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
- `analogWrite()` — https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/
- `map()` — https://www.arduino.cc/reference/en/language/functions/math/map/
- Servo library — https://www.arduino.cc/reference/en/libraries/servo/

**Related session logs:**
- (Reference specific session logs if available)

---

## Findings Used

**Existing Findings:**
- [Voltage Dividers](../Findings/Voltage-Divider.md) — Potentiometer operation principle

**Concepts to extract:**
- PWM Basics _(good candidate for Finding)_
- Map Function _(useful pattern for Finding)_
- Servo Control _(potentially Finding if used in multiple projects)_

---

**Overall summary:** Comprehensive introduction to analog input and variable output control. Three progressive examples demonstrate versatility of potentiometer input: reading values, controlling brightness, and positioning servo. Project establishes foundation for all sensor-based and user-controlled Arduino applications. Key concepts (analog input, PWM, map function, servo control) are fundamental building blocks for advanced projects.