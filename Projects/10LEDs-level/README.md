# LED Level Meter with Potentiometer

**Created:** 19.10.25  
**Status:** Completed ✓  
**Difficulty:** Beginner

---

## Description

A visual level meter using 11 LEDs controlled by a potentiometer. As you turn the potentiometer knob, LEDs light up progressively to show the analog input value.

**Key features:**
- Real-time visual feedback of analog input
- Smooth level progression across 11 LEDs
- Serial monitor debugging output
- Proper current limiting for all LEDs

---

## Components

**Required:**
- [x] Arduino Uno
- [x] 10kΩ potentiometer (3-pin)
- [x] 11 LEDs (any color)
- [x] 11× 220Ω resistors (current limiting)
- [x] Breadboard
- [x] Jumper wires

**Total cost:** ~$5-10

---

## Circuit

**Wiring:**
```
Potentiometer:
  Pin 1 → 5V
  Pin 2 (wiper) → A0
  Pin 3 → GND

LEDs (all identical):
  Arduino Pin 2  → 220Ω → LED (anode) → GND
  Arduino Pin 3  → 220Ω → LED (anode) → GND
  ...
  Arduino Pin 12 → 220Ω → LED (anode) → GND
```

**Important notes:**
- ⚠️ **Each LED needs its own 220Ω resistor** (prevents overcurrent)
- 💡 Arrange LEDs in a line for visual "level meter" effect
- ⚠️ LED polarity matters: long leg (anode) to resistor, short leg to GND

---

## How to Use

**Setup:**
1. Upload code to Arduino
2. Open Serial Monitor (9600 baud)
3. Turn potentiometer knob

**Operation:**
1. Turn potentiometer fully left → 1 LED lit
2. Turn potentiometer middle → 5-6 LEDs lit
3. Turn potentiometer fully right → All 11 LEDs lit

**Expected behavior:**
- LEDs light up progressively (like audio VU meter)
- Serial monitor shows analog reading (0-1023) and LED count (1-11)

---

## Concepts Used

This project demonstrates these fundamental concepts:

### Current Limiting _(to be extracted to Findings/)_
**How it's used here:** Each LED has 220Ω resistor in series  
**Why it matters:** Protects LEDs and Arduino pins from overcurrent damage  
**Discovery:** Pin 12 was drawing 29.4mA without resistor (exceeded 20mA safe limit). Adding 220Ω resistor reduced current to 13.6mA (safe). Learned that resistor value matters: 47Ω dangerous, 220Ω ideal, 2.2kΩ+ too dim.  
**See session:** 19.10.25 troubleshooting notes

### Arrays in Arduino
**How it's used here:** `int ledPins[]` array stores all 11 pin numbers  
**Why it matters:** Simplifies code - can loop through pins instead of 11 separate variables  
**Example:**
```cpp
int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};  // All pins in array
for (int i = 0; i < 11; i++) {
  pinMode(ledPins[i], OUTPUT);  // Loop instead of 11 lines
}
```

### Analog Input (analogRead)
**How it's used here:** Reading potentiometer position (0-1023)  
**Why it matters:** Foundation for all analog sensors (light, temperature, pressure)  
**Example:**
```cpp
int sensorValue = analogRead(A0);  // Returns 0-1023
```

### Map Function
**How it's used here:** Converting 0-1023 range to 1-11 LED count  
**Why it matters:** Scales any range to any other range - super useful!  
**Example:**
```cpp
int ledLevel = map(sensorValue, 0, 1023, 1, 11);  // 0-1023 → 1-11
```

---

## Code Highlights

**Key functions:**
```cpp
void setup() {
  // Initialize all LED pins as outputs using array
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    pinMode(ledPins[thisLed], OUTPUT);
  }
}

void loop() {
  // Read sensor, map to LED count, light appropriate LEDs
  int sensorValue = analogRead(A0);
  int ledLevel = map(sensorValue, 0, 1023, 1, 11);
  
  // Turn on LEDs up to level, turn off rest
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    if (thisLed < ledLevel) {
      digitalWrite(ledPins[thisLed], HIGH);  // On
    } else {
      digitalWrite(ledPins[thisLed], LOW);   // Off
    }
  }
}
```

**Libraries used:**
- None (uses only built-in Arduino functions)

---

## What I Learned

**Technical skills:**
- Using arrays to manage multiple pins efficiently
- Reading analog input (potentiometer)
- Controlling multiple LEDs simultaneously
- Calculating resistor values for LED current limiting

