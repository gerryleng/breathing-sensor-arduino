/*
  Version: 1.3
  Date: 04/03/2025
  Changes: 1. Added anomaly detection for abnormal breathing rates.
           2. Integrated buzzer & alert LED for warning signals.
           3. Fixed missing variable definitions and setup issues.

  AnalogReadSerial + BPM Measurement + Anomaly Detection
  Reads an analog input on pin A0, detects breathing peaks, calculates BPM,
  and alerts for abnormal breathing patterns.
*/

#define MIN_BPM 8   // Minimum safe breaths per minute (Possible apnea if below this)
#define MAX_BPM 40  // Maximum safe breaths per minute (Possible hyperventilation if above this)

int ledPin = 13;
int sensorPin = A0;
int buzzer = 11;
int alertLed = 12;

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
  pinMode(buzzer, OUTPUT);
  pinMode(alertLed, OUTPUT);
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

  // Update BPM every minute & Check for Abnormal Breathing
  if (millis() - startTime >= bpmUpdateInterval) {
    int bpm = breathCount; // Since we are counting over 60 sec, 1 count = 1 BPM
    Serial.print("BPM: ");
    Serial.println(bpm);

    // Check for anomalies
    if (bpm < MIN_BPM || bpm > MAX_BPM) {
      Serial.println("⚠ WARNING: Irregular Breathing Detected! ⚠");
      digitalWrite(alertLed, HIGH);
      tone(buzzer, 1000, 500); // Sound buzzer at 1kHz for 500ms
    } else {
      digitalWrite(alertLed, LOW);
      noTone(buzzer); // Stop buzzer if breathing is normal
    }

    // Reset counter for next measurement
    breathCount = 0;
    startTime = millis();
  }

  delay(100); // Small delay for stability
}
