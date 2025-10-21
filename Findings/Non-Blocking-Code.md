# Non-Blocking Code — Using millis() Instead of delay()

**Concept:** Keeping Arduino responsive while performing timed operations  
**Category:** Programming Pattern (Essential)  
**Difficulty:** Beginner-Intermediate  
**Created:** 20.10.25  
**Last Updated:** 20.10.25

---

## What Is Non-Blocking Code?

**Non-blocking code** is a programming approach that allows your Arduino to do multiple things "at the same time" by avoiding delays that freeze the entire program.

**Simple analogy:**
- **Blocking (delay):** Chef waits by oven, can't do anything else until timer beeps
- **Non-blocking (millis):** Chef checks oven timer periodically while prepping other dishes

**Why it matters:**
- ✅ Responsive buttons (no missed presses)
- ✅ Multiple timers running simultaneously
- ✅ Smooth animations and updates
- ✅ Real-time sensor monitoring
- ✅ Professional-quality code

---

## The Problem: delay() Blocks Everything

### **How delay() Works**

```cpp
void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);  // ← Program STOPS here for 1 second
  digitalWrite(LED_PIN, LOW);
  delay(1000);  // ← Program STOPS again for 1 second
}
```

**What happens during delay():**
- ❌ Arduino does NOTHING else
- ❌ Button presses missed
- ❌ Sensor readings skipped
- ❌ Serial communication ignored
- ❌ Other code cannot run

**Real example from 10resistors project:**

```cpp
void Volts() {
  int voltageReading = analogRead(A0);
  Serial.println(voltageReading);
  delay(1000);  // ← BLOCKS for 1 second!
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  // Button press might be missed during delay!
  if (buttonState == HIGH) {
    // This code might not run if delay() is active
    cycleLED();
  }
  
  Volts();  // Calls function with delay(1000)
}
```

**Symptoms:**
- Button feels "laggy" or unresponsive
- Sometimes presses don't register
- Can't do two things at once

---

## The Solution: millis() for Non-Blocking Timing

### **How millis() Works**

**What is millis()?**
- Built-in Arduino function
- Returns time since program started (in milliseconds)
- Doesn't stop program execution
- Continuously counting in background

**Example values:**
```
Time after startup:
0ms     → millis() returns 0
1000ms  → millis() returns 1000
5000ms  → millis() returns 5000
60000ms → millis() returns 60000 (1 minute)
```

**Key insight:** Instead of waiting, we check if enough time has passed.

---

### **Basic Pattern: Blink Without delay()**

**Old way (blocking):**
```cpp
void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);              // ← Blocked for 1 second
  digitalWrite(LED_PIN, LOW);
  delay(1000);              // ← Blocked for 1 second
}
```

**New way (non-blocking):**
```cpp
unsigned long previousMillis = 0;  // Stores last time LED changed
const long interval = 1000;        // Interval in milliseconds

void loop() {
  unsigned long currentMillis = millis();  // Get current time
  
  // Check if enough time has passed
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Remember this time
    
    // Toggle LED
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
  
  // Other code runs immediately (not blocked!)
  checkButton();
  readSensor();
  updateDisplay();
}
```

**How it works:**
1. Remember when we last changed LED (`previousMillis`)
2. Get current time (`currentMillis`)
3. Calculate time elapsed: `currentMillis - previousMillis`
4. If elapsed time ≥ interval, do action and update `previousMillis`
5. Otherwise, skip and continue to next code

**Result:** LED blinks, but loop() keeps running thousands of times per second!

---

## Real-World Example: 10resistors Project

### **The Problem (Before)**

```cpp
void Volts() {
  int voltageReading = analogRead(A0);
  float voltage = voltageReading * (5.0 / 1023.0);
  
  Serial.print("Voltage: ");
  Serial.println(voltage);
  
  delay(1000);  // ← BLOCKS entire program for 1 second!
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  // Edge detection for button press
  if (buttonState == HIGH && lastButtonState == LOW) {
    delay(50);  // Debounce (also blocks)
    cycleLED();
  }
  
  lastButtonState = buttonState;
  
  Volts();  // Calls function with delay(1000)
}
```

