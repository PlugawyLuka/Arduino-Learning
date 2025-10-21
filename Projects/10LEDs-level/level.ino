// Devine LED array to  correct pins
int ledPins[] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int pinCount = 10;
// Devine array of 10 values after converted into 0-255 range / 10.

// Assign potenciometer's pin
const int potenciometerPin = A0;

// Create variables for function to get analog read from potenciometer and change it to range  0=255
int sensorValue = 0;
int outputValue = 0;

void setup()  {

  // Assign LED pins as an output
  for (int i = 0; i < pinCount; i++)  {
    pinMode(ledPins[i], OUTPUT);
  }

  // Enable serial mnoitor
  Serial.begin(9600);
}

void loop() {
  // 1. Read sensor
  sensorValue = analogRead(potenciometerPin);
  
  // 2. Map to LED count (0-9 is safer)
  outputValue = map(sensorValue, 0, 1023, 0, pinCount);
  
  // 3. Constrain to valid range (safety)
  outputValue = constrain(outputValue, 0, pinCount);
  
  // 4. Update LEDs
  for (int n = 0; n < outputValue; n++) {
    digitalWrite(ledPins[n], HIGH);
  }
  for (int n = outputValue; n < pinCount; n++) {
    digitalWrite(ledPins[n], LOW);
  }
  
  // 5. Debug output
  Serial.print("Sensor = ");
  Serial.print(sensorValue);
  Serial.print(" | output = ");
  Serial.println(outputValue);
  
  // 6. Delay
  delay(100);
}