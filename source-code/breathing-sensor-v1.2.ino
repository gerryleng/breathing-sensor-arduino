/*
  Version: 1.2
  Date: 04/03/2025
  Changes: 1. Dynamically detect breathing peaks (inhales)
           2. Count peaks to calculate BPM
           3. Display BPM in Serial Monitor

  AnalogReadSerial + BPM Measurement
  Reads an analog input on pin A0, detects breathing peaks, and calculates BPM.
  Turns an LED on/off when a breath is detected.
*/

int ledPin = 13;
int sensorPin = A0;

// Variables for peak detection
int lastValue = 0;
bool breathDetected = false;

// Variables for BPM measurement
int breathCount = 0;
unsigned long lastPeakTime = 0;
unsigned long bpmUpdateInterval = 60000; // Update BPM every 60 seconds
unsigned long startTime;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  startTime = millis(); // Start the timer
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue); // Print raw sensor data

  // Detect peak (inhale): Value must be rising, and then start falling
  if (sensorValue > lastValue + 10 && !breathDetected) {  
    breathDetected = true; // Mark that a breath has been detected
    digitalWrite(ledPin, HIGH); // Turn on LED (breath detected)
    
    // Count the breath only if sufficient time has passed since last peak
    if (millis() - lastPeakTime > 2000) { // At least 2 seconds between breaths
      breathCount++;
      lastPeakTime = millis();
    }
  } 
  // Reset detection after peak
  else if (sensorValue < lastValue) { 
    breathDetected = false;
    digitalWrite(ledPin, LOW); // Turn off LED
  }

  lastValue = sensorValue; // Store last value for comparison

  // Update BPM every minute
  if (millis() - startTime >= bpmUpdateInterval) {
    int bpm = breathCount; // Since we are counting over 60 sec, 1 count = 1 BPM
    Serial.print("BPM: ");
    Serial.println(bpm);

    // Reset counter for next measurement
    breathCount = 0;
    startTime = millis();
  }

  delay(100); // Small delay for stability
}