**Issues:**
- Button presses during `delay(1000)` are missed
- If button pressed while voltage reading, nothing happens
- Feels unresponsive and sluggish

---

### **The Solution (After)**

```cpp
// Global variables for non-blocking timing
unsigned long previousMillis = 0;
const long voltageInterval = 1000;  // Read voltage every 1 second

void loop() {
  // Button handling (runs every loop, no blocking)
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH && lastButtonState == LOW) {
    delay(50);  // Short debounce OK (50ms negligible)
    
    // Turn off current LED
    if (thisPin >= 0) {
      digitalWrite(ledPins[thisPin], LOW);
    }
    
    // Move to next LED
    thisPin = (thisPin + 1) % 10;
    digitalWrite(ledPins[thisPin], HIGH);
    
    Serial.print("LED ");
    Serial.print(thisPin + 1);
    Serial.println(" ON");
  }
  
  lastButtonState = buttonState;
  
  // Non-blocking voltage reading
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= voltageInterval) {
    previousMillis = currentMillis;
    
    // Read and display voltage (no delay!)
    int voltageReading = analogRead(A0);
    float voltage = voltageReading * (5.0 / 1023.0);
    
    Serial.print("Voltage: ");
    Serial.println(voltage);
  }
  
  // Other code could go here and would run immediately
}
```

**Result:**
- ✅ Button always responsive (no missed presses)
- ✅ Voltage readings every 1 second (in background)
- ✅ Can add more features without breaking responsiveness
- ✅ Professional-quality behavior

---

## Multiple Timers Pattern

### **Running Multiple Tasks with Different Intervals**

**Example: LED blinks every 500ms, sensor reads every 2 seconds, display updates every 100ms**

```cpp
// Timer 1: LED blink
unsigned long previousLED = 0;
const long ledInterval = 500;

// Timer 2: Sensor reading
unsigned long previousSensor = 0;
const long sensorInterval = 2000;

// Timer 3: Display update
unsigned long previousDisplay = 0;
const long displayInterval = 100;

void loop() {
  unsigned long currentMillis = millis();
  
  // Task 1: Blink LED every 500ms
  if (currentMillis - previousLED >= ledInterval) {
    previousLED = currentMillis;
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
  
  // Task 2: Read sensor every 2 seconds
  if (currentMillis - previousSensor >= sensorInterval) {
    previousSensor = currentMillis;
    int sensorValue = analogRead(A0);
    processSensorData(sensorValue);
  }
  
  // Task 3: Update display every 100ms
  if (currentMillis - previousDisplay >= displayInterval) {
    previousDisplay = currentMillis;
    updateDisplay();
  }
  
  // All three tasks run independently without blocking each other!
}
```

**Key benefits:**
- Each task has its own timer
- Tasks don't interfere with each other
- Easy to add more tasks
- Precise timing for each task

---

## Common Patterns & Recipes

### **Pattern 1: Simple Periodic Action**

**Use case:** Do something every X milliseconds

```cpp
unsigned long previousMillis = 0;
const long interval = 1000;  // 1 second

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Your periodic action here
    doSomething();
  }
}
```

---

### **Pattern 2: Timeout (One-Time Delay)**

**Use case:** Do something after X milliseconds, only once

```cpp
unsigned long startTime = 0;
bool timerStarted = false;
bool actionDone = false;

void startTimer() {
  startTime = millis();
  timerStarted = true;
  actionDone = false;
}

void loop() {
  if (timerStarted && !actionDone) {
    if (millis() - startTime >= 5000) {  // 5 second timeout
      doDelayedAction();
      actionDone = true;
      timerStarted = false;
    }
  }
}
```

---

### **Pattern 3: Rate Limiting**

**Use case:** Limit how often something can happen

