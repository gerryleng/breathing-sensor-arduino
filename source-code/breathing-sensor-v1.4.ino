/*
  Version: 1.4
  Date: 11/03/2025
  Changes:
    - Removed buzzer and unnecessary functions.
    - LED now blinks on normal breathing (inhale detection).
    - LED stays ON to indicate abnormal BPM (low or high).
    - Cleaned up variable names and structure.

  AnalogReadSerial + BPM Measurement + LED Alert
  Reads analog input on pin A0, detects breathing peaks, calculates BPM,
  and uses an LED for feedback and alerts.
*/

#define MIN_BPM 8    // Minimum safe breaths per minute
#define MAX_BPM 40   // Maximum safe breaths per minute

const int ledPin = 13;      // LED indicator pin
const int sensorPin = A0;   // Breathing sensor analog input pin

// Peak detection variables
int lastValue = 0;
bool breathDetected = false;

// BPM measurement variables
int breathCount = 0;
unsigned long lastPeakTime = 0;
unsigned long bpmUpdateInterval = 60000; // 60 seconds
unsigned long startTime;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // Initialize timer
  startTime = millis();
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue); // Print sensor data to Serial Monitor

  // Detect a breath (peak detection based on rising value)
  if (sensorValue > lastValue + 10 && !breathDetected) {
    breathDetected = true;
    digitalWrite(ledPin, HIGH); // Blink LED on inhale (breath detected)

    // Count the breath if sufficient time has passed since last peak
    if (millis() - lastPeakTime > 2000) {
      breathCount++;
      lastPeakTime = millis();
    }
  }
  // Reset detection after the peak is passed
  else if (sensorValue < lastValue) {
    breathDetected = false;
    digitalWrite(ledPin, LOW); // Turn off LED during exhale
  }

  lastValue = sensorValue; // Update last value for next loop

  // Update BPM every minute
  if (millis() - startTime >= bpmUpdateInterval) {
    int bpm = breathCount;
    Serial.print("BPM: ");
    Serial.println(bpm);

    // Anomaly detection: if BPM is too low or too high, turn LED ON permanently
    if (bpm < MIN_BPM || bpm > MAX_BPM) {
      Serial.println("⚠ WARNING: Irregular Breathing Detected!");
      digitalWrite(ledPin, HIGH); // LED ON for warning
    } else {
      digitalWrite(ledPin, LOW); // LED off if no issue
    }

    // Reset for next interval
    breathCount = 0;
    startTime = millis();
  }

  delay(100); // Short delay for stability
}
