/*
  Lab 9: Computer Hardware and Tiny Machine Learning
  Completed By Ken Broden
  CSCI 3412 - University of Minnesota Morris

  This version allows the board to blink via typing "yes" or "no" in 
  the serial monitor.
*/

#include <Arduino_LSM9DS1.h>
#include <Arduino_APDS9960.h>
#include <PDM.h>

// Pins for LEDs
const int redLed = LEDR;
const int blueLed = LEDB;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU");
    while (1);
  }

  // Initialize LEDs
  pinMode(redLed, OUTPUT);
  pinMode(blueLed, OUTPUT);

  // Initialize PDM (microphone)
  if (!PDM.begin(1, 16000)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }

  // Set the gain
  PDM.setGain(40);

  // Set the buffer size
  PDM.setBufferSize(256);

  Serial.println("Keyword Spotting with Nano 33 BLE");
}

void loop() {
  // Simulated keyword spotting
  String command = listenForKeyword();

  if (command == "yes") {
    digitalWrite(redLed, HIGH);
    digitalWrite(blueLed, LOW);
    Serial.println("yes");
  } else if (command == "no") {
    digitalWrite(redLed, LOW);
    digitalWrite(blueLed, HIGH);
    Serial.println("no");
  } else {
    digitalWrite(redLed, LOW);
    digitalWrite(blueLed, LOW);
  }

  delay(1000); // Delay to simulate processing time
}

String listenForKeyword() {
  // Simulate listening for keywords "yes" or "no"
  // Replace this with actual keyword spotting code
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    return command;
  }
  return "";
}