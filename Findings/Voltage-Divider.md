# Voltage Divider Guide

**Discovered during:** Photoresistor Light Meter project (20.10.25)  
**Why it matters:** Essential for reading analog sensors with Arduino  
**Difficulty:** Beginner → Intermediate

---

## Table of Contents

1. [What is a Voltage Divider?](#what-is-a-voltage-divider)
2. [Why Do We Need It?](#why-do-we-need-it)
3. [How It Works (Simple)](#how-it-works-simple)
4. [The Math (Technical)](#the-math-technical)
5. [Real-World Examples](#real-world-examples)
6. [How to Choose Resistor Values](#how-to-choose-resistor-values)
7. [Common Mistakes](#common-mistakes)
8. [Quick Reference & Calculator](#quick-reference--calculator)
9. [Related Concepts](#related-concepts)

---

## What is a Voltage Divider?

### The Simple Explanation

A voltage divider is **two resistors in series** that split a voltage into a smaller voltage.

Think of it like a water pipe with two valves:
- First valve (R1) restricts some flow
- Second valve (R2) restricts more flow
- Pressure between them = the voltage we measure

**Visual:**
```
     INPUT VOLTAGE (5V)
           │
         ┌─┴─┐
         │ R1 │ ← First resistor
         └─┬─┘
           │ ← OUTPUT VOLTAGE (measured here)
         ┌─┴─┐
         │ R2 │ ← Second resistor
         └─┬─┘
           │
          GND (0V)
```

**Key insight:** The voltage in the middle depends on the **ratio** of the two resistors.

---

## Why Do We Need It?

### Problem: Arduino Can't Measure Resistance

**Arduino's limitation:**
- `analogRead()` measures **voltage** (0-5V)
- Many sensors are **variable resistors** (resistance changes)
- Can't directly read resistance!

**Solution:** Voltage divider converts resistance → voltage

### Three Main Uses

#### 1. Reading Variable Resistor Sensors
```
Sensor examples:
- Photoresistor (LDR) - light level
- Thermistor - temperature
- Flex sensor - bend angle
- Force-sensitive resistor (FSR) - pressure

All are variable resistors!
Need voltage divider to read them.
```

#### 2. Reducing High Voltages Safely
```
Problem: Want to measure 9V battery with Arduino
Risk: 9V would damage Arduino (max 5V input)
Solution: Voltage divider reduces 9V → safe voltage (e.g., 3V)
```

#### 3. Creating Reference Voltages
```
Example: Need 2.5V reference (half of 5V)
Solution: Two equal resistors divide 5V exactly in half
```

---

## How It Works (Simple)

### The Water Analogy

Imagine a water tower (5V) and two valves:

**Equal valves (equal resistors):**
```
Water Tower (5V)
      │
    [Valve 1] 50% restriction ← R1 = 10kΩ
      │
    [Pressure gauge] reads 2.5V (half)
      │
    [Valve 2] 50% restriction ← R2 = 10kΩ
      │
    Ground (0V)

Result: Pressure exactly in middle (2.5V)
```

**Unequal valves (unequal resistors):**
```
Water Tower (5V)
      │
    [Valve 1] 10% restriction ← R1 = 1kΩ (small)
      │
    [Pressure gauge] reads 4.5V (high)
      │
    [Valve 2] 90% restriction ← R2 = 10kΩ (large)
      │
    Ground (0V)

Result: Pressure mostly maintained (4.5V)
```

**The rule:**
- Bigger R2 (bottom) = Higher voltage at midpoint
- Bigger R1 (top) = Lower voltage at midpoint
- Equal resistors = Voltage exactly in middle

---

### Your Photoresistor Circuit Explained

**The circuit:**
```
     5V
      │
    ┌─┴─┐
    │LDR│ ← R1 = Variable (1kΩ - 100kΩ)
    └─┬─┘
      │ ← A0 reads this voltage
    ┌─┴─┐
    │10k│ ← R2 = Fixed (10kΩ)
    └─┬─┘
      │
     GND
```

**What happens:**

**Bright light → LDR = 1kΩ (low resistance):**
```
Small valve on top (R1 = 1kΩ)
Big valve on bottom (R2 = 10kΩ)
Result: Voltage stays high (4.5V)
Arduino reads: ~920 / 1023
```

**Dark room → LDR = 100kΩ (high resistance):**
```
Big valve on top (R1 = 100kΩ)
Small valve on bottom (R2 = 10kΩ)
Result: Voltage drops low (0.45V)
Arduino reads: ~92 / 1023
```

**See the magic?**
- Light changes resistance
- Resistance changes voltage
- Arduino measures voltage
- You detect light level!

---

## The Math (Technical)

### The Voltage Divider Formula

```
V_out = V_in × (R2 / (R1 + R2))

Where:
V_in  = Input voltage (usually 5V for Arduino)
V_out = Output voltage (what Arduino reads at A0)
R1    = Top resistor (or sensor resistance)
R2    = Bottom resistor (fixed reference)
```

**Alternative formula (solving for V_out):**
```
V_out = V_in × R2 / (R1 + R2)
```

**Percentage version:**
```
V_out = V_in × (R2 / Total_Resistance)

Where Total_Resistance = R1 + R2
```

---

### Example Calculations

#### Example 1: Equal Resistors (50/50 split)

**Circuit:**
```
5V → 10kΩ → [Midpoint] → 10kΩ → GND
```

**Calculation:**
```
V_out = 5V × (10kΩ / (10kΩ + 10kΩ))
V_out = 5V × (10kΩ / 20kΩ)
V_out = 5V × 0.5
V_out = 2.5V

Arduino reads: 2.5V / 5V × 1023 = 512
```

**Result:** Exactly half voltage (2.5V)

---

#### Example 2: Photoresistor in Bright Light

**Circuit:**
```
5V → LDR(1kΩ) → [A0] → 10kΩ → GND
```

**Calculation:**
```
V_out = 5V × (10kΩ / (1kΩ + 10kΩ))
V_out = 5V × (10kΩ / 11kΩ)
V_out = 5V × 0.909
V_out = 4.545V

Arduino reads: 4.545V / 5V × 1023 = 930
```

**Result:** High voltage = bright light detected

---

#### Example 3: Photoresistor in Darkness

**Circuit:**
```
5V → LDR(100kΩ) → [A0] → 10kΩ → GND
```

**Calculation:**
```
V_out = 5V × (10kΩ / (100kΩ + 10kΩ))
V_out = 5V × (10kΩ / 110kΩ)
V_out = 5V × 0.091
V_out = 0.455V

Arduino reads: 0.455V / 5V × 1023 = 93
```

**Result:** Low voltage = darkness detected

---

#### Example 4: Measuring 9V Battery Safely

**Goal:** Read 9V battery without damaging Arduino (max 5V input)

**Circuit:**
```
9V → 20kΩ → [A0] → 10kΩ → GND
```

**Calculation:**
```
V_out = 9V × (10kΩ / (20kΩ + 10kΩ))
V_out = 9V × (10kΩ / 30kΩ)
V_out = 9V × 0.333
V_out = 3V (safe!)

Arduino reads: 3V (raw value ~614)
Actual battery: 3V × 3 = 9V (multiply back)
```

**Result:** Safe voltage, can calculate original voltage

---

### Ratio Table (Quick Reference)

| R1 (Top) | R2 (Bottom) | Ratio R2/(R1+R2) | V_out (from 5V) | Percentage |
|----------|-------------|------------------|-----------------|------------|
| 1kΩ      | 10kΩ        | 0.909            | 4.5V            | 91%        |
| 2kΩ      | 10kΩ        | 0.833            | 4.2V            | 83%        |
| 5kΩ      | 10kΩ        | 0.667            | 3.3V            | 67%        |
| 10kΩ     | 10kΩ        | 0.500            | 2.5V            | 50%        |
| 10kΩ     | 5kΩ         | 0.333            | 1.7V            | 33%        |
| 10kΩ     | 2kΩ         | 0.167            | 0.8V            | 17%        |
| 10kΩ     | 1kΩ         | 0.091            | 0.5V            | 9%         |

**Pattern:**
- Larger R2 = Higher output voltage
- Equal resistors = 50% voltage
- R2 = 2×R1 = 67% voltage
- R2 = 0.5×R1 = 33% voltage

---

## Real-World Examples

### Example 1: Photoresistor (Your Current Project)

**Purpose:** Detect light level  
**Component:** LDR 5539 (Light Dependent Resistor)

**Specifications:**
```
Bright light: ~1kΩ
Room light:   ~10kΩ
Darkness:     ~100kΩ+
```

**Circuit:**
```
5V → LDR (variable) → A0 → 10kΩ → GND
```

**Why 10kΩ?**
- Matches middle of LDR range
- Gives good sensitivity in both bright and dark
- Standard value for most photoresistors

**Code example:**
```cpp
int lightLevel = analogRead(A0);

if (lightLevel > 700) {
  Serial.println("Bright!");
} else if (lightLevel < 300) {
  Serial.println("Dark!");
} else {
  Serial.println("Medium light");
}
```

**What you'll read:**
```
Bright:  900-1000 (very high)
Room:    400-600  (medium)
Dark:    50-200   (low)
```

---

### Example 2: Thermistor (Temperature Sensor)

**Purpose:** Measure temperature  
**Component:** NTC Thermistor 10kΩ @ 25°C

**Specifications:**
```
Hot (50°C):   ~3kΩ
Room (25°C):  ~10kΩ
Cold (0°C):   ~30kΩ
```

**Circuit:**
```
5V → Thermistor (variable) → A0 → 10kΩ → GND
```

**Why 10kΩ?**
- Matches thermistor value at room temperature (25°C)
- Best sensitivity around normal temperatures
- Standard for 10kΩ thermistors

**Code example:**
```cpp
int tempReading = analogRead(A0);
// Convert to voltage
float voltage = tempReading * (5.0 / 1023.0);
// Calculate resistance
float resistance = (10000.0 * voltage) / (5.0 - voltage);
// Convert to temperature (simplified)
// (Use Steinhart-Hart equation for accuracy)
```

**What you'll read:**
```
Hot:   800-900 (high voltage, low resistance)
Room:  500-550 (medium)
Cold:  200-300 (low voltage, high resistance)
```

---

### Example 3: FSR (Force Sensitive Resistor)

**Purpose:** Detect pressure/force  
**Component:** FSR 402 (force sensor)

**Specifications:**
```
No pressure:     >1MΩ (infinite)
Light touch:     ~100kΩ
Firm pressure:   ~10kΩ
Hard squeeze:    ~1kΩ
```

**Circuit:**
```
5V → FSR (variable) → A0 → 10kΩ → GND
```

**Why 10kΩ?**
- Good for detecting light to medium pressure
- Range matches typical hand pressure
- Can detect subtle changes

**Code example:**
```cpp
int pressure = analogRead(A0);

if (pressure > 800) {
  Serial.println("Hard press!");
} else if (pressure > 400) {
  Serial.println("Light touch");
} else {
  Serial.println("No pressure");
}
```

**What you'll read:**
```
Hard squeeze:  900-1000
Medium press:  600-800
Light touch:   300-500
No touch:      0-100
```

---

### Example 4: Measuring 9V Battery

**Purpose:** Monitor battery voltage safely  
**Goal:** Read 9V without exceeding Arduino's 5V limit

**Circuit:**
```
9V Battery (+) → 20kΩ → A0 → 10kΩ → GND (Battery -)
```

**Calculation:**
```
Divider ratio: 10kΩ / (20kΩ + 10kΩ) = 0.333
Max voltage at A0: 9V × 0.333 = 3V (safe!)

If battery drops to 6V:
A0 reads: 6V × 0.333 = 2V
```

**Code example:**
```cpp
int reading = analogRead(A0);
float voltage_A0 = reading * (5.0 / 1023.0);
float actual_voltage = voltage_A0 * 3.0;  // Multiply back by ratio

Serial.print("Battery: ");
Serial.print(actual_voltage);
Serial.println("V");

if (actual_voltage < 7.0) {
  Serial.println("Battery low!");
}
```

**What you'll read:**
```
Fresh 9V:    ~3V at A0 → 9V calculated
Used 7.5V:   ~2.5V at A0 → 7.5V calculated
Dead 6V:     ~2V at A0 → 6V calculated
```

---

### Example 5: Reading 12V Power Supply

**Purpose:** Monitor 12V system (LED strip, motor power)  
**Goal:** Read 12V safely with Arduino

**Circuit:**
```
12V Supply (+) → 30kΩ → A0 → 10kΩ → GND (Supply -)
```

**Calculation:**
```
Divider ratio: 10kΩ / (30kΩ + 10kΩ) = 0.25
Max voltage at A0: 12V × 0.25 = 3V (safe!)
```

**Safety margin:**
- Even if supply goes up to 15V: 15V × 0.25 = 3.75V (still safe)
- Arduino max: 5V
- Margin: 5V - 3.75V = 1.25V buffer

**Code example:**
```cpp
int reading = analogRead(A0);
float voltage_A0 = reading * (5.0 / 1023.0);
float supply_voltage = voltage_A0 * 4.0;  // Multiply back

Serial.print("Supply: ");
Serial.print(supply_voltage);
Serial.println("V");

if (supply_voltage < 11.0) {
  Serial.println("Power supply dropping!");
}
```

---

## How to Choose Resistor Values

### Step-by-Step Guide

#### Step 1: Identify Your Sensor's Range

**For variable resistors (sensors):**
```
Example - Photoresistor LDR 5539:
Minimum resistance (brightest): ~1kΩ
Maximum resistance (darkest):   ~100kΩ
Mid-range:                      ~10kΩ
```

**For fixed voltage division:**
```
Example - Measuring 9V with 5V Arduino:
Input: 9V
Safe output: <5V (let's target 3V)
Ratio needed: 3V / 9V = 0.333 (33%)
```

---

#### Step 2: Choose Fixed Resistor (R2)

**For sensors (variable R1):**
```
Rule: R2 ≈ Mid-range of sensor resistance

Photoresistor (1kΩ - 100kΩ):
Mid-range ≈ √(1kΩ × 100kΩ) ≈ 10kΩ
Choose: 10kΩ ✓

Thermistor 10kΩ @ 25°C:
Rated value = 10kΩ
Choose: 10kΩ ✓

FSR (1kΩ - 1MΩ):
Target medium pressure (~10kΩ)
Choose: 10kΩ ✓
```

**For voltage division:**
```
Rule: Choose R2, then calculate R1

Target output: 3V from 9V input
Ratio: R2 / (R1 + R2) = 3V / 9V = 0.333

Choose R2 = 10kΩ (common value)
Solve for R1:
0.333 = 10kΩ / (R1 + 10kΩ)
0.333(R1 + 10kΩ) = 10kΩ
0.333R1 + 3.33kΩ = 10kΩ
0.333R1 = 6.67kΩ
R1 = 20kΩ ✓
```

---

#### Step 3: Calculate Expected Voltages

**Use the formula:**
```
V_out = V_in × (R2 / (R1 + R2))
```

**Test extreme cases:**

**Photoresistor example:**
```
Brightest (R1 = 1kΩ, R2 = 10kΩ):
V_out = 5V × (10kΩ / 11kΩ) = 4.5V
Arduino: 920 / 1023 ✓

Darkest (R1 = 100kΩ, R2 = 10kΩ):
V_out = 5V × (10kΩ / 110kΩ) = 0.45V
Arduino: 92 / 1023 ✓

Good range! (92 to 920)
```

**Voltage measurement example:**
```
9V battery (R1 = 20kΩ, R2 = 10kΩ):
V_out = 9V × (10kΩ / 30kΩ) = 3V
Safe: < 5V ✓

If battery dies (6V):
V_out = 6V × 0.333 = 2V
Still safe ✓
```

---

#### Step 4: Consider Current Draw

**Power consumption matters!**

**Formula:**
```
I = V_in / (R1 + R2)
P = V_in × I
```

**Example 1: Small resistors (wasteful)**
```
R1 = 1kΩ, R2 = 1kΩ, V_in = 5V
I = 5V / 2kΩ = 2.5mA
P = 5V × 2.5mA = 12.5mW

Over time: Wastes battery power
```

**Example 2: Large resistors (efficient)**
```
R1 = 10kΩ, R2 = 10kΩ, V_in = 5V
I = 5V / 20kΩ = 0.25mA
P = 5V × 0.25mA = 1.25mW

Much better! 10x less power
```

**Rule of thumb:**
```
For sensors:      10kΩ - 100kΩ (good balance)
For battery reading: 10kΩ - 47kΩ (low power)
For precision:    <10kΩ (less noise, more power)
For battery projects: >47kΩ (very low power)
```

---

### Common Resistor Value Pairs

| Application | R1 (Top) | R2 (Bottom) | Output Ratio | Use Case |
|-------------|----------|-------------|--------------|----------|
| **Sensors** |
| Photoresistor | Variable (1k-100k) | 10kΩ | Variable | Light detection |
| Thermistor 10k | Variable (3k-30k) | 10kΩ | Variable | Temperature |
| FSR | Variable (1k-1M) | 10kΩ | Variable | Pressure |
| Potentiometer | Built-in divider | - | Variable | User input |
| **Voltage Measurement** |
| 9V battery | 20kΩ | 10kΩ | 33% (3V) | Battery monitor |
| 12V supply | 30kΩ | 10kΩ | 25% (3V) | Power monitor |
| 15V max | 40kΩ | 10kΩ | 20% (3V) | High voltage |
| **Reference Voltages** |
| Half voltage (2.5V) | 10kΩ | 10kΩ | 50% | Reference |
| Third voltage (1.67V) | 20kΩ | 10kΩ | 33% | Reference |
| Quarter voltage (1.25V) | 30kΩ | 10kΩ | 25% | Reference |

---

### Decision Flow Chart

```
START: Need to read analog value?
  │
  ├─→ [Variable resistor sensor?]
  │     YES → Use R2 = 10kΩ (standard)
  │            → Check sensor datasheet for range
  │            → Calculate expected voltages
  │            → Test: Does it give good range (100-900)?
  │               YES → Done! ✓
  │               NO → Try different R2 value
  │
  └─→ [Measuring voltage higher than 5V?]
        YES → Calculate ratio needed
             → Choose R2 = 10kΩ
             → Solve for R1
             → Verify: Output < 5V at max input?
                YES → Done! ✓
                NO → Increase R1 or decrease R2
```

---

## Common Mistakes

### Mistake 1: No Fixed Resistor (Your Original Error!)

**What you did:**
```
5V → Photoresistor → A0 → GND
      (No R2!)
```

**Why it doesn't work:**
- Photoresistor resistance varies
- But voltage at A0 has no reference point
- Arduino sees unpredictable voltage
- Readings unstable or stuck at extremes

**Fix:**
```
5V → Photoresistor → A0 → 10kΩ → GND
                           (R2 added!)
```

**Now it works:**
- 10kΩ provides stable reference
- Voltage divider creates measurable voltage
- Arduino sees predictable range (0.45V - 4.5V)

---

### Mistake 2: Wrong Resistor Value

**Example: Using 100Ω instead of 10kΩ**

**Circuit:**
```
5V → Photoresistor (1kΩ-100kΩ) → A0 → 100Ω → GND
                                        (Too small!)
```

**What happens:**

**In bright light (LDR = 1kΩ):**
```
V_out = 5V × (100Ω / 1100Ω) = 0.45V
Arduino: 92 (very low)
```

**In darkness (LDR = 100kΩ):**
```
V_out = 5V × (100Ω / 100.1kΩ) ≈ 0.005V
Arduino: 1 (almost zero!)
```

**Problem:** Can't detect brightness, only darkness!

**Fix:** Use 10kΩ instead of 100Ω

---

### Mistake 3: Exceeding 5V Input

**Dangerous circuit:**
```
9V Battery → 5kΩ → A0 → 1kΩ → GND
```

**Calculation:**
```
V_out = 9V × (1kΩ / 6kΩ) = 1.5V... wait, that's safe?
```

**Hidden danger:**
```
If battery is fresh: 9.6V × (1kΩ / 6kΩ) = 1.6V ✓
If you replace with 12V by mistake: 12V × (1kΩ / 6kΩ) = 2V ✓
Still safe... BUT:

What if R1 fails (opens)?
Now: 9V → NOTHING → A0
Result: Full 9V to A0 → Arduino DAMAGED! 💥
```

**Safe design:**
```
9V Battery → 20kΩ → A0 → 10kΩ → GND

Even at 12V: 12V × 0.333 = 4V (still safe)
Safety margin: 5V - 4V = 1V buffer
```

**Rule:** Always design with 20% safety margin!

---

### Mistake 4: Resistors Backwards

**Intended circuit (correct):**
```
5V → LDR → A0 → 10kΩ → GND

Bright: High voltage (4.5V)
Dark:   Low voltage (0.45V)
```

**Accidental swap:**
```
5V → 10kΩ → A0 → LDR → GND

Bright: Low voltage! (0.45V)
Dark:   High voltage! (4.5V)
```

**What happens:**
- Circuit still works
- But readings are inverted!
- Bright reads as dark, dark reads as bright

**Fix in code:**
```cpp
// Option 1: Fix hardware (swap resistors)

// Option 2: Invert in software
int rawReading = analogRead(A0);
int corrected = 1023 - rawReading;  // Flip values
```

**Lesson:** Position matters! Variable resistor usually goes on top.

---

### Mistake 5: Loading Effect (Advanced)

**The problem:**

**Simple divider:**
```
5V → 1MΩ → A0 → 1MΩ → GND
Expected: 2.5V
```

**But Arduino input has impedance:**
```
5V → 1MΩ → A0 (→ 100MΩ internal) → 1MΩ → GND
                   (Arduino input impedance)
```

**Effect:**
- High resistor values + input impedance = voltage error
- With 1MΩ resistors: ~1% error (minor)
- With 10MΩ resistors: ~10% error (significant!)

**Solution:**
```
Use resistors < 100kΩ for accurate readings
Ideal: 1kΩ - 47kΩ range
Acceptable: up to 100kΩ
Avoid: > 1MΩ (unless you know what you're doing)
```

**For sensors, 10kΩ is perfect!**

---

### Mistake 6: Forgetting Power Consumption

**Wasteful design:**
```
5V → 220Ω → A0 → 220Ω → GND

Current: 5V / 440Ω = 11.4mA
Power: 5V × 11.4mA = 57mW

Runs 24/7:
Energy per day: 57mW × 24h = 1.37Wh
Over a year: 500Wh wasted!
```

**Efficient design:**
```
5V → 10kΩ → A0 → 10kΩ → GND

Current: 5V / 20kΩ = 0.25mA
Power: 5V × 0.25mA = 1.25mW

Runs 24/7:
Energy per day: 1.25mW × 24h = 0.03Wh
Over a year: 11Wh (45x less!)
```

**For battery projects:** Use 47kΩ - 100kΩ resistors

---

### Mistake 7: Not Testing Extremes

**Assumption:**
```
"10kΩ should work for my photoresistor"
```

**Reality check needed:**

**Test 1: Bright light (flashlight directly on sensor)**
```
Reading: 950 / 1023 ✓ Good
```

**Test 2: Normal room light**
```
Reading: 520 / 1023 ✓ Good
```

**Test 3: Cover sensor completely**
```
Reading: 200 / 1023 ✓ Good
```

**Test 4: Dark room at night**
```
Reading: 5 / 1023 ⚠️ Too low!
Can't distinguish darkness levels
```

**Solution:** Might need larger R2 (22kΩ or 47kΩ) for better dark sensitivity

**Always test full range before finalizing design!**

---

## Quick Reference & Calculator

### Standard Formula

```
V_out = V_in × (R2 / (R1 + R2))
```

### Solving for Different Variables

**Given V_out, find R1:**
```
R1 = R2 × ((V_in / V_out) - 1)

Example:
V_in = 9V, V_out = 3V, R2 = 10kΩ
R1 = 10kΩ × ((9V / 3V) - 1)
R1 = 10kΩ × (3 - 1)
R1 = 10kΩ × 2
R1 = 20kΩ
```

**Given V_out, find R2:**
```
R2 = R1 × (V_out / (V_in - V_out))

Example:
V_in = 5V, V_out = 2V, R1 = 15kΩ
R2 = 15kΩ × (2V / (5V - 2V))
R2 = 15kΩ × (2V / 3V)
R2 = 15kΩ × 0.667
R2 = 10kΩ
```

### Arduino Conversion

**Voltage to analogRead() value:**
```
reading = (V_out / 5.0) × 1023

Example:
V_out = 3.5V
reading = (3.5V / 5.0) × 1023
reading = 0.7 × 1023
reading = 716
```

**analogRead() value to voltage:**
```
V_out = (reading / 1023.0) × 5.0

Example:
reading = 512
V_out = (512 / 1023.0) × 5.0
V_out = 0.5 × 5.0
V_out = 2.5V
```

### Current and Power

**Current through divider:**
```
I = V_in / (R1 + R2)

Example:
V_in = 5V, R1 = 10kΩ, R2 = 10kΩ
I = 5V / 20kΩ
I = 0.25mA
```

**Power dissipation:**
```
P_total = V_in × I
P_R1 = I² × R1
P_R2 = I² × R2

Example:
I = 0.25mA, R1 = 10kΩ, R2 = 10kΩ
P_total = 5V × 0.25mA = 1.25mW
P_R1 = (0.25mA)² × 10kΩ = 0.625mW
P_R2 = (0.25mA)² × 10kΩ = 0.625mW
```

---

### Quick Calculation Tool (Mental Math)

**For equal resistors (50% voltage):**
```
V_out = V_in / 2

5V → 2.5V
9V → 4.5V
12V → 6V
```

**For 1:2 ratio (33% voltage):**
```
V_out = V_in / 3

9V → 3V (perfect for measuring 9V battery!)
12V → 4V
15V → 5V
```

**For 1:3 ratio (25% voltage):**
```
V_out = V_in / 4

12V → 3V (perfect for measuring 12V supply!)
16V → 4V
20V → 5V
```

**For 1:4 ratio (20% voltage):**
```
V_out = V_in / 5

15V → 3V
20V → 4V
25V → 5V
```

---

### Resistor Ratio Table

| R1 : R2 | Multiplier | V_out from 5V | V_out from 12V | Use Case |
|---------|------------|---------------|----------------|----------|
| 1:1 | ÷2 | 2.5V | 6V | Half voltage |
| 1:2 | ÷3 | 1.67V | 4V | Third voltage |
| 2:1 | ×0.66 | 3.3V | 8V | Two-thirds |
| 1:3 | ÷4 | 1.25V | 3V | Quarter voltage |
| 3:1 | ×0.75 | 3.75V | 9V | Three-quarters |
| 1:4 | ÷5 | 1V | 2.4V | Fifth voltage |
| 1:9 | ÷10 | 0.5V | 1.2V | Tenth voltage |

---

### Code Template

```cpp
// Voltage divider reading template
const int SENSOR_PIN = A0;
const float V_IN = 5.0;              // Arduino supply voltage
const int R1 = 10000;                 // Top resistor (or sensor)
const int R2 = 10000;                 // Bottom resistor (fixed)

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
}

void loop() {
  // Read analog value (0-1023)
  int rawValue = analogRead(SENSOR_PIN);
  
  // Convert to voltage at A0
  float voltage = (rawValue / 1023.0) * V_IN;
  
  // Calculate sensor resistance (if using variable resistor sensor)
  float sensorResistance = R2 * (V_IN - voltage) / voltage;
  
  // Or calculate original voltage (if measuring higher voltage)
  float originalVoltage = voltage * (R1 + R2) / R2;
  
  // Display results
  Serial.print("Raw: ");
  Serial.print(rawValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 2);
  Serial.print("V | Resistance: ");
  Serial.print(sensorResistance / 1000.0, 1);
  Serial.println("kΩ");
  
  delay(500);
}
```

---

## Related Concepts

### 1. Current Limiting (with LEDs)

**Similarity:**
- Both use resistors to control circuit behavior
- Both follow Ohm's Law

**Difference:**
```
Voltage divider:     Creates specific voltage
Current limiter:     Limits current flow

LED circuit:
5V → 220Ω → LED → GND
     (limits current to ~14mA)

Voltage divider:
5V → R1 → [read voltage] → R2 → GND
```

**Learn more:** See `Findings/Current-Limiting.md` (from LED project)

---

### 2. Pull-Up and Pull-Down Resistors

**Connection:**
- Pull-up/pull-down IS a voltage divider!
- One resistor is the pull-up/pull-down
- Other "resistor" is button (open = infinite, closed = 0Ω)

**Pull-up example:**
```
5V → 10kΩ → Digital Pin → Button → GND
     (R1)       (read)      (R2 = 0Ω or ∞)

Button released: R2 = ∞, voltage ≈ 5V (HIGH)
Button pressed:  R2 = 0Ω, voltage ≈ 0V (LOW)
```

**It's a voltage divider with a switch!**

---

### 3. Wheatstone Bridge

**Advanced voltage divider:**
- Two voltage dividers in parallel
- Compares their outputs
- Used for precise sensor measurements

```
     5V
    /   \
   R1   R3
   |     |
   +--A--+
   |     |
   R2   Rx (sensor)
    \   /
     GND

If R1/R2 = R3/Rx, voltage at A = 0
Any change in Rx creates voltage difference
Very sensitive!
```

**Use:** Strain gauges, load cells, precision sensors

---

### 4. Potentiometer (Built-in Voltage Divider)

**How potentiometers work:**
```
Potentiometer = Variable voltage divider built into one component

Internal structure:
5V → [Resistive track] → GND
         ↑
    Wiper (adjustable)
    
Wiper position = voltage divider ratio
0%   position → 0V   (wiper at GND end)
50%  position → 2.5V (wiper at middle)
100% position → 5V   (wiper at 5V end)
```

**It's an adjustable R1/R2 divider!**

---

### 5. Sensor Linearization

**Problem:** Many sensors aren't linear

**Thermistor example:**
- Resistance vs. Temperature is exponential, not linear
- Voltage divider makes it worse!
- Need math correction (Steinhart-Hart equation)

**Photoresistor example:**
- Light vs. Resistance is logarithmic
- Need calibration curve or lookup table

**Solution:** Software compensation after reading

---

### 6. Voltage References

**Precision measurements need stable references:**

**Problem:**
```
Arduino 5V isn't exactly 5.00V
Might be 4.85V - 5.15V
Affects all analogRead() accuracy
```

**Solution:**
```
Use external voltage reference (2.5V, 4.096V)
Arduino has AREF pin for this
Or use voltage divider to create known reference
```

---

## Summary

### Key Takeaways

**What it is:**
- Two resistors in series that split voltage
- Creates predictable voltage at junction point
- Essential for reading analog sensors with Arduino

**When to use:**
- Variable resistor sensors (photoresistor, thermistor, FSR)
- Measuring voltages higher than 5V safely
- Creating reference voltages

**How to design:**
1. Identify sensor range or target voltage
2. Choose R2 (~10kΩ standard for sensors)
3. Calculate R1 or expected voltages
4. Test extremes before finalizing
5. Consider power consumption

**Common mistakes:**
- Forgetting the fixed resistor (no voltage division!)
- Wrong resistor values (no useful range)
- Exceeding 5V input (Arduino damage)
- Not testing full sensor range
- Ignoring power consumption

**Magic formula:**
```
V_out = V_in × (R2 / (R1 + R2))
```

---

### Next Steps

**Practice projects:**
1. ✅ Photoresistor light meter (your current project!)
2. Thermistor temperature monitor
3. FSR pressure sensor
4. Battery voltage monitor
5. Potentiometer control (built-in divider)

**Advanced topics:**
- Wheatstone bridge for precision
- Sensor linearization and calibration
- Multiple sensors on one Arduino
- Low-power sensor reading techniques

---

**Discovered:** 20.10.25 during Photoresistor project  
**Related files:**
- `Luka/Arduino/Photoresistor/` (project code)
- `Projects-Tracker.md` (session history)

**Questions or improvements?** Add notes here as you learn more!