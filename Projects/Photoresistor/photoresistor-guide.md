# Photoresistor Project Guide

**Component:** Light-Dependent Resistor (LDR / CdS Photocell)  
**Difficulty:** Beginner-Intermediate  
**Status:** Phase 1 Complete (Basic sensing & range mapping)  
**Awaiting:** DSO138Mini Oscilloscope for Phase 2 (PWM & Analog analysis)

**Created:** 19-20.10.25  
**Last Updated:** 20.10.25

---

## Project Overview

**What you'll learn:**
- ✅ How photoresistors work (resistance varies with light)
- ✅ Voltage divider practical application
- ✅ Analog input reading (analogRead)
- ✅ Sensor range characteristics & limitations
- ✅ Software calibration with map() function
- 🔜 PWM signals (with oscilloscope)
- 🔜 Analog signal analysis (with oscilloscope)

**Skills practiced:**
- Circuit building (voltage dividers)
- Serial debugging
- Sensor calibration
- Range mapping
- Real-world problem solving

---

## Components Required

### **Phase 1 (Current):**
- [x] Arduino Uno
- [x] Photoresistor (LDR)
- [x] 10kΩ resistor (voltage divider)
- [x] LED (for visual feedback)
- [x] 220Ω resistor (LED current limiting)
- [x] Breadboard + jumper wires

### **Phase 2 (Waiting for oscilloscope):**
- [ ] DSO138Mini Digital Oscilloscope Kit
- [ ] Additional test components (TBD)

---

## Theory — How Photoresistors Work

### **Basic Principle:**

**Photoresistor = Light-dependent resistor**
- Material: Cadmium Sulfide (CdS) semiconductor
- Property: Resistance DECREASES with increasing light
- Non-polarized: Can be connected either way

**Behavior:**
```
Bright light → Low resistance (~500Ω - 2kΩ)
Darkness → High resistance (~50kΩ - 200kΩ)
```

**Key characteristics:**
- Non-linear response (logarithmic)
- Slow response time (~10-100ms)
- Wide resistance range (3-4 orders of magnitude)
- Inexpensive and reliable
- No polarity (unlike LEDs or diodes)

---

### **Why We Need Voltage Divider:**

**The problem:** Arduino measures VOLTAGE, not RESISTANCE

**Solution:** Use voltage divider to convert resistance → voltage

**See also:** [Voltage Dividers Finding](../Findings/Voltage-Divider.md) for detailed explanation

---

## Circuit Design

### **Voltage Divider Configuration:**

```
Circuit Diagram:

       5V (Arduino)
        │
        ├─── Photoresistor (R_ldr, variable 500Ω-200kΩ)
        │
        ├─── A0 (analog input) ← Measures voltage HERE
        │
        └─── 10kΩ fixed resistor (R_fixed)
        │
       GND
```

**Voltage divider formula:**
```
V_out = V_in × (R_fixed / (R_ldr + R_fixed))
V_out = 5V × (10kΩ / (R_ldr + 10kΩ))
```

**Examples:**
```
Bright (R_ldr = 1kΩ):   V_out = 5V × (10k / 11k)  = 4.55V → analogRead ≈ 930
Medium (R_ldr = 10kΩ):  V_out = 5V × (10k / 20k)  = 2.50V → analogRead ≈ 512
Dark (R_ldr = 100kΩ):   V_out = 5V × (10k / 110k) = 0.45V → analogRead ≈ 92
```

**Why 10kΩ fixed resistor?**
- Good match for typical photoresistor mid-range
- Optimal sensitivity when R_fixed ≈ √(R_min × R_max)
- For 1kΩ-100kΩ range: √(1k × 100k) = 10kΩ ✓

---

### **Optional LED Indicator Circuit:**

**Add visual feedback:**
```
Digital Pin 9 (PWM) → 220Ω resistor → LED (+ longer leg) → GND

Or use built-in LED on Pin 13 (no external components needed)
```

**Current limiting:** 220Ω resistor protects LED and Arduino pin  
**See also:** [Current Limiting Finding](../Findings/Current-Limiting.md)

---

## Code Examples

### **Example 1: Basic Sensor Reading**

**Purpose:** Read and display photoresistor values

