#include <Arduino.h>

int switchPin = 2;
int ledPins[] = {3,4,5,6,7,8,9,10,11,12};
int pinCount = 10;

int thisPin = -1;
int lastButtonState = HIGH;
int buttonState = 0;

int analogPin = A0;
unsigned long lastVoltageRead = 0;

void setup()
{
  pinMode(switchPin, INPUT_PULLUP);
  for (int i = 0; i < pinCount; i++)
  {
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop()
{
    nextLed();
    Volts();
}

void nextLed() {
  buttonState = digitalRead(switchPin);
  if (buttonState == LOW && lastButtonState == HIGH)  
  {
    if (thisPin >= 0)
    {
        digitalWrite(ledPins[thisPin], LOW);
    }

    thisPin = (thisPin + 1) %pinCount;
    digitalWrite(ledPins[thisPin], HIGH);

    Serial.print("Switched to LED: ");
    Serial.println(thisPin);
  }
  lastButtonState = buttonState;
}

void Volts()
{
    if (millis() - lastVoltageRead >= 1000)
    {
        int rawReading = analogRead(analogPin);
        float volts = rawReading / 204.6;
        Serial.println(volts);
        lastVoltageRead = millis();
    }
}

/*
Arduino LED Cycling Code — Line-by-Line Explanation for Beginners

VARIABLE DECLARATIONS (Global Variables)
// These variables are accessible from anywhere in the program

int switchPin = 2;
// Creates an integer variable named 'switchPin' and sets it to 2
// This tells Arduino that our button is connected to digital pin 2
// Using a variable name instead of the number makes code easier to read and change later
// 'int' means integer (whole numbers like 1, 2, 3...)
// The semicolon ';' ends every statement in Arduino code

int ledPins[] = {3,4,5,6,7,8,9,10,11,12};
// Creates an array (list) of 10 integers representing pin numbers
// Instead of having 10 separate variables (led1, led2, led3...), we store all LED pins in one organized list
// We can access them using index numbers: ledPins[0] = pin 3, ledPins[1] = pin 4, etc.
// Arrays start counting from 0, not 1! So ledPins[0] is the first LED pin

int pinCount = 10;
// Stores the number 10 (how many LEDs we have)
// Makes loops easier and prevents mistakes
// If we add/remove LEDs later, we only change this one number

int thisPin = 0;
// Tracks which LED is currently turned on
// When thisPin = 0, LED on pin 3 is on. When thisPin = 1, LED on pin 4 is on
// This is our "pointer" to the active LED
// This variable changes throughout the program to cycle through LEDs

int lastButtonState = HIGH;
int buttonState = 0;
// Two variables to track button presses
// buttonState reads the current button state
// lastButtonState remembers what the button was doing in the previous loop cycle
// HIGH means 5V (button not pressed with INPUT_PULLUP). LOW means 0V (button pressed)
// We start with HIGH because the button starts not-pressed

SETUP FUNCTION (Runs Once at Startup)
void setup()
{
  // Arduino runs this code exactly once when it powers on or resets
  // Use this to configure pins and initialize settings
  // 'void' means this function doesn't return a value
  // The curly brace '{' starts the function code block
  
  pinMode(switchPin, INPUT_PULLUP);
  // Configures pin 2 as an input with internal pull-up resistor enabled
  // INPUT_PULLUP means:
  // - Pin 2 can read signals (INPUT)
  // - Arduino connects an internal 20kΩ resistor from pin 2 to 5V
  // - When button is not pressed: pin reads HIGH (5V)
  // - When button is pressed (connects to GND): pin reads LOW (0V)
  // This eliminates the need for external resistors on your button circuit
  
  for (int thisPin = 0; thisPin < pinCount; thisPin++)
  {
    pinMode(ledPins[thisPin], OUTPUT);
  }
  // A loop that runs 10 times to set up all LED pins
  // Instead of writing pinMode(3, OUTPUT); pinMode(4, OUTPUT);... 10 times, this loop does it automatically
  // Loop breakdown:
  // - int thisPin = 0: Start with thisPin = 0
  // - thisPin < pinCount: Keep looping while thisPin is less than 10
  // - thisPin++: Add 1 to thisPin after each loop
  // Loop 1: thisPin=0, sets pin 3 as OUTPUT
  // Loop 2: thisPin=1, sets pin 4 as OUTPUT
  // ...continues until all 10 pins are set
  // OUTPUT means the pin can send signals (turn LEDs on/off)
  
  Serial.begin(9600);
  // Starts serial communication at 9600 bits per second
  // Allows Arduino to send messages to your computer
  // You can see these messages in the Serial Monitor (Tools → Serial Monitor in Arduino IDE)
  // 9600 is a standard speed. Both Arduino and your computer must use the same speed to communicate
}

// MAIN LOOP FUNCTION (Runs Forever)
void loop() {
  // After setup() finishes, Arduino runs this code repeatedly forever until you turn off the power
  
  buttonState = digitalRead(switchPin);
  // Reads the current voltage on pin 2 and stores it in buttonState
  // We need to check if the button is pressed right now
  // digitalRead() returns:
  // - HIGH if pin voltage is close to 5V (button not pressed)
  // - LOW if pin voltage is close to 0V (button pressed)
  
  if (buttonState == LOW && lastButtonState == HIGH)  {
    // Checks if the button was just pressed (edge detection)
    // We only want to switch LEDs once per button press, not continuously while holding the button
    // buttonState == LOW: Button is currently pressed
    // lastButtonState == HIGH: Button was NOT pressed in the previous loop
    // Both conditions true = button was just pressed (transition moment)
    // == compares two values, = assigns a value. && means "and" (both conditions must be true)
    
    digitalWrite(ledPins[thisPin], LOW);
    // Turns OFF the currently active LED
    // Before turning on the next LED, we turn off the current one so only one LED is on at a time
    // ledPins[thisPin] gets the pin number of the current LED
    // LOW sends 0V to that pin, turning the LED off
    
    thisPin = (thisPin + 1) %pinCount;
    // Move to the next LED in the sequence
    // thisPin + 1: Add 1 to move to next LED
    // % pinCount: Modulo operator for wraparound (9+1 becomes 0, cycling back to first LED)
    // % gives the remainder after division. 10 % 10 = 0, so we cycle from LED 9 back to LED 0
    
    digitalWrite(ledPins[thisPin], HIGH);
    // Turns ON the new current LED
    // After moving to the next LED position, we turn on that LED
    // HIGH sends 5V to the pin, turning the LED on

    Serial.print("Switched to LED: ");
    Serial.println(thisPin);
    // Sends a message to your computer showing which LED is now on
    // Debugging and verification. You can watch the Serial Monitor to confirm the code is working correctly
    // Serial.print(): Sends text without a new line
    // Serial.println(): Sends the number and adds a new line
  }
  
  lastButtonState = buttonState;
  // Remembers the current button state for the next loop cycle
  // Next time through the loop, this becomes the "last" state so we can detect when the button changes from not-pressed to pressed
}

/*
HOW THE COMPLETE PROGRAM WORKS:

1. Startup: Configure pins, start serial communication
2. Forever loop:
   - Read button
   - If button just pressed: turn off current LED, move to next LED, turn on new LED
   - Remember button state
   - Repeat

Expected behavior: Each button press cycles through LEDs 1→2→3→...→10→1→2...

COMMON BEGINNER MISTAKES TO AVOID:
- Forgetting semicolons ;
- Using = instead of == in comparisons
- Array indexing confusion (remember arrays start at 0)
- Not understanding the difference between digitalWrite() and pinMode()
*/