# Edge Detection — Detecting State Changes

**Concept:** Triggering actions on state transitions (LOW→HIGH or HIGH→LOW)  
**Category:** Programming Pattern  
**Difficulty:** Beginner-Intermediate  
**Created:** 20.10.25  
**Last Updated:** 20.10.25

---

## What Is Edge Detection?

**Edge detection** is a programming technique that detects when a signal changes state (transitions), rather than just checking if it's currently HIGH or LOW.

**Simple analogy:**
- **Level detection:** "Is the light on?" (checks current state)
- **Edge detection:** "Did the light just turn on?" (detects change)

**Types of edges:**
- **Rising edge:** Transition from LOW to HIGH (0→1)
- **Falling edge:** Transition from HIGH to LOW (1→0)
- **Any edge:** Either transition (change detected)

**Why it matters:**
- ✅ Trigger action once per button press (not continuously while held)
- ✅ Count events (button presses, pulses, revolutions)
- ✅ Detect state changes in sensors
- ✅ Create responsive user interfaces
- ✅ Implement toggles and mode switches

---

## The Problem: Level Detection vs Edge Detection

### **Level Detection (Continuous)**

```cpp
void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == HIGH) {
    Serial.println("Button is pressed");
    // This prints CONTINUOUSLY while button held!
  }
}
```

**Output when button held for 1 second:**
```
Button is pressed
Button is pressed
Button is pressed
Button is pressed
... (hundreds of times)
```

**Problems:**
- ❌ Action repeats while button held
- ❌ Can't distinguish press from hold
- ❌ Difficult to count button presses
- ❌ Leads to unintended repeated actions

---

### **Edge Detection (Change-Based)**

```cpp
int buttonState = 0;
int lastButtonState = 0;

void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  
  // Detect rising edge (button just pressed)
  if (buttonState == HIGH && lastButtonState == LOW) {
    Serial.println("Button was just pressed");
    // This prints ONCE per press!
  }
  
  lastButtonState = buttonState;  // Remember state for next loop
}
```

**Output when button held for 1 second:**
```
Button was just pressed
(nothing more until button released and pressed again)
```

**Benefits:**
- ✅ Action triggers once per press
- ✅ Can count individual presses
- ✅ Responsive and predictable
- ✅ Easy to implement toggles

---

## How Edge Detection Works

### **The Pattern**

**Key components:**
1. **Current state** — What's happening NOW
2. **Previous state** — What happened LAST loop
3. **Comparison** — Did it change?
4. **State update** — Remember current for next time

**Visual representation:**
```
Loop 1: buttonState = LOW,  lastButtonState = LOW  → No edge (both LOW)
Loop 2: buttonState = LOW,  lastButtonState = LOW  → No edge (no change)
Loop 3: buttonState = HIGH, lastButtonState = LOW  → Rising edge! ✓
Loop 4: buttonState = HIGH, lastButtonState = HIGH → No edge (both HIGH)
Loop 5: buttonState = HIGH, lastButtonState = HIGH → No edge (still HIGH)
Loop 6: buttonState = LOW,  lastButtonState = HIGH → Falling edge! ✓
Loop 7: buttonState = LOW,  lastButtonState = LOW  → No edge (both LOW)
```

---

### **Rising Edge Detection**

**Detects:** Button just pressed (LOW→HIGH transition)

```cpp
int buttonState = 0;
int lastButtonState = 0;

void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  
  // Check for LOW → HIGH transition
  if (buttonState == HIGH && lastButtonState == LOW) {
    // Rising edge detected!
    Serial.println("Button pressed");
    doAction();
  }
  
  lastButtonState = buttonState;  // Update for next loop
}
```

**Use cases:**
- Button press detection
- Start of event
- Counting pulses
- Triggering one-time actions

---

### **Falling Edge Detection**

**Detects:** Button just released (HIGH→LOW transition)

```cpp
int buttonState = 0;
int lastButtonState = 0;

void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  
  // Check for HIGH → LOW transition
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Falling edge detected!
    Serial.println("Button released");
    stopAction();
  }
  
  lastButtonState = buttonState;
}
```