```cpp
unsigned long lastAction = 0;
const long minInterval = 500;  // Minimum 500ms between actions

void loop() {
  if (buttonPressed()) {
    unsigned long currentMillis = millis();
    
    // Only allow action if enough time has passed
    if (currentMillis - lastAction >= minInterval) {
      lastAction = currentMillis;
      performAction();
    }
  }
}
```

---

### **Pattern 4: State Machine with Timing**

**Use case:** Sequence of timed events (like animation or process)

```cpp
enum State { IDLE, STEP1, STEP2, STEP3 };
State currentState = IDLE;
unsigned long stateStartTime = 0;

void loop() {
  unsigned long currentMillis = millis();
  
  switch (currentState) {
    case IDLE:
      // Wait for trigger
      if (startButtonPressed()) {
        currentState = STEP1;
        stateStartTime = currentMillis;
      }
      break;
      
    case STEP1:
      // Run for 2 seconds
      if (currentMillis - stateStartTime >= 2000) {
        currentState = STEP2;
        stateStartTime = currentMillis;
      }
      break;
      
    case STEP2:
      // Run for 3 seconds
      if (currentMillis - stateStartTime >= 3000) {
        currentState = STEP3;
        stateStartTime = currentMillis;
      }
      break;
      
    case STEP3:
      // Run for 1 second, then back to idle
      if (currentMillis - stateStartTime >= 1000) {
        currentState = IDLE;
      }
      break;
  }
}
```

---

## Important Considerations

### **1. millis() Overflow**

**Problem:** millis() rolls over after ~49.7 days

```cpp
At ~49.7 days: millis() = 4,294,967,295 (maximum unsigned long)
Next millisecond: millis() = 0 (rolls over)
```

**Why this matters:**
```cpp
// This can fail at rollover:
if (millis() > previousMillis + interval) {  // BAD!
  // Action
}

// This handles rollover correctly:
if (millis() - previousMillis >= interval) {  // GOOD!
  // Action
}
```

**Explanation:**
- Subtraction handles overflow automatically (unsigned math wraps correctly)
- Comparison with addition can fail when millis() wraps to 0

**Best practice:** Always use subtraction pattern: `currentMillis - previousMillis`

---

### **2. Very Short Intervals**

**Limitation:** loop() takes time to execute

```cpp
void loop() {
  // If loop takes 10ms to execute...
  if (millis() - previousMillis >= 1) {
    // Won't trigger every 1ms (can't run loop fast enough)
    // Will actually trigger every ~10ms
  }
}
```

**Guidelines:**
- ✅ Intervals ≥ 10-50ms: Reliable
- ⚠️ Intervals < 10ms: May be inconsistent
- ❌ Intervals < 1ms: Use hardware timers instead

---

### **3. Precision vs. Accuracy**

**millis() precision:** ±1-4ms depending on timing

**Example:**
```cpp
// Request 1000ms interval
if (currentMillis - previousMillis >= 1000) {
  // Actual intervals might be: 1001ms, 1003ms, 999ms, 1002ms...
  // Good enough for most applications!
}
```

**When you need better accuracy:**
- Use hardware timers (Timer1, Timer2)
- Use micros() for microsecond precision (but more limited range)
- Use interrupt-based timing

**For typical Arduino projects:** millis() is plenty accurate!

---

## When to Use delay() vs. millis()

### **✅ delay() Is OK When:**

1. **One-time setup operations**
   ```cpp
   void setup() {
     pinMode(LED_PIN, OUTPUT);
     delay(100);  // Let pin stabilize (OK in setup)
     digitalWrite(LED_PIN, HIGH);
   }
   ```

2. **Short debounce delays**
   ```cpp
   if (buttonPressed()) {
     delay(50);  // 50ms debounce (negligible impact)
     // Rest of code
   }
   ```

3. **Simple beginner projects** (one LED blinking, no other features)

4. **Debugging** (temporarily slow down code)
   ```cpp
   Serial.println("Debug value");
   delay(500);  // Slow down output for reading
   ```

---

### **❌ Use millis() When:**

