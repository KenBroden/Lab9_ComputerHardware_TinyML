/*
  Lab 9: Computer Hardware and Tiny Machine Learning
  Completed By Ken Broden
  CSCI 3412 - University of Minnesota Morris

  Requires the Arduino_LSM9DS1 library
  Requires Adafruit TensorFlow Lite library
*/

#include <Arduino_LSM9DS1.h>
#include <TensorFlowLite.h>
#include "yes_no_model.h"

// Pins for LEDs
const int redLed = LEDR;
const int blueLed = LEDB;

// TensorFlow Lite globals
tflite::MicroErrorReporter tflErrorReporter;
TfLiteTensor *input, *output;
tflite::MicroInterpreter *interpreter;
uint8_t tensorArena[20 * 1024];

// Setup function
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

  // Load the TensorFlow Lite model
  const tflite::Model* model = tflite::GetModel(yes_no_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema version does not match!");
    while (1);
  }

  // Create the interpreter
  static tflite::MicroMutableOpResolver<5> microOpResolver;
  microOpResolver.AddBuiltin(tflite::BuiltinOperator_DEPTHWISE_CONV_2D, tflite::ops::micro::Register_DEPTHWISE_CONV_2D());
  microOpResolver.AddBuiltin(tflite::BuiltinOperator_FULLY_CONNECTED, tflite::ops::micro::Register_FULLY_CONNECTED());
  microOpResolver.AddBuiltin(tflite::BuiltinOperator_SOFTMAX, tflite::ops::micro::Register_SOFTMAX());
  microOpResolver.AddBuiltin(tflite::BuiltinOperator_RESHAPE, tflite::ops::micro::Register_RESHAPE());

  static tflite::MicroInterpreter staticInterpreter(model, microOpResolver, tensorArena, sizeof(tensorArena), &tflErrorReporter);
  interpreter = &staticInterpreter;

  // Allocate memory for the model's input and output tensors
  interpreter->AllocateTensors();

  input = interpreter->input(0);
  output = interpreter->output(0);

  Serial.println("Keyword Spotting with Nano 33 BLE");
}

// Main loop function
void loop() {
  listenForKeyword();
  delay(100);
}

// Function to simulate listening for keywords
void listenForKeyword() {
  // Simulated microphone input (replace with actual microphone input processing)
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    // Fill the input tensor with the simulated command
    for (int i = 0; i < command.length(); i++) {
      input->data.int8[i] = command[i];
    }

    // Run the model
    TfLiteStatus invokeStatus = interpreter->Invoke();
    if (invokeStatus != kTfLiteOk) {
      Serial.println("Error invoking the interpreter");
      return;
    }

    // Get the output from the model
    int8_t yesScore = output->data.int8[0];
    int8_t noScore = output->data.int8[1];

    // Determine if the keyword is "yes" or "no"
    if (yesScore > noScore) {
      digitalWrite(redLed, HIGH);
      digitalWrite(blueLed, LOW);
      Serial.println("yes");
    } else {
      digitalWrite(redLed, LOW);
      digitalWrite(blueLed, HIGH);
      Serial.println("no");
    }
  }
}
