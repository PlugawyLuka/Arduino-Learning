# 4x4 Keypad Matrix - Project Tracker

**Status:** 🟡 PENDING — Awaiting hardware delivery  
**Category:** Arduino  
**Added:** 20.10.25  
**Last Updated:** 26.10.25

---

## Quick Status

**Current Phase:** Phase 0 — Hardware acquisition  
**Blocker:** 4x4 keypad hardware on order  
**Next Action:** Unbox and test keypad when delivered  

---

## Project Overview

**Goal:** Learn matrix scanning technique to read 16 buttons using only 8 Arduino pins.

**Key Concept:** Matrix scanning (row/column addressing) — same technique used in keyboards, calculators, and keypad systems worldwide.

**Hardware:**
- 4x4 matrix keypad (16 buttons: 0-9, A-D, *, #)
- Arduino Uno
- 8 digital pins (4 rows + 4 columns)

---

## Goals & Progress

### Phase 0: Hardware Acquisition 🟡
- ⏳ Order 4x4 keypad online
- ⏳ Wait for delivery
- ⏳ Unbox and inspect hardware

### Phase 1: Basic Matrix Scanning (Not Started)
- ⏳ Understand row/column matrix concept
- ⏳ Wire 4x4 keypad to Arduino (8 pins total)
- ⏳ Implement basic scanning algorithm (row HIGH, read columns)
- ⏳ Serial monitor output: Display which key is pressed
- ⏳ Test all 16 keys for correct detection

**Learning Goals:**
- Digital I/O pin switching (OUTPUT → INPUT)
- Pull-up/pull-down resistor concepts
- Debouncing for keypad input
- Matrix addressing (row × column = button ID)

### Phase 2: Practical Applications (Future)
- ⏳ Build simple calculator (0-9, +, -, *, =)
- ⏳ Create PIN code entry system (security application)
- ⏳ Menu navigation system (A/B/C/D = menu options)
- ⏳ Password door lock project

**Advanced Goals:**
- Multi-key detection (can keypad detect 2+ keys simultaneously?)
- Long press vs short press differentiation
- Custom key mapping (assign functions to each key)

---

## Current Blockers

1. **HARDWARE:** 4x4 keypad on order, not yet delivered
   - **Impact:** Cannot start Phase 1 until hardware arrives
   - **Resolution:** Wait for delivery, track package

---

## Technical Summary

### Matrix Scanning Principle
A 4x4 matrix keypad has 16 buttons but only needs 8 wires (4 rows + 4 columns):

```
    C1  C2  C3  C4
R1  1   2   3   A
R2  4   5   6   B
R3  7   8   9   C
R4  *   0   #   D
```

**Scanning Algorithm:**
1. Set all row pins to OUTPUT LOW
2. Set one row HIGH (e.g., R1)
3. Read all column pins (INPUT with pull-down)
4. If column reads HIGH → button at (row, column) is pressed
5. Repeat for R2, R3, R4
6. Complete scan ~1000 times/second

**Why This Works:**
- Each button connects one row to one column
- When button pressed: current flows from row → button → column
- By scanning rows sequentially, we detect which (row, column) pair is active
- 16 buttons × 1 scan = 8 pin reads (very efficient!)

---

## Concepts to Learn

**Before Starting:**
- Digital I/O basics (pinMode OUTPUT/INPUT)
- Pull-up resistor concept (avoid floating pins)
- Serial communication (display key presses)

**During Project:**
- Matrix addressing (2D array concept)
- Pin multiplexing (switching pin modes dynamically)
- Debouncing (clean button press detection)
- Scanning loops (efficient polling)

**After Completion:**
- State machines (track input sequences)
- Event-driven programming (key press events)
- User interface design (feedback, timeouts)

---

## Related Projects

- **10LEDs Level Indicator:** Digital output basics
- **Button Debouncing:** Clean input detection
- **Photoresistor:** Analog input (different from keypad's digital)
- **Future: PIN Code Lock:** Security application using keypad

---

## Files & Documentation

### Project Files (Future)
- `keypad-test.ino` — Basic matrix scanning test
- `keypad-calculator.ino` — Simple calculator application
- `keypad-pincode.ino` — PIN entry system
- `README.md` — Project documentation

### Reference Materials
- Arduino Keypad library (optional, for later)
- Matrix scanning algorithm (custom implementation first)
- Debouncing techniques (from previous projects)

### Findings to Create
- **Matrix-Scanning.md** — Explain row/column addressing concept
- **Keypad-Applications.md** — Real-world uses (security, menus, calculators)
- Update **Debouncing.md** with keypad-specific techniques

---

## Next Actions

**Immediate (Phase 0):**
1. Track package delivery
2. Unbox keypad when arrived
3. Inspect hardware (check for 8 pins: 4 row + 4 column)
4. Research basic matrix scanning algorithm

**When Hardware Arrives (Phase 1):**
1. Wire keypad to Arduino (8 digital pins)
2. Write basic scanning code (no library)
3. Test all 16 keys in Serial Monitor
4. Implement debouncing
5. Document learning in Finding files

**Future (Phase 2):**
1. Build calculator application
2. Create PIN code entry system
3. Explore multi-key detection
4. Consider LCD display integration

---

## Session Logs

### 20.10.25 - Project Added
- Added to Projects-Tracker.md
- Status: PENDING (hardware on order)
- Planned as learning project for matrix scanning

### 26.10.25 - Tracker.md Created
- Migrated to neural network architecture
- Created dedicated Tracker.md file
- Documented matrix scanning principle
- Planned 2-phase approach (basic → applications)

---

## Quick Links

- **Projects-Tracker:** `/Learning-Private/Mentoring/Projects-Tracker.md`
- **Completed Projects:** `/Learning-Private/Mentoring/Completed.md`
- **Session Logs:** `/Learning-Private/Logs/2025/Oct/`
- **Findings:** `/Arduino-Learning/Findings/`

---

**Created:** 26.10.25  
**Format:** Neural Network Architecture (Tracker.md)  
**Version:** 1.0