**Use cases:**
- Button release actions
- End of event
- Measuring pulse width (with rising edge)
- Debounce validation

---

### **Any Edge Detection**

**Detects:** Any change (either direction)

```cpp
int buttonState = 0;
int lastButtonState = 0;

void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  
  // Check if state changed (any direction)
  if (buttonState != lastButtonState) {
    // Edge detected (rising or falling)
    Serial.print("State changed to: ");
    Serial.println(buttonState);
    
    // Optional: distinguish direction
    if (buttonState == HIGH) {
      Serial.println("  → Rising edge");
    } else {
      Serial.println("  → Falling edge");
    }
  }
  
  lastButtonState = buttonState;
}
```

**Use cases:**
- Toggle switches
- State change monitoring
- Motion detection
- General change detection

---

## Real-World Example: 10resistors Project

### **The Implementation**

```cpp
// Global variables
int buttonState = 0;
int lastButtonState = 0;
int thisPin = -1;  // Start at -1 so first press goes to pin 0
int ledPins[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

void loop() {
  buttonState = digitalRead(buttonPin);
  
  // Rising edge detection: button just pressed
  if (buttonState == HIGH && lastButtonState == LOW) {
    delay(50);  // Debounce delay
    
    // Turn off current LED (if any)
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
  
  lastButtonState = buttonState;  // Remember for next loop
  
  // Non-blocking voltage monitoring continues...
}
```

**Why edge detection was essential:**
- ✅ One button press → One LED change
- ✅ Holding button doesn't cycle rapidly
- ✅ Predictable, controllable behavior
- ✅ Easy to count which LED is active

**Without edge detection:**
```cpp
// BAD: Level detection version
if (digitalRead(buttonPin) == HIGH) {
  cycleLED();  // Would cycle CONSTANTLY while button held!
}
```

---

## Common Patterns & Applications

### **Pattern 1: Toggle (On/Off Switch)**

```cpp
int buttonState = 0;
int lastButtonState = 0;
bool ledOn = false;

void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == HIGH && lastButtonState == LOW) {
    delay(50);  // Debounce
    
    ledOn = !ledOn;  // Toggle state
    digitalWrite(LED_PIN, ledOn);
    
    Serial.println(ledOn ? "LED ON" : "LED OFF");
  }
  
  lastButtonState = buttonState;
}
```

**Use cases:**
- Power on/off
- Mode switching
- Feature enable/disable

---

### **Pattern 2: Counter (Event Counting)**

```cpp
int buttonState = 0;
int lastButtonState = 0;
int pressCount = 0;

void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == HIGH && lastButtonState == LOW) {
    delay(50);  // Debounce
    
    pressCount++;
    Serial.print("Button pressed ");
    Serial.print(pressCount);
    Serial.println(" times");
  }
  
  lastButtonState = buttonState;
}
```

**Use cases:**
- Lap counter
- Click counter
- Event logging
- User input tracking

---

### **Pattern 3: Mode Cycling**

```cpp
int buttonState = 0;
int lastButtonState = 0;
int currentMode = 0;
const int NUM_MODES = 3;

void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == HIGH && lastButtonState == LOW) {
    delay(50);  // Debounce
    
    currentMode = (currentMode + 1) % NUM_MODES;
    
    Serial.print("Mode changed to: ");
    Serial.println(currentMode);
    
    // Execute mode-specific behavior
    switch (currentMode) {
      case 0:
        modeA();
        break;
      case 1:
        modeB();
        break;
      case 2:
        modeC();
        break;
    }
  }
  
  lastButtonState = buttonState;
}
```

**Use cases:**
- Display modes (time/date/temperature)
- Operation modes (auto/manual)
- Brightness levels
- Speed settings

---

### **Pattern 4: Pulse Measurement**

