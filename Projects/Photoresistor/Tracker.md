# Photoresistor Project — Tracker

**Status:** 🟡 PENDING — Phase 2 (Awaiting Hardware)  
**Category:** Arduino Learning  
**Added:** 19.10.25  
**Phase 1 Complete:** 20.10.25  
**Last Updated:** 26.10.25

---

## Quick Status

**Phase 1:** ✅ Complete — LED brightness control via photoresistor  
**Phase 2:** 🟡 Blocked — Awaiting DSO138Mini oscilloscope delivery  
**Phase 3:** 💭 Planned — Advanced signal analysis & findings documentation

---

## Goals & Progress

### Phase 1: Basic Light Sensing ✅
- [x] Build voltage divider circuit (photoresistor + 10kΩ)
- [x] Read analog values (0-1023 range)
- [x] Control LED brightness via PWM
- [x] Discover sensor range limitation (100-1000 vs full 0-1023)
- [x] Solve with map() function calibration
- [x] Create comprehensive guide (photoresistor-guide.md)
- [x] Document 5 code examples
- [x] Compare with Potentiometer project
- [x] Create README.md with learnings

**Completion:** 20.10.25 (~3 hours)

### Phase 2: Oscilloscope Analysis 🟡
- [ ] Assemble DSO138Mini oscilloscope (when arrives)
- [ ] Connect to Arduino PWM pin 9
- [ ] Visualize PWM signal at different duty cycles
- [ ] Measure analog voltage on A0 pin
- [ ] Analyze photoresistor response time
- [ ] Study signal noise and filtering
- [ ] Document findings with screenshots

**Blocked By:**
- 🚧 DSO138Mini oscilloscope on order (AliExpress)
- 🚧 Oscilloscope probe missing (may need to order separately)
- **Ordered:** 15.10.25
- **Expected:** TBD (2-4 weeks typical)

### Phase 3: Knowledge Extraction 💭
- [ ] Create Findings/analogRead.md (ADC concepts)
- [ ] Create Findings/PWM.md (pulse width modulation)
- [ ] Expand photoresistor-guide.md with oscilloscope data
- [ ] Cross-reference with Potentiometer project
- [ ] Consider Thermistor project (same voltage divider technique)

---

## Current Blockers

**Hardware:**
- DSO138Mini oscilloscope on order (primary blocker)
- Oscilloscope probe availability TBD

**No software/knowledge blockers** — Phase 1 complete, ready for Phase 2 when hardware arrives

---

## Technical Summary

**Circuit:**
- Photoresistor (LDR) + 10kΩ resistor → voltage divider
- Arduino pin A0 → analog input (10-bit ADC, 0-1023)
- Arduino pin 9 → PWM output (8-bit, 0-255)
- LED + 220Ω current-limiting resistor

**Key Discovery:**
- Photoresistor range: ~100-1000 (not full 0-1023!)
- Solution: `map(sensorValue, 100, 1000, 0, 255)`
- Learning: Real sensors have practical limitations, software compensates

**Code Files:**
1. `Photoresistor_Basic.ino` — Simple reading
2. `Photoresistor_Voltage.ino` — ADC to voltage
3. `Photoresistor_Threshold.ino` — Dark/bright detection
4. `Photoresistor_PWM_Basic.ino` — Limited range PWM
5. `Photoresistor_PWM_Mapped.ino` — ✅ Full range (BEST)

---

## Concepts Learned

**New Concepts:**
- Voltage divider for resistance-to-voltage conversion
- Analog input (10-bit ADC)
- PWM output for LED brightness
- Sensor calibration with map()
- Comparing sensor characteristics (photoresistor vs potentiometer)

**Reused Concepts:**
- Current limiting (from LED basics)
- pinMode/digitalWrite/analogWrite
- Serial debugging

**To Extract:**
- analogRead() → Findings/analogRead.md
- PWM theory → Findings/PWM.md

---

## Related Projects

**Similar Techniques:**
- [Potentiometer](../Potentiometer/) — Analog input, full range by design
- Thermistor (planned) — Same voltage divider, different sensor

**Future Applications:**
- Light-following robot (multiple photoresistors)
- Automatic night light (threshold + relay)
- Plant monitor (light + moisture sensors)

---

## Files & Documentation

**Project Files:**
- `README.md` — Project summary & learnings
- `photoresistor-guide.md` — Comprehensive theory & troubleshooting (1000+ lines)
- 5x `.ino` files — Progressive code examples
- Circuit images (3 failures + 1 success with schematics)

**Session Logs:**
- `Logs/2025/Oct/19.10.md` — Initial exploration
- `Logs/2025/Oct/20.10.md` — Discovery & solution

**Cross-References:**
- Findings/Voltage-Divider.md
- Findings/Current-Limiting.md

---

## Next Actions

**When oscilloscope arrives:**
1. Unbox and assemble DSO138Mini
2. Check for probe (order if missing)
3. Connect to Arduino PWM output (pin 9)
4. Visualize signal at different brightness levels
5. Measure analog voltage on A0
6. Screenshot waveforms for documentation
7. Update photoresistor-guide.md with findings
8. Create Findings/PWM.md

**After Phase 2:**
- Extract analogRead.md from learnings
- Consider moving to Completed.md
- Plan Thermistor project (reuse voltage divider knowledge)

---

## Quick Links

**Back to Index:**
- [Projects-Tracker.md](../../../Learning-Private/Mentoring/Projects-Tracker.md)

**Related:**
- [Session Logs](../../../Learning-Private/Logs/2025/Oct/)
- [Findings](../../Findings/)
- [Components](../../Components/)

---

**Location:** `/Arduino-Learning/Projects/Photoresistor/`  
**Completion:** 33% (Phase 1 done, Phase 2-3 pending)  
**Time Invested:** ~3 hours  
**Satisfaction:** 🌟🌟🌟🌟🌟 High — Elegant solution discovered!
