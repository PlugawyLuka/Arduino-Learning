#include <Arduino.h>

int switchPin = 2;
int ledPins[] = {12,11,10,9,8,7,6,5,4,3};
int pinCount = 10;

int thisPin = -1;
int lastButtonState = HIGH;
int buttonState = 0;

int analogPin = A0;

void setup()
{
  pinMode(switchPin, INPUT_PULLUP);
  for (int thisPin = 0; thisPin < pinCount; thisPin++)
  {
    pinMode(ledPins[thisPin], OUTPUT);
  }
  Serial.begin(9600);
}

void loop()
{
  nextLed();
  checkVolts();
}

void nextLed()
{
  buttonState = digitalRead(switchPin);
  if (buttonState == HIGH && lastButtonState == LOW)  {
    digitalWrite(ledPins[thisPin], LOW);
    thisPin = (thisPin + 1) %pinCount;
    digitalWrite(ledPins[thisPin], HIGH);

    Serial.print("Switched to LED: ");
    Serial.println(thisPin);
  }
  lastButtonState = buttonState;
}

void checkVolts()
{
  int rawReading = analogRead(analogPin);
  float volts = rawReading / 204.6;
  Serial.println(volts);
  delay(1000);
}