```cpp
int signalState = 0;
int lastSignalState = 0;
unsigned long pulseStart = 0;
unsigned long pulseWidth = 0;

void loop() {
  signalState = digitalRead(SIGNAL_PIN);
  
  // Rising edge: start timing
  if (signalState == HIGH && lastSignalState == LOW) {
    pulseStart = millis();
  }
  
  // Falling edge: calculate width
  if (signalState == LOW && lastSignalState == HIGH) {
    pulseWidth = millis() - pulseStart;
    Serial.print("Pulse width: ");
    Serial.print(pulseWidth);
    Serial.println("ms");
  }
  
  lastSignalState = signalState;
}
```

**Use cases:**
- PWM signal analysis
- RPM calculation (wheel rotation)
- Communication protocols
- Timing measurements

---

## Edge Detection with Debouncing

### **Combined Pattern**

Mechanical buttons "bounce" (rapidly switch on/off when pressed). Edge detection + debouncing = reliable button handling.

```cpp
int buttonState = 0;
int lastButtonState = 0;

void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == HIGH && lastButtonState == LOW) {
    delay(50);  // Debounce delay (wait for bounce to settle)
    
    // Re-read button state after debounce
    buttonState = digitalRead(BUTTON_PIN);
    
    // Verify button is still pressed (wasn't just noise)
    if (buttonState == HIGH) {
      // Confirmed rising edge
      handleButtonPress();
    }
  }
  
  lastButtonState = buttonState;
}
```

**See also:** [Button Debouncing](ButtonDebouncing.md) Finding for more detail

---

## Common Mistakes & Solutions

### ❌ **Mistake 1: Not Updating lastButtonState**

**Wrong:**
```cpp
void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == HIGH && lastButtonState == LOW) {
    doAction();
    // ← Forgot to update lastButtonState!
  }
}
```

**Problem:** lastButtonState stays LOW forever, so edge triggers every loop when button is HIGH

**Correct:**
```cpp
void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == HIGH && lastButtonState == LOW) {
    doAction();
  }
  
  lastButtonState = buttonState;  // ← Must update!
}
```

---

### ❌ **Mistake 2: Updating lastButtonState Too Early**

**Wrong:**
```cpp
void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  lastButtonState = buttonState;  // ← Too early!
  
  if (buttonState == HIGH && lastButtonState == LOW) {
    // This NEVER triggers (both are same now)
    doAction();
  }
}
```

**Correct:**
```cpp
void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == HIGH && lastButtonState == LOW) {
    doAction();
  }
  
  lastButtonState = buttonState;  // ← After comparison!
}
```

---

### ❌ **Mistake 3: Using Local Variable**

**Wrong:**
```cpp
void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  int lastButtonState = 0;  // ← Resets every loop!
  
  if (buttonState == HIGH && lastButtonState == LOW) {
    // Always triggers when button HIGH (lastButtonState always 0)
    doAction();
  }
}
```

**Correct:**
```cpp
// Global or static variables
int buttonState = 0;
int lastButtonState = 0;

void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  // Now lastButtonState persists between loops ✓
}
```

---

### ❌ **Mistake 4: Forgetting Debounce**

**Problem:**
```cpp
if (buttonState == HIGH && lastButtonState == LOW) {
  // No debounce delay
  // Mechanical bounce can cause multiple triggers
  counter++;  // Might count 2-3 times per press!
}
```

**Solution:**
```cpp
if (buttonState == HIGH && lastButtonState == LOW) {
  delay(50);  // Debounce delay
  // Now only counts once per press ✓
  counter++;
}
```

**See:** [Button Debouncing](ButtonDebouncing.md) for advanced techniques

---

## Multiple Inputs with Edge Detection

### **Handling Several Buttons**

