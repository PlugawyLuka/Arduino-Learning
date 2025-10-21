# Double-Click Detection Research Project — 09.10.25 14:45

## Project Overview
Building on our LED and button debouncing knowledge, this project explores implementing double-click detection in Arduino.

## Learning Objectives
- Master timing with `millis()`
- Understand state machines
- Practice debugging complex interactions
- Apply debouncing knowledge

## Research Topics

### 1. Timing Concepts
Study and document:
```
First Click │     │ Second Click
           ▼     │    ▼
HIGH ──┐   │     │    ┌────
       └───┘     │    └────
    Click1    Gap    Click2
|--100ms--|200ms|--100ms--|
```

Tasks:
- [ ] Research standard double-click timings (Windows: 500ms default)
- [ ] Document timing components:
  - Single click duration
  - Maximum gap between clicks
  - Total detection window
- [ ] Create timing diagrams in your notebook

### 2. State Machine Design
Document these states:
```
┌─────────────┐
│  IDLE       │◄────────────────┐
└─────┬───────┘                 │
      │ First Click             │ Timeout
      ▼                         │
┌─────────────┐    Second Click ┌─────────────┐
│ FIRST_CLICK │─────────────────► DOUBLE_CLICK│
└─────────────┘                 └─────────────┘
```

Tasks:
- [ ] Draw complete state diagram
- [ ] List transitions between states
- [ ] Document timing for each state
- [ ] Plan error handling

### 3. Variables Planning
Required variables (plan their purpose):
```cpp
// State tracking
currentState
lastButtonState
buttonReading

// Timing
firstClickTime
clickGapStart
totalClickTime

// Configuration
CLICK_MAX_DURATION
CLICK_GAP_MAX
DOUBLE_CLICK_TIMEOUT
```

### 4. Debug Strategy
Serial output format:
```
Time(ms) | State | Button | Action
0000     | IDLE  | HIGH   | Waiting
0100     | CLICK1| LOW    | First Click
0300     | WAIT  | HIGH   | Gap
0400     | CLICK2| LOW    | Double!
```

## Implementation Phases

### Phase 1: Planning (Current)
- [ ] Complete state diagram in notebook
- [ ] Document all timing constants
- [ ] Create variable list with types
- [ ] Draw debug output format

### Phase 2: Testing Plan
Scenarios to test:
1. Perfect double-click
2. Too slow second click
3. Too fast second click
4. Button held down
5. Random button noise

### Phase 3: Implementation Planning
Break into functions:
```cpp
// Proposed function structure (pseudocode only)
void checkButton()
void updateState()
void processClick()
void handleTimeout()
void debugOutput()
```

## Debugging Tools

### Serial Monitor Templates
```cpp
// Debug output format
"State: %s, Time: %lu, Button: %d\n"
"Click duration: %lu, Gap: %lu\n"
```

### LED Feedback Plan
- Solid LED: Waiting for first click
- Fast blink: Detected first click
- Double blink: Success
- Long blink: Error/timeout

## Study Materials
1. Review your `Debounce.md` notes
2. Study state machine patterns
3. Research timing diagrams
4. Read `millis()` documentation

## Test Scenario Checklist
- [ ] Single click only
- [ ] Perfect double-click
- [ ] Slow double-click
- [ ] Fast double-click
- [ ] Button held down
- [ ] Random presses
- [ ] Power-on behavior

## Next Steps
1. Complete state diagram
2. Document timing requirements
3. Create test plan
4. Design debug output
5. Start with pseudocode

## References
- Your `Debounce.md` (09.10.25)
- Arduino Button Tutorial
- State Machine Patterns
- Windows Double-Click Settings

**Note:** Don't start coding until you have a complete plan! Good preparation prevents debugging headaches.

## Document Properties
- Created: 09.10.25 14:45
- Location: c:\Users\Luka\Code\Luka\Arduino\10resistors\Double-click.md