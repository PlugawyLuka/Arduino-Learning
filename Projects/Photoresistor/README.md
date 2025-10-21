# Photoresistor — Light Sensor Project

**Status:** ✅ Complete (Phase 1)  
**Difficulty:** Beginner-Intermediate  
**Completion Date:** 19-20.10.25

---

## Quick Overview

**What it does:** Reads light levels and controls LED brightness accordingly

**Key achievement:** Discovered and solved sensor range limitation using `map()` function

**Hardware:**
- Arduino Uno
- Photoresistor (LDR)
- 10kΩ resistor (voltage divider)
- LED + 220Ω resistor

---

## What I Learned

### **Core Concepts:**
✅ Voltage divider for sensor reading  
✅ Analog input (analogRead)  
✅ PWM output (analogWrite)  
✅ Sensor calibration with map()  
✅ Sensor characteristic differences

### **Key Discovery:**
**Problem:** LED wouldn't reach full OFF or full brightness  
**Cause:** Photoresistor uses limited resistance range (100-1000) vs potentiometer's full range (0-1023)  
**Solution:** Software mapping with `map()` function stretches actual range to full LED control

**This taught me:** Real-world sensors have practical limitations that software can compensate for!

---

## Project Files

**Code examples (5 total):**
1. `Photoresistor_Basic.ino` — Simple sensor reading
2. `Photoresistor_Voltage.ino` — ADC to voltage conversion
3. `Photoresistor_Threshold.ino` — Dark/bright detection
4. `Photoresistor_PWM_Basic.ino` — Basic PWM control (limited range)
5. `Photoresistor_PWM_Mapped.ino` — **Full range PWM ✅ BEST VERSION**

**Documentation:**
- `photoresistor-guide.md` — Comprehensive guide with theory, circuits, and troubleshooting
- `README.md` — This file

---

## Circuit Diagram

```
Voltage Divider:
       5V
        │
    Photoresistor (R_ldr)
        │
        ├─── A0 (analog input)
        │
      10kΩ (R_fixed)
        │
       GND

LED Indicator (optional):
    Pin 9 (PWM) → 220Ω → LED → GND
```

---

## Best Code Version

**Full Range PWM with Calibration:**

```cpp
const int sensorPin = A0;
const int ledPin = 9;
const int sensorMin = 100;   // Calibrated for my environment
const int sensorMax = 1000;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  int brightness = map(sensorValue, sensorMin, sensorMax, 0, 255);
  brightness = constrain(brightness, 0, 255);
  analogWrite(ledPin, brightness);
  delay(100);
}
```

**Result:** LED smoothly transitions from completely OFF to maximum brightness! ✅

---

## Concepts Used

**From Findings:**
- [Voltage Dividers](../Findings/Voltage-Divider.md) — Converting resistance to voltage
- [Current Limiting](../Findings/Current-Limiting.md) — LED protection

**New concepts:**
- Analog input (10-bit ADC, 0-1023 range)
- PWM output (8-bit, 0-255 range)
- `map()` function for range conversion
- Sensor calibration techniques
- Comparing sensor characteristics

---

## Phase 2 (Planned)

**Awaiting:** DSO138Mini Digital Oscilloscope from AliExpress

**Next experiments:**
- Visualize PWM signals with oscilloscope
- Analyze analog voltage waveforms
- Measure photoresistor response time
- Study signal noise and filtering
- Create PWM Basics Finding
- Expand guide to full version

---

## Challenges & Solutions

**Challenge 1: LED limited range**
- **Problem:** LED brightness never reached 0% or 100%
- **Root cause:** Photoresistor only uses 100-1000 range (not full 0-1023)
- **Solution:** `map(sensorValue, 100, 1000, 0, 255)` stretches to full range
- **Learning:** Software can compensate for hardware limitations!

**Challenge 2: Understanding sensor differences**
- **Problem:** Why does potentiometer work better than photoresistor?
- **Investigation:** Compared resistance ranges of both sensors
- **Discovery:** Potentiometer designed for full 0-10kΩ sweep, photoresistor limited by physics
- **Learning:** Different sensors have different characteristic ranges

---

## Related Projects

**Uses same concepts:**
- Potentiometer (analog input, PWM output, but full range by design)
- Thermistor (will use same voltage divider technique)
- Soil moisture sensor (analog input reading)

**Future projects:**
- Light-following robot (multiple photoresistors)
- Automatic night light (threshold detection + relay)
- Plant monitor (light + moisture sensors)

---

## Quick Stats

**Time spent:** ~3 hours (including discovery and documentation)  
**Code examples:** 5  
**Key functions learned:** analogRead(), analogWrite(), map(), constrain()  
**Lines of documentation:** ~1000+  
**Satisfaction level:** High — Solved real problem with elegant solution! 🌟

---

## Files Structure

```
Luka/Arduino/Photoresistor/
├── README.md                          ← This file
├── photoresistor-guide.md             ← Comprehensive guide
├── Photoresistor_Basic.ino
├── Photoresistor_Voltage.ino
├── Photoresistor_Threshold.ino
├── Photoresistor_PWM_Basic.ino
└── Photoresistor_PWM_Mapped.ino       ← Best version ✅
```

---

## Key Takeaway

> **"Real-world sensors don't behave like ideal components in textbooks. Software calibration (map function) bridges the gap between theory and practice."**

This project taught me to:
- 🔍 Observe unexpected behavior carefully
- 🤔 Form and test hypotheses
- 🔬 Compare components to understand differences
- 💡 Find elegant software solutions to hardware limitations
- 📝 Document discoveries for future reference

**Status:** Ready for Phase 2 when oscilloscope arrives! 🚀

---

**Created:** 19-20.10.25  
**Last Updated:** 20.10.25  
**Phase:** 1 Complete, Phase 2 Planned