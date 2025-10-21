# LED Cycling with Resistor Comparison

**Created:** 07.10.25  
**Completed:** 08.10.25  
**Status:** Completed ✓  
**Difficulty:** Beginner-Intermediate

---

## Description

An Arduino project that cycles through 10 LEDs using a single button, with each LED having a different resistor value to observe brightness variations and learn about current limiting.

**Key features:**
- Button-controlled LED cycling (one LED at a time)
- 10 different resistor values (47Ω to 100kΩ range)
- Real-time voltage monitoring via analog pin A0
- Serial monitor debugging output
- Non-blocking code with proper debouncing

**Purpose:** Understand how resistor values affect LED brightness and current limiting principles.

---

## Components

**Required:**
- [x] Arduino Uno
- [x] 10 LEDs (same type/color for comparison)
- [x] 10 different resistors (47Ω, 100Ω, 220Ω, 470Ω, 1kΩ, 2.2kΩ, 4.7kΩ, 10kΩ, 47kΩ, 100kΩ)
- [x] 1 push button (tactile switch)
- [x] 1× 10kΩ resistor (button pull-down)
- [x] Breadboard
- [x] Jumper wires

**Optional:**
- [ ] Multimeter (for measuring actual current/voltage)

---

## Circuit

**LED Wiring (10 different circuits):**
```
Arduino Pin 2  → 47Ω    → LED → GND
Arduino Pin 3  → 100Ω   → LED → GND
Arduino Pin 4  → 220Ω   → LED → GND
Arduino Pin 5  → 470Ω   → LED → GND
Arduino Pin 6  → 1kΩ    → LED → GND
Arduino Pin 7  → 2.2kΩ  → LED → GND
Arduino Pin 8  → 4.7kΩ  → LED → GND
Arduino Pin 9  → 10kΩ   → LED → GND
Arduino Pin 10 → 47kΩ   → LED → GND
Arduino Pin 11 → 100kΩ  → LED → GND
```

**Button Wiring:**
```
Digital Pin 12 → Button → GND
Digital Pin 12 → 10kΩ pull-down resistor → 5V
```

**Voltage Monitor:**
```
5V → A0 (analog input for voltage monitoring)
```

**Important notes:**
- ⚠️ Only ONE LED lights at a time (cycling through sequence)
- 💡 Arrange LEDs in order to compare brightness easily
- ⚠️ Button needs pull-down resistor to prevent floating input

---

## How to Use

**Setup:**
1. Upload code to Arduino
2. Open Serial Monitor (9600 baud)
3. Observe first LED (pin 2 with 47Ω resistor)

**Operation:**
1. Press button → Next LED lights up (previous turns off)
2. Cycle through all 10 LEDs in sequence
3. After LED 10, wraps back to LED 1
4. Serial Monitor shows:
   - Which LED is currently on
   - Voltage reading (continuous monitoring)

**Expected behavior:**
- Lower resistance (47Ω) → Brighter LED
- Higher resistance (100kΩ) → Dimmer/off LED
- Smooth cycling with proper debouncing (no double-triggers)

---

## Concepts Used

This project demonstrates these fundamental concepts:

### Current Limiting _(to be extracted to Findings/)_
**How it's used here:** Each LED has different resistor value to compare brightness  
**Why it matters:** Shows how resistor value affects current and brightness  
**Discovery:** 
- 47Ω-220Ω range: Visible brightness (safe current)
- 470Ω-2.2kΩ: Progressively dimmer
- 10kΩ+: Very dim or invisible (current too low)
- **Key insight:** Resistor value directly controls LED brightness through current limiting

**Experiment results:**
- Initial range (47Ω to 100kΩ) too extreme for comparison
- Series 10kΩ addition made all LEDs equally dim (dominated circuit)
- Optimal comparison range: 100Ω to 2.2kΩ

### Button Debouncing
**How it's used here:** 50ms delay prevents multiple LED switches from single press  
**Why it matters:** Mechanical buttons "bounce" causing multiple triggers  
**Implementation:**
```cpp
if (buttonState == HIGH && lastButtonState == LOW) {
  delay(50);  // Debounce delay
  // Edge detection: only trigger on press, not hold
}
```

### Edge Detection
**How it's used here:** Detect button press (LOW→HIGH transition), not continuous hold  
**Why it matters:** Cycles LED once per press, not continuously while held  
**Pattern:**
```cpp
if (buttonState == HIGH && lastButtonState == LOW) {
  // Rising edge detected - button just pressed
  thisPin++;  // Cycle to next LED
}
lastButtonState = buttonState;  // Remember state for next loop
```

### Arrays in Arduino
**How it's used here:** `int ledPins[10]` stores all LED pin numbers  
**Why it matters:** Scalable approach (better than 10 separate variables)  
**Example:**
```cpp
int ledPins[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
thisPin = (thisPin + 1) % 10;  // Modulo wraparound
digitalWrite(ledPins[thisPin], HIGH);
```