```cpp
// filepath: Luka/Arduino/Photoresistor/Photoresistor_Basic.ino

/*
 * Basic Photoresistor Reading
 * Displays raw sensor values (0-1023) in Serial Monitor
 */

const int sensorPin = A0;

void setup() {
  Serial.begin(9600);
  Serial.println("Photoresistor Test - Basic Reading");
  Serial.println("Cover sensor with hand to test");
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  
  Serial.print("Sensor value: ");
  Serial.println(sensorValue);
  
  delay(500);  // Update every half second
}
```

**Expected output:**
```
Photoresistor Test - Basic Reading
Cover sensor with hand to test
Sensor value: 650
Sensor value: 680
Sensor value: 320  ← Covered with hand
Sensor value: 340
Sensor value: 720  ← Hand removed
```

**Testing:**
- Cover photoresistor → value decreases
- Shine light → value increases
- Typical room light: 300-700

---

### **Example 2: Voltage Conversion**

**Purpose:** Convert ADC reading to actual voltage

```cpp
// filepath: Luka/Arduino/Photoresistor/Photoresistor_Voltage.ino

/*
 * Photoresistor with Voltage Display
 * Shows both raw value and actual voltage at A0
 */

const int sensorPin = A0;

void setup() {
  Serial.begin(9600);
  Serial.println("Sensor Value | Voltage");
  Serial.println("-------------------------");
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  
  // Convert to voltage (10-bit ADC, 5V reference)
  float voltage = sensorValue * (5.0 / 1023.0);
  
  Serial.print(sensorValue);
  Serial.print(" | ");
  Serial.print(voltage);
  Serial.println("V");
  
  delay(500);
}
```

**Understanding ADC conversion:**
- Arduino ADC: 10-bit resolution (2^10 = 1024 values)
- Range: 0-1023 (0V to 5V)
- Formula: `voltage = reading × (5.0 / 1023.0)`
- Example: 512 → 2.5V (middle of range)

**Expected output:**
```
Sensor Value | Voltage
-------------------------
682 | 3.33V
695 | 3.40V
412 | 2.02V  ← Covered
720 | 3.52V  ← Uncovered
```

---

### **Example 3: Simple Threshold Detection**

**Purpose:** Detect dark/bright conditions, control LED

```cpp
// filepath: Luka/Arduino/Photoresistor/Photoresistor_Threshold.ino

/*
 * Photoresistor Threshold Detection
 * Turns LED on when dark, off when bright
 * Like an automatic night light!
 */

const int sensorPin = A0;
const int ledPin = 13;  // Built-in LED (or use Pin 9 for external)
const int threshold = 400;  // Adjust based on your environment

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  
  Serial.println("Automatic Night Light");
  Serial.println("Threshold: " + String(threshold));
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  
  // Simple threshold logic
  if (sensorValue < threshold) {
    digitalWrite(ledPin, HIGH);  // Dark → LED on
    Serial.print("DARK - ");
  } else {
    digitalWrite(ledPin, LOW);   // Bright → LED off
    Serial.print("BRIGHT - ");
  }
  
  Serial.print("Sensor: ");
  Serial.println(sensorValue);
  
  delay(500);
}
```

**Calibration steps:**
1. Run code without threshold logic
2. Note typical values in your environment
3. Set threshold between "dark" and "bright"
4. Test by covering/uncovering sensor

---

### **Example 4: PWM LED Brightness Control (Basic)**

**Purpose:** LED brightness follows light level (BEFORE range fix)

```cpp
// filepath: Luka/Arduino/Photoresistor/Photoresistor_PWM_Basic.ino

/*
 * Basic PWM Brightness Control
 * LED brightness changes with light
 * Problem: Limited range (see Example 5 for solution)
 */

const int sensorPin = A0;
const int ledPin = 9;  // Must be PWM pin (3, 5, 6, 9, 10, 11)

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int sensorValue = analogRead(sensorPin);  // 0-1023
  
  // Direct mapping (problem: limited range!)
  int brightness = sensorValue / 4;  // Scale 0-1023 to 0-255
  
  analogWrite(ledPin, brightness);
  
  Serial.print("Sensor: ");
  Serial.print(sensorValue);
  Serial.print(" | Brightness: ");
  Serial.println(brightness);
  
  delay(100);
}
```

**Issue discovered:** LED doesn't reach full OFF or full MAX!  
**Why?** Photoresistor only uses 100-1000 range (not 0-1023)  
**Solution:** See Example 5 (map function)

---

### **Example 5: Full Range PWM with map() ✅ BEST VERSION**

**Purpose:** LED goes from completely OFF to maximum brightness