```cpp
// Button 1
int button1State = 0;
int lastButton1State = 0;

// Button 2
int button2State = 0;
int lastButton2State = 0;

// Button 3
int button3State = 0;
int lastButton3State = 0;

void loop() {
  // Button 1 edge detection
  button1State = digitalRead(BUTTON1_PIN);
  if (button1State == HIGH && lastButton1State == LOW) {
    delay(50);
    action1();
  }
  lastButton1State = button1State;
  
  // Button 2 edge detection
  button2State = digitalRead(BUTTON2_PIN);
  if (button2State == HIGH && lastButton2State == LOW) {
    delay(50);
    action2();
  }
  lastButton2State = button2State;
  
  // Button 3 edge detection
  button3State = digitalRead(BUTTON3_PIN);
  if (button3State == HIGH && lastButton3State == LOW) {
    delay(50);
    action3();
  }
  lastButton3State = button3State;
}
```

**Scalable alternative (arrays):**
```cpp
const int NUM_BUTTONS = 3;
int buttonPins[NUM_BUTTONS] = {2, 3, 4};
int buttonStates[NUM_BUTTONS] = {0};
int lastButtonStates[NUM_BUTTONS] = {0};

void loop() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttonStates[i] = digitalRead(buttonPins[i]);
    
    if (buttonStates[i] == HIGH && lastButtonStates[i] == LOW) {
      delay(50);
      handleButton(i);  // Call function with button number
    }
    
    lastButtonStates[i] = buttonStates[i];
  }
}

void handleButton(int buttonNum) {
  Serial.print("Button ");
  Serial.print(buttonNum);
  Serial.println(" pressed");
}
```

---

## Edge Detection with Interrupts

### **Hardware Interrupt Version**

For ultimate responsiveness, use hardware interrupts with edge detection:

```cpp
volatile int pressCount = 0;
volatile unsigned long lastInterruptTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  
  // Attach interrupt on rising edge
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), 
                  buttonPressed, 
                  RISING);  // Trigger on rising edge
  
  Serial.begin(9600);
}

void loop() {
  // Main code runs without checking button
  // Interrupt handles button automatically
  
  Serial.println(pressCount);
  delay(1000);
}

void buttonPressed() {
  unsigned long currentTime = millis();
  
  // Software debounce in ISR
  if (currentTime - lastInterruptTime > debounceDelay) {
    pressCount++;
    lastInterruptTime = currentTime;
  }
}
```

**Benefits:**
- ✅ Zero latency (immediate response)
- ✅ No polling needed
- ✅ Works even during delays or long operations

**Cautions:**
- ⚠️ ISR (Interrupt Service Routine) should be fast
- ⚠️ Use `volatile` for variables shared with ISR
- ⚠️ Limited to specific pins (2, 3 on Uno)

---

## State Transition Diagrams

### **Button State Machine**

```
        RELEASED (LOW)
             │
             │ Rising edge
             ↓
    ┌─── PRESSED (HIGH) ───┐
    │        │              │
    │        │ Held         │
    │        ↓              │
    │   STILL_PRESSED       │
    │        │              │
    │        │              │
    └────────┘              │
             │ Falling edge │
             ↓              │
        RELEASED (LOW) ←────┘
```

**Actions on edges:**
- Rising edge (RELEASED → PRESSED): Trigger button action
- Falling edge (PRESSED → RELEASED): Optional release action
- While STILL_PRESSED: No action (waiting for release)

---

## Advanced: Edge Detection Library Pattern

### **Reusable EdgeDetector Class**

```cpp
class EdgeDetector {
  private:
    int pin;
    int currentState;
    int lastState;
    
  public:
    EdgeDetector(int p) {
      pin = p;
      pinMode(pin, INPUT);
      currentState = digitalRead(pin);
      lastState = currentState;
    }
    
    void update() {
      lastState = currentState;
      currentState = digitalRead(pin);
    }
    
    bool risingEdge() {
      return (currentState == HIGH && lastState == LOW);
    }
    
    bool fallingEdge() {
      return (currentState == LOW && lastState == HIGH);
    }
    
    bool anyEdge() {
      return (currentState != lastState);
    }
    
    int getState() {
      return currentState;
    }
};

// Usage:
EdgeDetector button(2);

void loop() {
  button.update();
  
  if (button.risingEdge()) {
    Serial.println("Button pressed");
  }
  
  if (button.fallingEdge()) {
    Serial.println("Button released");
  }
}
```

