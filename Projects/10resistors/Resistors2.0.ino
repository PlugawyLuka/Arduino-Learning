#include <Arduino.h>

int switchPin = 2;
int ledPins[] = {3,4,5,6,7,8,9,10,11,12};
int pinCount = 10;

int thisPin = -1;
int lastButtonState = HIGH;
int buttonState = 0;

int analogPin = A0;
unsigned long lastVoltageRead = 0;

unsigned long lastDebounceTime = 0;
int lastButtonReading = HIGH;
const unsigned long debounceDelay = 50;

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
    int reading = digitalRead(switchPin);
    
    // If button state changed, reset debounce timer
    if (reading != lastButtonReading) {
        lastDebounceTime = millis();
    }
    
    // If enough time has passed, accept the reading
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != buttonState) {
            buttonState = reading;
            
            // Button was just pressed (HIGH to LOW transition)
            if (buttonState == LOW) {
                if (thisPin >= 0) {
                    digitalWrite(ledPins[thisPin], LOW);
                }

                thisPin = (thisPin + 1) % pinCount;
                digitalWrite(ledPins[thisPin], HIGH);

                Serial.print("Switched to LED: ");
                Serial.println(thisPin);
            }
        }
    }
    
    lastButtonReading = reading;
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