```cpp
// filepath: Luka/Arduino/Photoresistor/Photoresistor_PWM_Mapped.ino

/*
 * Full Range PWM Control with map()
 * Maps actual sensor range to full LED brightness range
 * This is the BEST version for smooth, full-range control!
 */

const int sensorPin = A0;
const int ledPin = 9;

// Calibration values (measured from YOUR environment)
const int sensorMin = 100;   // Darkest reading
const int sensorMax = 1000;  // Brightest reading

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  
  Serial.println("Full Range PWM Control");
  Serial.println("Calibrated: " + String(sensorMin) + "-" + String(sensorMax));
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  
  // Map sensor range (100-1000) to PWM range (0-255)
  int brightness = map(sensorValue, sensorMin, sensorMax, 0, 255);
  
  // Constrain to valid PWM range (safety)
  brightness = constrain(brightness, 0, 255);
  
  analogWrite(ledPin, brightness);
  
  Serial.print("Sensor: ");
  Serial.print(sensorValue);
  Serial.print(" | Brightness: ");
  Serial.print(brightness);
  Serial.print(" | Percentage: ");
  Serial.print(map(brightness, 0, 255, 0, 100));
  Serial.println("%");
  
  delay(100);
}
```

**The magic of map():**
```cpp
map(value, fromLow, fromHigh, toLow, toHigh)

Examples:
map(100, 100, 1000, 0, 255)  → 0   (darkest → LED off)
map(550, 100, 1000, 0, 255)  → 127 (middle → LED 50%)
map(1000, 100, 1000, 0, 255) → 255 (brightest → LED max)
```

**Calibration procedure:**
1. Run Example 2 (voltage display)
2. Note darkest value (cover sensor completely)
3. Note brightest value (flashlight or sunlight)
4. Update `sensorMin` and `sensorMax` constants
5. Upload and test!

**Result:** LED now uses FULL brightness range! ✅

---

## Key Discovery: Sensor Range Limitations

### **The Problem We Discovered:**

**Observation:** Potentiometer gives full LED range (OFF to MAX), but photoresistor doesn't!

**Why?**

### **Comparison: Potentiometer vs Photoresistor**

#### **Potentiometer (10kΩ linear):**
```
Resistance range:  0Ω → 10,000Ω (FULL RANGE)
analogRead range:  0 → 1023 (100% of ADC range)
LED brightness:    0% → 100% (FULL)
User control:      Complete (manual adjustment)
```

#### **Photoresistor (CdS cell):**
```
Resistance range:  500Ω → 200kΩ (WIDE but NEVER 0Ω!)
analogRead range:  ~100 → ~1000 (only 88% of ADC range)
LED brightness:    ~10% → ~96% (LIMITED without map())
Environmental:     Depends on available light
```

**The key difference:**
- **Potentiometer:** Designed to sweep through entire 0-10kΩ range
- **Photoresistor:** Natural resistance range doesn't include 0Ω or approach infinity
- **Room lighting:** Provides even narrower range (~5kΩ to 20kΩ)

---

### **Why Limited Range Occurs:**

**Voltage divider sensitivity:**

Maximum sensitivity when **R_variable ≈ R_fixed**

```
With 10kΩ fixed resistor:

Bright (R_ldr = 500Ω):
  V_out = 5V × (10k / 10.5k) = 4.76V → analogRead ≈ 975
  
Dark (R_ldr = 100kΩ):
  V_out = 5V × (10k / 110k) = 0.45V → analogRead ≈ 92

Range: 92-975 (only 86% of full ADC range!)
```

**Visual comparison:**

**Potentiometer:**
```
Resistance:    0Ω ────── 5kΩ ────── 10kΩ
                ↓         ↓          ↓
analogRead:    1023 ──── 682 ────── 512
                ↓         ↓          ↓
LED:           100% ──── 66% ────── 50%
              FULL RANGE
```

**Photoresistor:**
```
Resistance:    500Ω ── 5kΩ ── 10kΩ ── 50kΩ ── 100kΩ
                ↓      ↓      ↓       ↓       ↓
analogRead:    975 ── 682 ── 512 ─── 186 ─── 92
                ↓      ↓      ↓       ↓       ↓
LED:           95% ── 66% ── 50% ─── 18% ─── 9%
              LIMITED RANGE (never 0% or 100%)

Room lighting keeps sensor between 5kΩ-20kΩ
LED brightness only varies between ~30%-70%
```

---

