#include <Arduino.h>
#include <BleKeyboard.h>
#include "esp_sleep.h"

BleKeyboard bleKeyboard;

const int buttonPin = 2;
int buttonState = HIGH;
int lastButtonState = HIGH;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

unsigned long lastPressTime = 0;
bool waitingForSecondPress = false;
const unsigned long doublePressDelay = 400;
unsigned long lastActivityTime = 0;

const unsigned long sleepTimeout = 15 * 60 * 1000; // 15 minutes

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  bleKeyboard.begin();

  // Setup wakeup from GPIO (button press)
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_2, 0); // Wake up on LOW level from GPIO2

  // Check if woke from deep sleep
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
    Serial.println("Woke up from button press.");
  } else {
    Serial.println("Normal startup.");
  }

  lastActivityTime = millis();
}

void loop() {
  if (!bleKeyboard.isConnected()) return;

  int reading = digitalRead(buttonPin);

  // Debounce
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && buttonState == HIGH) {
      lastActivityTime = millis(); // Reset inactivity timer

      if (waitingForSecondPress && (millis() - lastPressTime) < doublePressDelay) {
        Serial.println("Double Press: Sending UP Arrow");
        bleKeyboard.write(KEY_UP_ARROW);
        waitingForSecondPress = false;
      } else {
        lastPressTime = millis();
        waitingForSecondPress = true;
      }
    }
    buttonState = reading;
  }

  // If no second press within allowed time
  if (waitingForSecondPress && (millis() - lastPressTime) >= doublePressDelay) {
    Serial.println("Single Press: Sending Down Arrow");
    bleKeyboard.write(KEY_DOWN_ARROW);
    waitingForSecondPress = false;
  }

  // Enter deep sleep if idle for 15 minutes
  if ((millis() - lastActivityTime) > sleepTimeout) {
    Serial.println("No activity for 15 minutes. Going to deep sleep...");
    bleKeyboard.end(); // stop BLE before sleeping
    delay(100); // Allow time for BLE disconnect
    esp_deep_sleep_start(); // Enter deep sleep
  }

  lastButtonState = reading;
}