1. **Button/input responsiveness matters**
2. **Multiple things need to happen simultaneously**
3. **Real-time sensor monitoring**
4. **Animations or smooth transitions**
5. **Communication (Serial, I2C, SPI) while doing other tasks**
6. **Professional/production code**
7. **Any project beyond basic LED blink**

**Rule of thumb:** If you say "I wish this was more responsive," use millis()!

---

## Common Mistakes & Solutions

### ❌ **Mistake 1: Declaring previousMillis Inside loop()**

**Wrong:**
```cpp
void loop() {
  unsigned long previousMillis = 0;  // ← RESETS EVERY LOOP!
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= 1000) {
    // This triggers EVERY loop because previousMillis is always 0!
  }
}
```

**Correct:**
```cpp
unsigned long previousMillis = 0;  // ← GLOBAL or static

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;  // Updates persist
    // Triggers every 1 second ✓
  }
}
```

**Lesson:** Timer variables must persist between loop() calls!

---

### ❌ **Mistake 2: Not Updating previousMillis**

**Wrong:**
```cpp
if (currentMillis - previousMillis >= 1000) {
  // Do action
  // ← Forgot to update previousMillis!
}
// This triggers EVERY loop after first second
```

**Correct:**
```cpp
if (currentMillis - previousMillis >= 1000) {
  previousMillis = currentMillis;  // ← Update timer!
  // Do action
}
```

---

### ❌ **Mistake 3: Using == Instead of >=**

**Wrong:**
```cpp
if (currentMillis - previousMillis == 1000) {  // ← Too precise!
  // Might never trigger (loop might skip exact value)
}
```

**Correct:**
```cpp
if (currentMillis - previousMillis >= 1000) {  // ← Use >=
  // Triggers when elapsed time reaches or exceeds interval
}
```

---

### ❌ **Mistake 4: Long Operations Still Block**

**Problem:**
```cpp
if (currentMillis - previousMillis >= 1000) {
  previousMillis = currentMillis;
  
  // Non-blocking timer, but...
  for (int i = 0; i < 1000; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(10);  // ← This STILL blocks!
  }
}
```

**Solution:** Break long operations into smaller chunks or use state machines

---

## Debugging Non-Blocking Code

### **Serial Monitor Technique**

```cpp
void loop() {
  unsigned long currentMillis = millis();
  
  // Debug: Print timing info
  Serial.print("Time: ");
  Serial.print(currentMillis);
  Serial.print(" | Elapsed: ");
  Serial.println(currentMillis - previousMillis);
  
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    Serial.println(">>> ACTION TRIGGERED <<<");
    doAction();
  }
  
  delay(100);  // Slow down output for readability
}
```

**Output example:**
```
Time: 0 | Elapsed: 0
Time: 100 | Elapsed: 100
Time: 200 | Elapsed: 200
...
Time: 1000 | Elapsed: 1000
>>> ACTION TRIGGERED <<<
Time: 1100 | Elapsed: 100
Time: 1200 | Elapsed: 200
```

---

### **LED Indicator Technique**

```cpp
void loop() {
  // Visual feedback: LED shows when task runs
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    digitalWrite(INDICATOR_LED, HIGH);  // Turn on
    doTimedTask();
    digitalWrite(INDICATOR_LED, LOW);   // Turn off
  }
}
```

---

## Advanced: Converting Blocking to Non-Blocking

### **Example: Animated LED Sequence**

**Blocking version:**
```cpp
void animateLEDs() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);  // ← Blocks for 100ms each LED
    digitalWrite(ledPins[i], LOW);
  }
}
```

**Non-blocking version:**
```cpp
int currentLED = 0;
unsigned long previousMillis = 0;
const long stepInterval = 100;
bool animationActive = false;

void startAnimation() {
  currentLED = 0;
  animationActive = true;
  previousMillis = millis();
}

void loop() {
  if (animationActive) {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= stepInterval) {
      previousMillis = currentMillis;
      
      // Turn off previous LED
      if (currentLED > 0) {
        digitalWrite(ledPins[currentLED - 1], LOW);
      }
      
      // Turn on current LED
      digitalWrite(ledPins[currentLED], HIGH);
      
      currentLED++;
      
      // End animation when done
      if (currentLED >= 10) {
        digitalWrite(ledPins[9], LOW);
        animationActive = false;
      }
    }
  }
  
  // Other code runs smoothly during animation!
  checkButton();
  readSensors();
}
```