### **Solutions to Limited Range:**

#### **Solution 1: Software Mapping (RECOMMENDED) ✅**

Use `map()` to stretch actual range to full output:

```cpp
// Measure YOUR actual range first
int sensorMin = 100;   // Darkest
int sensorMax = 1000;  // Brightest

int brightness = map(sensorValue, sensorMin, sensorMax, 0, 255);
brightness = constrain(brightness, 0, 255);
```

**Advantages:**
- ✅ No hardware changes needed
- ✅ Easy to recalibrate for different environments
- ✅ Full LED range achieved
- ✅ Most flexible solution

**See:** Example 5 code above

---

#### **Solution 2: Change Fixed Resistor (Hardware)**

**Current:** 10kΩ fixed resistor

**Alternative values to try:**
- 4.7kΩ → Shifts sensitivity toward brighter range
- 2.2kΩ → Even more sensitive to bright light
- 22kΩ → More sensitive to dark conditions

**Optimal value:**
```
R_optimal ≈ √(R_bright × R_dark)

If R_bright = 500Ω and R_dark = 100kΩ:
R_optimal ≈ √(500 × 100,000) = 7.07kΩ

Nearest standard: 6.8kΩ or 10kΩ (we're already close!)
```

**Experiment:** Try different resistors and compare ranges

---

#### **Solution 3: Reverse Circuit (Alternative Configuration)**

**Current configuration:**
```
5V → Photoresistor → A0 → 10kΩ → GND
(More light → Higher voltage)
```

**Reversed configuration:**
```
5V → 10kΩ → A0 → Photoresistor → GND
(More light → Lower voltage - INVERTED)
```

**Effect:**
- Response inverted (bright = low reading)
- Different portion of resistance range utilized
- Might give better range for your specific LDR

**Try both and compare!**

---

## Calibration & Testing

### **Calibration Table:**

Create a table of conditions vs. readings in YOUR environment:

| Light Condition | Sensor Value | Voltage | Notes |
|----------------|--------------|---------|-------|
| Covered completely | 92-120 | 0.45-0.59V | Darkest possible |
| Room lights off | 180-250 | 0.88-1.22V | Nighttime |
| Room lights on | 400-600 | 1.95-2.93V | Typical usage |
| Near window (day) | 700-850 | 3.42-4.15V | Indirect sunlight |
| Flashlight direct | 900-980 | 4.40-4.79V | Very bright |

**Your values will differ!** This depends on:
- Specific photoresistor model
- Fixed resistor value
- Ambient lighting conditions
- Sensor positioning

---

### **How to Calibrate:**

**Step 1: Find your range**
```
Upload Example 2 (voltage display)
Note minimum and maximum values in typical usage
```

**Step 2: Update code**
```cpp
const int sensorMin = [your darkest value];
const int sensorMax = [your brightest value];
```

**Step 3: Test**
```
Upload Example 5 (mapped PWM)
Verify LED goes from OFF to MAX
```

**Step 4: Fine-tune**
```
If LED never fully off: Increase sensorMin slightly
If LED never fully bright: Decrease sensorMax slightly
```

---

## Concepts Applied

### **From Findings:**

✅ **[Voltage Dividers](../Findings/Voltage-Divider.md)**
- Converting resistance to voltage
- Formula: V_out = V_in × (R2 / (R1 + R2))
- Optimal resistor selection
- Practical sensor application

✅ **[Current Limiting](../Findings/Current-Limiting.md)**
- LED protection with 220Ω resistor
- Arduino pin current limits (20mA recommended)
- Safe circuit design

### **New Concepts Learned:**

**Analog Input:**
- `analogRead()` function (0-1023)
- 10-bit ADC resolution
- 5V reference voltage
- Converting reading to voltage

**Sensor Characteristics:**
- Resistance-based sensors
- Non-linear response
- Environmental dependencies
- Range limitations vs. ideal components

**PWM Output:**
- `analogWrite()` function (0-255)
- 8-bit resolution for brightness
- Must use PWM-capable pins
- Smooth LED dimming

**Software Calibration:**
- `map()` function for range conversion
- `constrain()` for safety limits
- Calibration procedures
- Real-world sensor adaptation

---

## Common Issues & Troubleshooting

### **Problem: Readings stuck at 0 or 1023**
**Symptoms:** Value doesn't change, always at extreme  
**Likely causes:**
- Wiring error (voltage divider not formed)
- Photoresistor not connected properly
- Wrong pin specified in code

