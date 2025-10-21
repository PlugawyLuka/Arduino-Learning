// Define LED array to  correct pins
int ledPins[] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int pinCount = 10;
// Define array of 10 values after converted into 0-255 range / 10.

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
  // Convert analog read to 10 scale and store it in outputValue
  sensorValue = analogRead(potenciometerPin);
  outputValue = map(sensorValue, 0, 1023, 0, 10);

  for (int n = 0; n < outputValue; n++){
    if (int n = outputValue)  {
      digitalWrite(ledPins[n], LOW);
    }
  digitalWrite(ledPins[n], HIGH);
  }

  

  // Print current information in serial monitor
  Serial.print("Sensor = ");
  Serial.print(sensorValue);
  Serial.print("\n output = ");
  Serial.print(outputValue);

  // Refresh after 2 miliseconds
  delay(2);

}