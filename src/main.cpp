#include <Arduino.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

const int buttonPin = 2; // Use digital pin 2
int buttonState = HIGH;
int lastButtonState = HIGH;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

unsigned long lastPressTime = 0;
bool waitingForSecondPress = false;
const unsigned long doublePressDelay = 400; // Max time between presses (in ms)

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP); // Button with internal pull-up
  bleKeyboard.begin();
  Serial.println("BLE Keyboard ready");
}

void loop() {
  if (!bleKeyboard.isConnected()) return;

  int reading = digitalRead(buttonPin);

  // Debounce the button
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && buttonState == HIGH) {
      // Button was just pressed
      if (waitingForSecondPress && (millis() - lastPressTime) < doublePressDelay) {
        // Double press detected
        Serial.println("Double Press: Sending Down Arrow");
        bleKeyboard.write(KEY_DOWN_ARROW);
        waitingForSecondPress = false;
      } else {
        // First press - start waiting for a second press
        lastPressTime = millis();
        waitingForSecondPress = true;
      }
    }
    buttonState = reading;
  }

  // Check for timeout of second press window
  if (waitingForSecondPress && (millis() - lastPressTime) >= doublePressDelay) {
    // Only one press - treat as single press
    Serial.println("Single Press: Sending Up Arrow");
    bleKeyboard.write(KEY_UP_ARROW);
    waitingForSecondPress = false;
  }

  lastButtonState = reading;
}