---

## Related Concepts

**Related Findings:**
- [Button Debouncing](ButtonDebouncing.md) — Often used with non-blocking code
- Edge Detection _(to be created)_ — Event detection without blocking
- State Machines _(could be separate Finding)_ — Managing complex timed sequences

**Related topics:**
- Hardware interrupts (even more responsive)
- Timer libraries (simplify multiple timers)
- RTOS (Real-Time Operating System) for complex multitasking
- micros() for microsecond timing

---

## Projects Using This Concept

**Current projects:**
1. **[10resistors](../10resistors/README.md)** — First implementation
   - Problem: delay(1000) made button unresponsive
   - Solution: millis() for voltage monitoring
   - Lesson: Responsive UI requires non-blocking code

**Future projects that will use this:**
- Any button-controlled project
- Sensor monitoring with user input
- Animations or sequences
- Multi-function projects (keypad + display + sensors)
- Real-time control systems

---

## Quick Reference Card

### **Basic Template**
```cpp
// At top of sketch (global)
unsigned long previousMillis = 0;
const long interval = 1000;  // milliseconds

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Your code here (runs every 'interval' milliseconds)
  }
  
  // Other code (runs every loop, not blocked)
}
```

### **Multiple Timers Template**
```cpp
// Timer 1
unsigned long timer1Previous = 0;
const long timer1Interval = 500;

// Timer 2
unsigned long timer2Previous = 0;
const long timer2Interval = 2000;

void loop() {
  unsigned long now = millis();
  
  // Timer 1 check
  if (now - timer1Previous >= timer1Interval) {
    timer1Previous = now;
    task1();
  }
  
  // Timer 2 check
  if (now - timer2Previous >= timer2Interval) {
    timer2Previous = now;
    task2();
  }
}
```

---

## Conversion Checklist

**Converting blocking code to non-blocking:**

- [ ] Identify all delay() calls
- [ ] Create unsigned long variables for timing
- [ ] Replace delay() with millis() checks
- [ ] Ensure previousMillis is global or static
- [ ] Use >= for time comparisons
- [ ] Update previousMillis after action
- [ ] Test responsiveness (button, sensors)
- [ ] Check for rollover handling (use subtraction)

---

## Key Takeaways

✅ **Essential pattern:** Use millis() for responsive Arduino programs  
✅ **delay() blocks:** Nothing else can run during delay()  
✅ **millis() doesn't block:** Checks time without stopping execution  
✅ **Pattern:** `if (currentMillis - previousMillis >= interval)`  
✅ **Remember:** Update previousMillis after action  
✅ **Multiple timers:** Each task gets its own previousMillis  
✅ **Handles overflow:** Subtraction pattern works after ~49 days  
✅ **Professional code:** Non-blocking is the standard approach  

**Remember:** Good Arduino code is responsive Arduino code!

---

## Further Learning

**Next concepts to explore:**
- Hardware interrupts (attachInterrupt) — Even more responsive
- Timer1/Timer2 libraries — Precise hardware timing
- State machines — Managing complex sequences
- RTOS (FreeRTOS for Arduino) — True multitasking

**Practice exercises:**
- Convert all your delay() code to millis()
- Create 3 independent blinking LEDs (different rates)
- Build button-controlled animation with non-blocking timing
- Implement timeout (action after X seconds of inactivity)

**Advanced projects:**
- Multi-step process with different durations
- Game with animations, score display, and button input
- Sensor logger with display update and data saving

---

**Created from discovery in:**
- 08.10.25 session (10resistors project debugging)

**Last updated:** 20.10.25

**Status:** Complete and ready for reference ✓