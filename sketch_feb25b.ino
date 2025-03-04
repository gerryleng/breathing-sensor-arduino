/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Turns an LED on/off based on sensor input.
  Connect the anode (long leg) of the LED to pin 13 on the Arduino.
  Connect the cathode (short leg) of the LED to GND through a 220Ω resistor.
*/

// Define the LED pin
int ledPin = 13;

// Define the sensor pin
int sensorPin = A0;

void setup() {
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // Initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Read the input from the sensor on analog pin 0:
  int sensorValue = analogRead(sensorPin);
  
  // Print out the value to the Serial Monitor:
  Serial.println(sensorValue);

  // Define a threshold for when to turn the LED on/off
  if (sensorValue > 180) {  // Adjust this value based on your sensor's range
    digitalWrite(ledPin, HIGH);  // Turn the LED ON
  } else {
    digitalWrite(ledPin, LOW);   // Turn the LED OFF
  }

    delay(100);// Delay for stability
}