**Solutions:**
- ✅ Verify circuit matches diagram
- ✅ Check: 5V → photoresistor → A0 → 10kΩ → GND
- ✅ Measure voltage at A0 with multimeter
- ✅ Confirm `sensorPin = A0` in code

---

### **Problem: Readings don't change with light**
**Symptoms:** Covering sensor has no effect  
**Likely causes:**
- Faulty photoresistor
- Fixed resistor value too extreme
- Photoresistor in wrong position

**Solutions:**
- ✅ Test photoresistor with multimeter (resistance should change)
- ✅ Try different fixed resistor (4.7kΩ or 2.2kΩ)
- ✅ Check photoresistor orientation in circuit
- ✅ Ensure good breadboard connections

---

### **Problem: Readings very noisy/jittery**
**Symptoms:** Values jump around rapidly  
**Causes:**
- Electrical noise
- Photoresistor response time
- Poor connections

**Solutions:**
- ✅ Add averaging (see Smoothing code below)
- ✅ Increase delay between readings
- ✅ Add 0.1µF capacitor parallel to photoresistor (hardware filter)
- ✅ Check breadboard connections

**Smoothing example:**
```cpp
const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

void setup() {
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

void loop() {
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(A0);
  total = total + readings[readIndex];
  readIndex = (readIndex + 1) % numReadings;
  average = total / numReadings;
  // Use 'average' instead of raw reading
}
```

---

### **Problem: LED doesn't reach full OFF or full brightness**
**Symptoms:** LED always partly on, never maximum  
**Cause:** Sensor range limitation (see Key Discovery section)

**Solution:** ✅ Use `map()` function (Example 5)

---

### **Problem: LED behavior inverted (bright = dim, dark = bright)**
**Symptoms:** LED brighter in darkness  
**Causes:**
- Voltage divider positions swapped
- Or: Intentional design!

**Solutions:**
- If unintended: Swap photoresistor and fixed resistor positions
- If intentional: Invert in software:
  ```cpp
  brightness = map(sensorValue, sensorMin, sensorMax, 255, 0);
  // Note: 255 and 0 swapped (toLow and toHigh reversed)
  ```

---

## Phase 2 Plans (Awaiting Oscilloscope)

### **DSO138Mini Oscilloscope:**
**Status:** Ordered from AliExpress, awaiting delivery  
**Use cases:** PWM signal analysis, analog waveform visualization

### **Planned Experiments:**

#### **PWM Analysis with Oscilloscope:**
- [ ] Visualize PWM signal at different duty cycles
- [ ] Measure frequency (typically ~490Hz on Arduino pins 9, 10)
- [ ] See pulse width modulation in real-time
- [ ] Understand why `analogWrite()` creates "analog-like" behavior
- [ ] Compare different PWM values (0, 64, 128, 192, 255)

#### **Analog Signal Study:**
- [ ] View voltage divider output on scope
- [ ] See how voltage changes as light varies
- [ ] Measure response time of photoresistor
- [ ] Observe any noise or ripple
- [ ] Compare filtered vs. unfiltered signals

#### **Advanced Concepts:**
- [ ] PWM frequency modification
- [ ] Capacitor smoothing effects
- [ ] Signal noise analysis
- [ ] Voltage divider loading effects

### **Potential New Findings:**
- **PWM Basics** — How pulse width modulation works
- **Analog Input Deep Dive** — ADC operation, resolution, accuracy
- **Signal Analysis** — Using oscilloscope for debugging

### **When Oscilloscope Arrives:**
- [ ] Assemble kit (if DIY)
- [ ] Calibrate oscilloscope
- [ ] Test with known signals (5V, GND, PWM)
- [ ] Document PWM experiments
- [ ] Document analog signal analysis
- [ ] Create comprehensive Finding guides
- [ ] Update this guide to "Full Version"

---

## Project Files

### **Code Files Created:**
```
Luka/Arduino/Photoresistor/
├── Photoresistor_Basic.ino           ← Example 1: Simple reading
├── Photoresistor_Voltage.ino         ← Example 2: Voltage conversion
├── Photoresistor_Threshold.ino       ← Example 3: Dark/bright detection
├── Photoresistor_PWM_Basic.ino       ← Example 4: Basic PWM (limited range)
└── Photoresistor_PWM_Mapped.ino      ← Example 5: Full range PWM ✅ BEST
```

