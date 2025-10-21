j# Arduino Timing: delay() vs millis() — Blocking vs Non-Blocking Code

## The Problem with delay() — Blocking Execution

### How `delay()` Works (Blocking)

```cpp
void loop() {
    nextLed();      // Check button
    delay(1000);    // ⚠️ STOPS EVERYTHING for 1 second
    // Button presses during this 1 second are MISSED
}
```

**What happens:**
1. Arduino reads button state
2. Arduino **freezes completely** for 1000ms
3. During freeze: button presses are ignored
4. After 1 second: Arduino wakes up and reads button again

**Timeline:**
```
0ms: Read button ✓
1ms-999ms: FROZEN (button presses lost) ❌  
1000ms: Read button again ✓
1001ms-1999ms: FROZEN again ❌
```

## The Solution with millis() — Non-Blocking Execution

### How `millis()` Works (Non-Blocking)

```cpp
unsigned long lastVoltageRead = 0;

void loop() {
    nextLed();      // ✓ Check button EVERY loop cycle
    Volts();        // ✓ Only read voltage when needed
}

void Volts() {
    if (millis() - lastVoltageRead >= 1000) {  // Check if 1 second passed
        // Read voltage
        lastVoltageRead = millis();  // Remember when we did this
    }
    // Function exits immediately - no waiting!
}
```

**What happens:**
1. Arduino reads button state
2. Arduino checks: "Has 1 second passed since last voltage reading?"
3. If yes: read voltage, if no: skip it
4. Arduino **immediately** goes back to step 1

**Timeline:**
```
0ms: Read button ✓, Check voltage timing ✓
1ms: Read button ✓, Skip voltage (too soon) ✓
2ms: Read button ✓, Skip voltage ✓
...
1000ms: Read button ✓, Read voltage ✓, Reset timer
1001ms: Read button ✓, Skip voltage ✓
```

## Visual Comparison

**With `delay(1000)` (Bad):**
```
Button Press:     ↓         ↓    ↓         ↓
Arduino Awake:    |---------|    |---------|
Arduino Asleep:          ████████       ████████
Response:         ✓         ❌    ❌         ✓
```

**With `millis()` (Good):**
```
Button Press:     ↓    ↓    ↓    ↓    ↓    ↓
Arduino Awake:    |||||||||||||||||||||||||||||
Arduino Asleep:   (never)
Response:         ✓    ✓    ✓    ✓    ✓    ✓
```

## Performance Impact

### Before (with delay)
- Button response time: **0-1000ms** (depending when in the delay cycle you pressed)
- User experience: **Laggy, unpredictable**

### After (with millis)
- Button response time: **<1ms** (next loop cycle)
- User experience: **Instant, reliable**

## The Concept: Cooperative Multitasking

```cpp
// BAD - Blocking approach
void loop() {
    doTask1();
    delay(1000);    // Everyone waits
    doTask2();
    delay(500);     // Everyone waits again
}

// GOOD - Non-blocking approach  
void loop() {
    doTask1();      // Always runs
    doTask2IfTime(); // Runs when needed
    doTask3IfTime(); // Runs when needed
}
```

## Real-World Analogy

**Delay approach** = Sleeping waiter:
- Waiter takes your order
- **Falls asleep for 1 minute**
- Other customers can't order during sleep
- Very bad service!

**Millis approach** = Alert waiter:
- Waiter constantly checks all tables
- Remembers: "Check kitchen timer every minute"
- Takes new orders immediately
- Excellent service!

## Standard Non-Blocking Pattern

**Template to remember:**
```cpp
unsigned long lastTime = 0;
const unsigned long interval = 1000;  // 1 second

void periodicTask() {
    if (millis() - lastTime >= interval) {
        // Do the periodic task here
        Serial.println("Task executed!");
        
        lastTime = millis();  // Reset timer
    }
}

void loop() {
    // These run every loop cycle
    checkButtons();
    updateDisplay();
    
    // This runs only when it's time
    periodicTask();
}
```

## Multiple Timers Example

```cpp
unsigned long ledTimer = 0;
unsigned long sensorTimer = 0;
unsigned long serialTimer = 0;

void loop() {
    // Always responsive
    checkButtons();
    
    // Blink LED every 500ms
    if (millis() - ledTimer >= 500) {
        toggleLED();
        ledTimer = millis();
    }
    
    // Read sensor every 2 seconds
    if (millis() - sensorTimer >= 2000) {
        readSensor();
        sensorTimer = millis();
    }
    
    // Print status every 5 seconds
    if (millis() - serialTimer >= 5000) {
        printStatus();
        serialTimer = millis();
    }
}
```

## Key Learning Points

### Golden Rules:
1. **Never use `delay()` in main loop** if you need responsive input
2. **Use `millis()` for all timing** when multitasking
3. **Each task gets its own timer variable**
4. **Check timing conditions with `>=` not `==`**

### When to Use Each:
- **`delay()`**: Simple programs with only one task, or brief pauses (like debouncing)
- **`millis()`**: Any program that needs to do multiple things simultaneously

### Common Mistakes:
```cpp
// WRONG - can miss the exact moment
if (millis() - lastTime == 1000)

// CORRECT - catches any time >= target
if (millis() - lastTime >= 1000)
```

## Conclusion

This timing pattern transformation is what made your LED button responsive while maintaining background voltage monitoring. The Arduino now handles both tasks smoothly instead of blocking on one task while ignoring the other.

**Result:** Professional-quality, responsive embedded software that can handle multiple concurrent operations!