# Assignment 2 - Update Plan

## Brief Plan to Update the TinyML Board to Recognize New Words

### Step 1: Update `micro_speech.ino`

Modify the `RespondToCommand()` method to handle the new keywords “up” and “down.” The updated method would look like this:

    ```c
    void RespondToCommand(tflite::ErrorReporter * error_reporter,
                            int32_t current_time, const char* found_command,
                            uint8_t score, bool is_new_command) {
        if (is_new_command) {
        if (strcmp(found_command, "up") == 0) {
            digitalWrite(RED_LED_PIN, HIGH);
            digitalWrite(BLUE_LED_PIN, LOW);
            Serial.println("up");
        } else if (strcmp(found_command, "down") == 0) {
            digitalWrite(RED_LED_PIN, LOW);
            digitalWrite(BLUE_LED_PIN, HIGH);
            Serial.println("down");
        } else {
            digitalWrite(RED_LED_PIN, LOW);
            digitalWrite(BLUE_LED_PIN, LOW);
        }
        }
    }
    ```

### Step 2: Configure LED Pins

Ensure that the LED pins are configured as outputs in the `setup()` method. This is necessary for controlling the LEDs in response to the recognized commands.

### Step 3: Retrain the Neural Network

Focus on retraining the neural network to recognize the new keywords. This involves:

- Training a new model in TensorFlow for the words "up" and "down"
- Exporting it as a `.tflite` file
- Quantizing the model during the TensorFlow Lite conversion process to optimize it for the TinyML development board

### Step 4: Convert and Replace Model

Convert the `.tflite` file into a C array using the `xxd -i` command and replace the `g_model` in `micro_features_model.cpp`.

### Step 5: Upload and Test

Upload the updated code to the Nano 33 BLE board and test it by speaking “up” and “down,” observing the response of the LEDs to verify functionality.