### Non-Blocking Code (millis())
**How it's used here:** Voltage monitoring runs continuously without blocking button  
**Why it matters:** Eliminates `delay()` that prevents responsive button reading  
**Before (blocking):**
```cpp
void Volts() {
  delay(1000);  // Blocks entire program for 1 second!
}
```
**After (non-blocking):**
```cpp
unsigned long previousMillis = 0;
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {
    // Read voltage without blocking
    previousMillis = currentMillis;
  }
}
```

### Analog Input (analogRead)
**How it's used here:** Monitoring 5V supply voltage via A0  
**Why it matters:** Real-time voltage feedback for circuit analysis  
**Example:**
```cpp
int voltageReading = analogRead(A0);
float voltage = voltageReading * (5.0 / 1023.0);
Serial.print("Voltage: ");
Serial.println(voltage);
```

---

## Code Highlights

**Key initialization:**
```cpp
int thisPin = -1;  // Start at -1 so first press goes to pin 0 (LED 1)

void setup() {
  for (int i = 0; i < 10; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);  // All LEDs start off
  }
}
```

**Button handling with edge detection:**
```cpp
void loop() {
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH && lastButtonState == LOW) {
    delay(50);  // Debounce
    
    // Turn off current LED
    if (thisPin >= 0) {
      digitalWrite(ledPins[thisPin], LOW);
    }
    
    // Move to next LED (with wraparound)
    thisPin = (thisPin + 1) % 10;
    
    // Turn on new LED
    digitalWrite(ledPins[thisPin], HIGH);
    
    // Debug output
    Serial.print("LED ");
    Serial.print(thisPin + 1);
    Serial.println(" ON");
  }
  
  lastButtonState = buttonState;
}
```

**Libraries used:**
- None (uses only built-in Arduino functions)

---

## What I Learned

**Technical skills:**
- Button edge detection (press vs hold)
- Non-blocking timing with `millis()`
- Array indexing with modulo wraparound
- Debouncing mechanical buttons
- Analog voltage monitoring

**Concepts understood:**
- **Current limiting** — How resistor value affects LED brightness
- **Ohm's Law practical application** — I = V / R in real circuit
- **Circuit analysis** — Why series resistance affects all LEDs equally
- **Blocking vs non-blocking code** — `delay()` vs `millis()`
- **Edge detection pattern** — Comparing current vs previous state

**Challenges overcome:**
- **All LEDs same brightness** → Initial resistor range too extreme
  - Solution: Narrowed range to 100Ω-2.2kΩ for visible differences
  - Lesson: Resistor selection matters for comparison experiments

- **Series 10kΩ made all LEDs dim** → Series resistor dominated circuit
  - Cause: 10kΩ >> individual LED resistors, so total R ≈ 10kΩ for all
  - Solution: Remove series resistor, use only individual LED resistors
  - Lesson: Series resistances add, dominant value controls behavior

- **Unresponsive button** → `delay(1000)` in voltage monitoring blocked loop
  - Solution: Implemented non-blocking timing with `millis()`
  - Lesson: Never use `delay()` in programs requiring responsiveness

- **Compilation errors** → Arduino IDE vs manual includes
  - Error: "identifier HIGH is undefined"
  - Cause: Unnecessary `#include <Arduino.h>` in Arduino IDE
  - Solution: Remove manual includes, IDE handles automatically
  - Lesson: Arduino IDE auto-includes core library

**Mistakes made:**
- Used blocking `delay()` initially → Poor responsiveness
- Forgot button debouncing → Double-triggering
- Shadowed variables in `setup()` → Compilation errors
- Series resistor experiment → Learned about resistance dominance

---

## Troubleshooting

### Problem: All LEDs Same Brightness
**Symptoms:** Can't see brightness differences between LEDs  
**Cause:** Resistor value range too extreme (47Ω to 100kΩ)  
**Solution:**
- Use narrower range: 100Ω to 2.2kΩ (visible differences)
- Ensure no series resistor affecting all LEDs equally
- Test individual LEDs with multimeter to verify current  
**Prevention:** Plan resistor values for visible comparison range

### Problem: Button Not Responding
**Symptoms:** Button press doesn't cycle LEDs  
**Cause:** `delay()` blocking the main loop  
**Solution:**
- Remove all `delay()` except short debounce (50ms)
- Use `millis()` for non-blocking timing
- Check Serial Monitor for debug output  
**Prevention:** Avoid `delay()` in responsive programs

### Problem: LED Cycles Multiple Times Per Press
**Symptoms:** One button press cycles through 2-3 LEDs  
**Cause:** Mechanical button bounce (switch contacts vibrate)  
**Solution:**
- Add 50ms debounce delay after edge detection
- Use edge detection (LOW→HIGH), not level detection
- Verify `lastButtonState` properly updated  
**Prevention:** Always debounce mechanical buttons

### Problem: Compilation Error "HIGH is undefined"
**Symptoms:** Code won't compile in Arduino IDE  
**Cause:** Unnecessary manual includes conflicting with IDE  
**Solution:**
- Remove `#include <Arduino.h>` line
- Let Arduino IDE auto-include libraries
- Clean and recompile  
**Prevention:** Don't manually include core Arduino libraries in IDE

---

## Session History

### Session 1: October 7, 2025
**Goal:** Create LED cycling code with button control