**Concepts understood:**
- **Current limiting** — Why and how to use resistors with LEDs
- **Resistor selection** — 220Ω optimal for 5V Arduino with standard LEDs
- **Ohm's Law application** — Calculating current: I = V / R
- **Analog vs Digital** — analogRead() returns 0-1023, not just HIGH/LOW

**Challenges overcome:**
- **Pin 12 flickering** → Missing resistor causing overcurrent (29.4mA)
  - Solution: Added 220Ω resistor (reduced to 13.6mA)
  - Lesson: Every LED needs current limiting!
- **Tinkercad warnings** → Showed pin current exceeded safe limits
  - Solution: Resistor value calculation
  - Lesson: Simulators catch issues before hardware damage

**Mistakes made:**
- Initially forgot resistors on some LEDs → Flickering and warnings
- Tried 47Ω resistor → Still too much current
- Tried 2.2kΩ resistor → LEDs too dim
- Found sweet spot: 220Ω (13.6mA, perfect brightness)

---

## Troubleshooting

### Problem: LED Flickering or Dim
**Symptoms:** LED doesn't light consistently or appears very dim  
**Cause:** Missing or wrong-value resistor  
**Solution:**
- Check resistor is present (220Ω for 5V)
- Verify resistor is in series with LED, not parallel
- Test resistor with multimeter (should read ~220Ω)  
**Prevention:** Always add resistor before connecting LED

### Problem: All LEDs Stay On/Off
**Symptoms:** LEDs don't respond to potentiometer  
**Cause:** Potentiometer wiring incorrect  
**Solution:**
- Verify potentiometer: 5V on pin 1, GND on pin 3, A0 on pin 2 (wiper)
- Test with Serial Monitor: does reading change when turning knob?  
**Prevention:** Test potentiometer alone before adding LEDs

### Problem: LEDs Light in Wrong Order
**Symptoms:** Random LEDs light instead of sequential  
**Cause:** Array order doesn't match physical pin connections  
**Solution:**
- Verify `ledPins[]` array matches physical wiring
- Check pin numbers in code match Arduino connections  
**Prevention:** Document pin connections before wiring

---

## Improvements & Next Steps

**Possible enhancements:**
- [ ] Add button to change LED animation pattern (medium)
- [ ] Use RGB LEDs for color-changing level meter (medium)
- [ ] Add sound output (buzzer pitch changes with level) (easy)
- [ ] Replace potentiometer with photoresistor (light-responsive) (easy) ← **Next project!**

**What I'd do differently:**
- Plan resistor placement before wiring (avoid rework)
- Use color-coded wires (easier troubleshooting)
- Document pin connections in comments (faster debugging)

**Future project ideas:**
- Audio VU meter (microphone input)
- Temperature bar graph (thermistor)
- Battery level indicator (voltage divider)

---

## Related Projects

**Next logical step:**
- [Photoresistor Light Meter](../Photoresistor/README.md) — Replaces potentiometer with light sensor, uses voltage dividers

**Uses same concepts:**
- Any multi-LED project
- Any analog input project

---

## Files

**Code:**
- `10LEDs-level.ino` — Main Arduino sketch

**Documentation:**
- `level-guide.md` — Detailed explanation of project (includes resistor selection)
- Circuit diagram (Tinkercad) — Visual wiring reference

---

## Session History

**Created:** 19.10.25 (Session log: `Logs/2025/Oct/19.10.md`)  
**Completed:** 19.10.25 (same session)  

**Findings mentioned in this project:**
- Current Limiting — Needs to be extracted to Findings/ folder
- Resistor selection for LEDs — Part of current limiting concept

---

## References

**Calculations:**
- LED forward voltage: ~2V (standard red LED)
- Arduino pin voltage: 5V
- Voltage across resistor: 5V - 2V = 3V
- Target current: ~13-15mA (safe for LED and Arduino)
- Resistor value: R = V / I = 3V / 0.014A ≈ 214Ω → Use 220Ω (standard value)

**Related Findings:**
- Current Limiting _(to be created based on this project)_

---

## Additional Documentation

**See also:** `level-guide.md` for comprehensive project guide with:
- Detailed resistor selection process
- Troubleshooting steps
- Code explanation
- Learning outcomes

---

**Notes:**
- This project serves as foundation for all analog input projects
- Resistor selection process documented for future LED projects
- Good candidate for extracting "Current Limiting" Finding guide
- Successfully completed in single session with troubleshooting