# Photoresistor Light Meter

**Created:** [To be filled on project start]  
**Status:** In Progress 🚧  
**Difficulty:** Beginner-Intermediate

---

## Description

A light-responsive LED level meter that uses a photoresistor (LDR) to detect ambient light and displays the light level using 10 LEDs. This project builds on the [10LEDs-level](../10LEDs-level/README.md) project by replacing the potentiometer with a light sensor.

**Key features:**
- Photoresistor-based light detection
- Voltage divider circuit for sensor reading
- Real-time LED bar graph display
- Serial Monitor debugging output
- Calibration for different lighting conditions

**Purpose:** Understand how resistive sensors work with Arduino and master voltage divider circuits.

---

## Components

**Required:**
- [ ] Arduino Uno
- [ ] Photoresistor LDR 5539 (or similar)
- [ ] 10kΩ resistor (voltage divider)
- [ ] 10 LEDs (same color for bar graph)
- [ ] 10× 220Ω resistors (LED current limiting)
- [ ] Breadboard
- [ ] Jumper wires

**Optional:**
- [ ] Flashlight (for testing bright condition)
- [ ] Multimeter (for measuring resistance)

---

## Circuit

**Photoresistor Voltage Divider:**
```
5V → Photoresistor (LDR) → A0 → 10kΩ → GND
```

**LED Bar Graph:**
```
Pin 2  → 220Ω → LED → GND
Pin 3  → 220Ω → LED → GND
Pin 4  → 220Ω → LED → GND
Pin 5  → 220Ω → LED → GND
Pin 6  → 220Ω → LED → GND
Pin 7  → 220Ω → LED → GND
Pin 8  → 220Ω → LED → GND
Pin 9  → 220Ω → LED → GND
Pin 10 → 220Ω → LED → GND
Pin 11 → 220Ω → LED → GND
```

**Important notes:**
- ⚠️ **Must use 10kΩ resistor in voltage divider** - See [Voltage Dividers Finding](../Findings/Voltage-Divider.md)
- 💡 All LEDs need 220Ω current limiting resistors - See [Current Limiting Finding](../Findings/Current-Limiting.md)
- 📍 Photoresistor polarity doesn't matter (non-polarized component)

---

## How to Use

**Setup:**
1. Build voltage divider circuit (5V → LDR → A0 → 10kΩ → GND)
2. Connect LED bar graph with resistors
3. Upload code to Arduino
4. Open Serial Monitor (9600 baud)

**Operation:**
1. Cover photoresistor → Fewer LEDs light up
2. Shine flashlight → More LEDs light up
3. Serial Monitor shows:
   - Raw sensor value (0-1023)
   - Number of LEDs to light
   - Approximate light level description

**Expected behavior:**
- Bright light → 8-10 LEDs on, reading 700-1000
- Room light → 4-6 LEDs on, reading 300-600
- Darkness → 1-3 LEDs on, reading 50-200

---

## Concepts Used

This project demonstrates these fundamental concepts:

### [Voltage Dividers](../Findings/Voltage-Divider.md) ⭐
**How it's used here:** Converting photoresistor resistance (1kΩ-100kΩ) to voltage (0-5V)  
**Why it matters:** Arduino can't measure resistance directly, only voltage  
**Circuit breakdown:**
```
5V → LDR (variable R1) → A0 → 10kΩ (fixed R2) → GND

Bright: LDR = 1kΩ  → V_out = 4.5V → Arduino reads ~920
Dark:   LDR = 100kΩ → V_out = 0.45V → Arduino reads ~92
```
**Without 10kΩ resistor:** Circuit doesn't work! (No voltage reference)  
**See:** [Voltage Dividers Finding](../Findings/Voltage-Divider.md) for complete theory

### [Current Limiting](../Findings/Current-Limiting.md)
**How it's used here:** 220Ω resistors protect each LED  
**Why it matters:** Same as 10LEDs-level project - prevents damage  
**See:** [Current Limiting Finding](../Findings/Current-Limiting.md)

### Analog Input (analogRead)
**How it's used here:** Reading voltage divider output at pin A0  
**Why it matters:** Foundation for all analog sensors  
**Example:**
```cpp
int lightLevel = analogRead(A0);  // 0-1023
// Higher value = brighter light (with standard voltage divider)
```

### Map Function
**How it's used here:** Convert 0-1023 sensor range to 0-10 LED count  
**Why it matters:** Scales sensor reading to meaningful output  
**Example:**
```cpp
int ledCount = map(lightLevel, 0, 1023, 0, 10);
```