---

## Related Concepts

**Related Findings:**
- [Button Debouncing](ButtonDebouncing.md) — Essential companion to edge detection
- [Non-Blocking Code](Non-Blocking-Code.md) — Often used together (responsive edge detection)

**Related topics:**
- Hardware interrupts (attachInterrupt)
- State machines (managing complex sequences)
- Event-driven programming
- Digital signal processing

---

## Projects Using This Concept

**Current projects:**
1. **[10resistors](../10resistors/README.md)** — Button-controlled LED cycling
   - Implementation: Rising edge detection for button press
   - Result: One press = one LED change
   - Combined with: Debouncing, non-blocking code

**Future projects that will use this:**
- All button-controlled projects
- Rotary encoder (detects rotation direction via edges)
- Mode switching (cycle through display modes)
- Event counters (lap timer, click counter)
- Game controllers (button press actions)

---

## Quick Reference Templates

### **Basic Rising Edge**
```cpp
int state = 0, lastState = 0;

void loop() {
  state = digitalRead(PIN);
  if (state == HIGH && lastState == LOW) {
    // Rising edge action
  }
  lastState = state;
}
```

### **Rising + Falling Edges**
```cpp
int state = 0, lastState = 0;

void loop() {
  state = digitalRead(PIN);
  
  if (state == HIGH && lastState == LOW) {
    onRisingEdge();
  }
  
  if (state == LOW && lastState == HIGH) {
    onFallingEdge();
  }
  
  lastState = state;
}
```

### **Edge Detection + Debounce**
```cpp
int state = 0, lastState = 0;

void loop() {
  state = digitalRead(PIN);
  
  if (state == HIGH && lastState == LOW) {
    delay(50);  // Debounce
    doAction();
  }
  
  lastState = state;
}
```

---

## Troubleshooting Guide

### **Problem: Action Triggers Multiple Times Per Press**
**Symptoms:** One button press causes 2-3 actions  
**Cause:** No debouncing (mechanical bounce)  
**Solution:** Add 50ms delay after edge detection  
**See:** [Button Debouncing](ButtonDebouncing.md)

### **Problem: Action Never Triggers**
**Symptoms:** Button press doesn't do anything  
**Cause:** lastState not updating correctly  
**Solution:** Verify lastState = state at end of loop()

### **Problem: Action Triggers Continuously**
**Symptoms:** Action repeats while button held  
**Cause:** Forgot edge detection, using level detection  
**Solution:** Add && lastState == LOW condition

### **Problem: Random Triggering**
**Symptoms:** Actions happen without button press  
**Cause:** Floating input (no pull-up/pull-down resistor)  
**Solution:** Add INPUT_PULLUP or external pull-down resistor

---

## Key Takeaways

✅ **Essential technique:** Detect transitions, not just states  
✅ **Rising edge:** LOW → HIGH (button just pressed)  
✅ **Falling edge:** HIGH → LOW (button just released)  
✅ **Pattern:** Compare current vs. previous state  
✅ **Remember:** Update lastState at END of loop  
✅ **Variables must persist:** Use global or static  
✅ **Combine with debouncing:** For reliable button handling  
✅ **Professional code:** Edge detection is the standard approach  

**Remember:** Edge detection = responsive, predictable user interfaces!

---

## Further Learning

**Next concepts to explore:**
- Hardware interrupts (even more responsive)
- State machines (complex event sequences)
- Rotary encoders (use both edges + direction)
- Event-driven programming patterns

**Practice exercises:**
- Create toggle switch (on/off with one button)
- Build counter (count button presses)
- Implement mode cycling (3+ modes)
- Measure button hold time (rising + falling edges)

**Advanced projects:**
- Multi-button controller
- Rotary encoder menu system
- Game with multiple button inputs
- Event logger with timestamps

---

**Created from discovery in:**
- 07-08.10.25 sessions (10resistors project)

**Last updated:** 20.10.25

**Status:** Complete and ready for reference ✓