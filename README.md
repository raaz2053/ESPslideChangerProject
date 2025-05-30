# BLE Slide chager with Smart Sleep and single Button Control (ESP32)

This project implements a **BLE (Bluetooth Low Energy) HID Keyboard** using an **ESP32**, with **smart power-saving** and **custom button interaction**:

- **Single button press** sends an **Down Arrow** key to go to next slide.
- **Double press** sends a **Up Arrow** key to go to previous slide.
- **Automatically enters deep sleep** after 15 minutes of inactivity to save battery.
- **Wakes up on button press** using GPIO interrupt.

## 🧠 Features

- ✅ BLE HID keyboard using `BleKeyboard` library.
- ✅ Debounced single/double press logic.
- ✅ Power-efficient deep sleep after inactivity.
- ✅ Wake on button press (via `ext0` interrupt).
- ✅ Ideal for custom remote control or assistive devices.

## 🔧 Hardware Requirements

- **ESP32** board (with BLE capability)
- **Push button**
- **Basic wiring:**
  - Button connected between **GPIO 2** and **GND**

## 🔌 Wiring
Simply put a push button between PIN2 of ESP32 and GND pin.

## 📦 Libraries Used

- [`BleKeyboard`](https://github.com/T-vK/ESP32-BLE-Keyboard) by T-vK

## 🕹 Usage

- **Single press** → Sends `KEY_DOWN_ARROW`
- **Double press (within 400ms)** → Sends `KEY_UP_ARROW`
- **Idle for 15 minutes** → Enters deep sleep
- **Press button again** → Wakes up

## 🌙 Power Efficiency

When inactive:
- Enters **deep sleep** mode
- Uses **GPIO 2** to wake up the ESP32 with minimal current draw
- BLE disconnects cleanly before sleep for graceful reconnection on wake

## 🚀 Getting Started

1. Clone this repo
2. Open the code in Arduino IDE or PlatformIO
3. Upload to your ESP32 board
4. Pair with a device as a Bluetooth keyboard
5. Press the button and observe the key events!

## 📜 License

MIT License — free to use, modify, and distribute.

---

Made with 💡 by Er. Raju Sharan