**Progress:**
- Developed array-based approach for 10 LED management
- Implemented edge detection for button press
- Added Serial debugging output
- Chose array over 10 boolean variables (scalability)

**Challenges:**
- Initial resistor selection too extreme (47Ω to 100kΩ)
- All LEDs appeared same brightness despite different resistors
- Series 10kΩ experiment made all LEDs equally dim

**Key insight:** Need better resistor value selection for visible comparison

---

### Session 2: October 8, 2025
**Goal:** Debug and enhance functionality

**Improvements:**
- Added voltage monitoring via analog pin A0
- Implemented non-blocking timing with `millis()`
- Fixed button debouncing (50ms delay)
- Resolved compilation errors

**Issues resolved:**
- Removed blocking `delay(1000)` from voltage function
- Fixed "HIGH is undefined" by removing manual includes
- Corrected variable shadowing in setup()
- Implemented proper edge detection

**Result:** Fully functional, responsive LED cycling with continuous voltage monitoring

---

## Improvements & Next Steps

**Possible enhancements:**
- [ ] Add LCD display to show current resistor value and brightness (medium)
- [ ] Measure actual current through each LED with INA219 sensor (advanced)
- [ ] Create brightness graph/chart over Serial plotter (easy)
- [ ] Add second button for reverse cycling (easy)
- [ ] Auto-cycle mode with adjustable speed (medium)

**What I'd do differently:**
- Plan resistor value range before building (avoid rework)
- Start with non-blocking code from beginning
- Use multimeter during testing for current measurements
- Document expected brightness for each resistor value

**Resistor optimization recommendations:**
- **Best comparison range:** 100Ω, 220Ω, 470Ω, 1kΩ, 2.2kΩ (5 LEDs)
- **Extended range:** Add 47Ω (very bright) and 4.7kΩ (very dim) for extremes
- **Avoid:** 10kΩ+ (too dim to see), below 47Ω (risky current)

---

## Related Projects

**Similar concepts:**
- [10LEDs Level Meter](../10LEDs-level/README.md) — Uses arrays, current limiting, similar LED control

**Next logical steps:**
- Multi-LED patterns (all LEDs with different brightness via PWM)
- Current measurement project (INA219 sensor)
- Battery voltage monitor with LED indicator

**Uses same concepts:**
- Any button-controlled project
- Projects requiring non-blocking timing
- Current limiting experiments

---

## Files

**Code:**
- Arduino sketch (filename not specified in summary)

**Documentation:**
- `README.md` — This file

---

## Technical Analysis

### Resistor Value Effects (Theoretical)

**Calculations for 5V Arduino, 2V LED forward voltage:**

| Resistor | Voltage Drop | Current (I=V/R) | LED Brightness | Safe? |
|----------|--------------|-----------------|----------------|-------|
| 47Ω      | 3V           | 63.8mA          | Very bright    | ⚠️ Borderline (>20mA pin limit) |
| 100Ω     | 3V           | 30mA            | Bright         | ⚠️ Exceeds 20mA pin limit |
| 220Ω     | 3V           | 13.6mA          | Good           | ✅ Safe |
| 470Ω     | 3V           | 6.4mA           | Dim            | ✅ Safe |
| 1kΩ      | 3V           | 3mA             | Very dim       | ✅ Safe |
| 2.2kΩ    | 3V           | 1.4mA           | Barely visible | ✅ Safe |
| 4.7kΩ    | 3V           | 0.6mA           | Almost off     | ✅ Safe |
| 10kΩ     | 3V           | 0.3mA           | Off            | ✅ Safe |

**Recommended safe range:** 220Ω to 2.2kΩ (visible differences, all safe)

---

## References

**Concepts documentation:**
- Current Limiting _(to be extracted to Findings/)_
- Button Debouncing _(to be extracted to Findings/)_
- Non-Blocking Code _(to be extracted to Findings/)_

**Related session logs:**
- `Logs/2025/Oct/07.10.md` — Initial development
- `Logs/2025/Oct/08.10.md` — Debugging and enhancement

---

## Findings Candidates

**Concepts worth extracting to Findings/ folder:**

1. **Current Limiting** — LED resistor selection and Ohm's Law application
   - Used in: 10LEDs-level, 10resistors (this project)
   - Fundamental concept for all LED projects

2. **Button Debouncing** — Handling mechanical switch bounce
   - Used in: This project, future button projects
   - Essential for reliable user input

3. **Non-Blocking Code** — Using `millis()` instead of `delay()`
   - Used in: This project, any responsive program
   - Critical programming pattern

4. **Edge Detection** — Detecting state transitions (LOW→HIGH)
   - Used in: Button handling, future interrupt projects
   - Important for event-driven programming

---

**Overall summary:** Successfully created LED cycling project demonstrating current limiting effects. Overcame multiple technical challenges (blocking code, button bounce, compilation errors) through systematic debugging. Enhanced with voltage monitoring and non-blocking timing. Project serves as excellent foundation for understanding current limiting, button handling, and responsive Arduino programming. Ready for future enhancements (LCD display, current measurement, auto-cycling mode).