### **Documentation:**
```
Luka/Arduino/Photoresistor/
├── photoresistor-guide.md            ← This file
└── README.md                         ← Project summary
```

---

## Related Resources

### **Findings Referenced:**
- [Voltage Dividers](../Findings/Voltage-Divider.md) — Core circuit technique
- [Current Limiting](../Findings/Current-Limiting.md) — LED protection

### **Concepts for Future Findings:**
- **PWM Basics** — Pulse width modulation explained (awaiting oscilloscope)
- **Analog Input** — ADC operation and usage (could create now or wait for scope)
- **Sensor Calibration** — General techniques for sensor range mapping
- **Signal Filtering** — Noise reduction methods (hardware and software)

### **Arduino Reference:**
- `analogRead()` — [Arduino Docs](https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/)
- `analogWrite()` — [Arduino Docs](https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/)
- `map()` — [Arduino Docs](https://www.arduino.cc/reference/en/language/functions/math/map/)
- `constrain()` — [Arduino Docs](https://www.arduino.cc/reference/en/language/functions/math/constrain/)

---

## Session History

**Session 1 (19.10.25):** Initial photoresistor exploration
- Built basic voltage divider circuit
- Tested analog reading
- Created basic code examples

**Session 2 (20.10.25):** Range mapping discovery
- Discovered photoresistor range limitation vs. potentiometer
- Understood sensor characteristic differences
- Implemented `map()` solution for full LED range
- Created comprehensive guide (this document)

**Next session:** PWM and analog signal analysis with oscilloscope (when hardware arrives)

---

## Key Takeaways

### **What We Learned:**

1. ✅ **Photoresistors are resistance-based sensors**
   - Resistance varies with light (500Ω to 200kΩ)
   - Need voltage divider to convert to measurable voltage
   - Non-linear, logarithmic response

2. ✅ **Voltage dividers enable sensor reading**
   - Convert resistance change to voltage change
   - Fixed resistor value affects sensitivity range
   - 10kΩ is good general-purpose value

3. ✅ **Different sensors have different characteristics**
   - Potentiometer: Full, linear, user-controlled range (0-10kΩ)
   - Photoresistor: Partial, non-linear, environment-dependent range
   - Neither is "better" — different tools for different jobs

4. ✅ **Software can compensate for hardware limitations**
   - `map()` function stretches limited range to full output
   - Calibration makes sensors useful in any environment
   - More flexible than changing hardware

5. ✅ **Real-world sensors have practical limits**
   - Won't use full ADC range without mapping
   - Need calibration for specific conditions
   - This is NORMAL and expected behavior

### **Skills Developed:**

- ✅ Building voltage divider circuits
- ✅ Using analog input (analogRead)
- ✅ PWM output control (analogWrite)
- ✅ Sensor calibration procedures
- ✅ Range mapping with map()
- ✅ Debugging sensor behavior
- ✅ Comparing component characteristics
- ✅ Problem-solving with software solutions

### **Engineering Thinking:**

> **"Why doesn't the LED reach full brightness?"**  
> → Investigation → Discovery → Solution

**This is real engineering:**
1. Observe unexpected behavior
2. Form hypothesis (resistance range?)
3. Test hypothesis (compare with potentiometer)
4. Understand root cause (sensor characteristics)
5. Implement solution (software mapping)
6. Document for future reference

**Excellent work!** 🌟

---

## Next Steps

### **Immediate:**
- [x] Create comprehensive guide
- [x] Create project README (summary document)
- [x] Add to Projects-Tracker
- [ ] Consider creating "Sensor Calibration" Finding

### **When Oscilloscope Arrives:**
- [ ] Assemble DSO138Mini kit
- [ ] Run PWM experiments
- [ ] Analyze analog signals
- [ ] Create PWM Basics Finding
- [ ] Expand Analog Input Finding
- [ ] Update this guide to "Full Version"

### **Future Projects Using These Concepts:**
- Thermistor (same voltage divider technique!)
- Soil moisture sensor (analog input)
- Light-following robot (multiple photoresistors)
- Automatic night light (threshold + relay)
- Light meter (calibrated display)
- Plant monitoring system (light + moisture)

---

**Guide Status:** Phase 1 Complete ✅  
**Awaiting:** DSO138Mini Oscilloscope for Phase 2  
**Last Updated:** 20.10.25

**Remember:** You discovered the range mapping solution yourself through observation and experimentation. That's the best kind of learning! 🎯