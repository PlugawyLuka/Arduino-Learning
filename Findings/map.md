# Arduino map() — practical guide (analogRead → analogWrite)

Purpose
- Remap a value from one numeric range to another (linear scaling, integer math).
- Typical use: convert analogRead (0..1023) to PWM duty cycle (0..255) for analogWrite.

Signature and math
- Signature: `long map(long x, long in_min, long in_max, long out_min, long out_max)`
- Formula: `out = (x − in_min) × (out_max − out_min) / (in_max − in_min) + out_min`

Basic workflow
1) Read sensor (e.g., potentiometer on A0) to get 0..1023.
2) Map to the target range (e.g., 0..255 for PWM).
3) Optionally clamp with constrain() to stay within bounds.
4) Write to a PWM-capable output pin with analogWrite.

Example snippets
```c
// Read → map → write (UNO PWM pins: 3, 5, 6, 9, 10, 11)
int sensor = analogRead(A0);               // 0..1023
int pwm    = map(sensor, 0, 1023, 0, 255); // 0..255
pwm        = constrain(pwm, 0, 255);       // optional clamp
analogWrite(9, pwm);                       // write to PWM pin (not A0)
```

Common variants
- Inverted output: brighter when sensor is low
```c
int pwm = map(sensor, 0, 1023, 255, 0);
```
- Calibrated endpoints (use measured min/max instead of 0/1023)
```c
const int S_MIN = 18;    // measured low
const int S_MAX = 1008;  // measured high
int pwm = map(sensor, S_MIN, S_MAX, 0, 255);
pwm = constrain(pwm, 0, 255);
```

Notes for your Potentiometer.ino
- Use the output pin variable when writing:
```c
// Correct: write to analogOutPin, not analogInPin
analogWrite(analogOutPin, outputValue);
```
- Only PWM-capable pins accept analogWrite.

Delay vs millis (non‑blocking updates)
- Using delay() slows responsiveness. Prefer a millis()-based loop to update at intervals while keeping loop() free for other tasks (see your /10resistors/delayVSmillis example).
```c
// Pseudocode: non-blocking cycle
unsigned long last = 0;
const unsigned long interval = 10; // ms

if (millis() - last >= interval) {
  last = millis();
  int sensor = analogRead(A0);
  int pwm = map(sensor, 0, 1023, 0, 255);
  analogWrite(9, pwm);
}
```

Accuracy and pitfalls
- Integer truncation: map() uses integer math; values are truncated, not rounded.
- No auto-clamp: map() extrapolates if x is outside [in_min, in_max]; add constrain() when needed.
- Validate endpoints: avoid `in_min == in_max` (division by zero).
- Noise: add simple smoothing (moving average) before mapping if output flickers.

Troubleshooting checklist
- Verify the output pin supports PWM.
- Print raw sensor values with Serial to confirm the input range.
- Confirm you’re writing to the correct output pin (use your analogOutPin variable).
- Add constrain() after map() to prevent out-of-range writes.
- If updates feel sluggish, switch from delay() to a millis() scheduler.

Quick reference
- Map: `map(x, a, b, c, d)`
- Clamp: `constrain(v, min, max)`
- Invert: swap `c` and `d`
- Calibrate: replace `a/b` with measured sensor endpoints

See also
- Your example: /10resistors/delayVSmillis (non-blocking timing pattern)