---

## Code Highlights

**Key setup:**
```cpp
const int photoresistorPin = A0;
const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

void setup() {
  for (int i = 0; i < 10; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.begin(9600);
}
```

**Main loop:**
```cpp
void loop() {
  int lightLevel = analogRead(photoresistorPin);
  int ledCount = map(lightLevel, 0, 1023, 0, 10);
  
  // Update LED bar graph
  for (int i = 0; i < 10; i++) {
    digitalWrite(ledPins[i], i < ledCount ? HIGH : LOW);
  }
  
  delay(10);
}
```

---

## What I Learned

**Technical skills:**
- Building voltage divider circuits
- Reading resistive sensors
- Calibrating sensor thresholds
- Adapting existing project (potentiometer → photoresistor)

**Concepts understood:**
- **[Voltage Dividers](../Findings/Voltage-Divider.md)** — Why sensors need fixed resistor
- **Resistor selection** — Why 10kΩ works for this LDR range
- **Sensor characteristics** — LDR resistance vs light level relationship

**Challenges overcome:**
- [To be filled during project]

---

## Troubleshooting

### Problem: Readings Don't Change with Light
**Symptoms:** analogRead always returns ~0 or ~1023  
**Cause:** Missing 10kΩ resistor in voltage divider  
**Solution:** Add 10kΩ from A0 to GND (see [Voltage Dividers](../Findings/Voltage-Divider.md))  
**Prevention:** Always verify voltage divider circuit before powering on

### Problem: LEDs Don't Respond to Light Changes
**Symptoms:** LEDs static or flickering  
**Cause:** Incorrect map() range or inverted logic  
**Solution:**
- Check Serial Monitor for actual sensor values
- Adjust map() parameters based on your room lighting
- May need to invert: `map(lightLevel, 0, 1023, 10, 0)` for darker=more LEDs  
**Prevention:** Calibrate with flashlight and dark cover

### Problem: Readings Unstable/Noisy
**Symptoms:** Serial Monitor shows jumping values  
**Cause:** Electrical noise or floating input  
**Solution:**
- Verify voltage divider wiring
- Add smoothing/averaging in code
- Check breadboard connections  
**Prevention:** Keep sensor wires short and away from power lines

---

## Improvements & Next Steps

**Possible enhancements:**
- [ ] Add calibration mode (auto-adjust thresholds)
- [ ] Smooth readings with moving average
- [ ] Add buzzer for darkness alarm
- [ ] Reverse logic (more LEDs in darkness)
- [ ] Add threshold adjustment with potentiometer

**What I'd do differently:**
- [To be filled after completion]

**Future project ideas:**
- Thermistor temperature meter (same voltage divider concept)
- FSR pressure sensor
- Combined light+temperature display

---

## Related Projects

**Built from:**
- [10LEDs Level Meter](../10LEDs-level/README.md) — Same LED control, different input

**Uses same concepts:**
- [Potentiometer](../Potentiometer/README.md) — Analog input basics
- Any resistive sensor project

**Next logical steps:**
- Thermistor temperature sensor
- FSR pressure-sensitive pad
- Multi-sensor dashboard

---

## Files

**Code:**
- `Photoresistor.ino` — Main Arduino sketch

**Documentation:**
- `README.md` — This file
- `photoresistor-guide.md` — Detailed guide (if created)

---

## Session History

**Created:** [Date to be filled]  
**Status:** In Progress  

**Key milestones:**
- [To be filled during development]

---

## References

**Concepts documentation:**
- [Voltage Dividers](../Findings/Voltage-Divider.md) — Complete theory and examples
- [Current Limiting](../Findings/Current-Limiting.md) — LED resistor calculation

**Related session logs:**
- [`Logs/2025/Oct/20.10.md`](../../../Luka-Private/Logs/2025/Oct/20.10.md) — Voltage divider discovery

**Arduino documentation:**
- `analogRead()` — https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
- Photoresistor tutorial — https://learn.adafruit.com/photocells

**Component datasheets:**
- LDR 5539 specifications (if available)

---

## Findings Used

This project directly applies these Findings:

1. **[Voltage Dividers](../Findings/Voltage-Divider.md)** ⭐ — Core concept for this project
2. **[Current Limiting](../Findings/Current-Limiting.md)** — LED protection
3. **Analog Input** _(to be extracted)_ — Reading sensor values

---

**Note:** This README follows the standard project template and cross-references all relevant Findings for easy learning and future reference.