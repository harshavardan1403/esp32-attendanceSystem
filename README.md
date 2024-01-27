# esp32-attendanceSystem

This project demonstrates fingerprint recognition using the Adafruit Fingerprint sensor and ESP32-CAM module. The program allows users to enroll fingerprints, store them on an SD card, and search for enrolled fingerprints. Additionally, it connects to a PHP server to retrieve names associated with registered roll numbers. This works as a simple attendance system. The coise of ESP 32 cam module was made to involve develop a face recognisiton system in the future.

## Requirements

- Arduino IDE
- ESP32 board support
- Adafruit Fingerprint Sensor library
- Adafruit GFX library
- Adafruit SSD1306 library
- ArduinoHttpClient library
- ESP32WiFi library
- SD library

## Setup

1. Clone the repository:

   ```bash
   git clone https://github.com/harshavardan1403/esp32-attendanceSystem.git

- Open the esp32-cam-fingerprint-recognition.ino file in Arduino IDE.

- Install the required libraries through the Arduino IDE Library Manager.

- Adjust the following configurations in the code:

  1. Wi-Fi credentials (SSID and PASSWORD)
  2. Server address (SERVER_ADDRESS and SERVER_PORT)
  3. Pin configurations, such as ENROLLMENT_PIN, SDA_PIN, and SCL_PIN
- Upload the code to your ESP32-CAM module.

## Libraries required:
- Adafruit Fingerprint Sensor Library
  - Library for interfacing with the Adafruit Fingerprint sensor.
  - [GitHub Repository](https://github.com/adafruit/Adafruit-Fingerprint-Sensor-Library)

- Adafruit GFX Library
  - Core graphics library for all Adafruit displays.
  - [GitHub Repository](https://github.com/adafruit/Adafruit-GFX-Library)

- Adafruit SSD1306 Library
  - Library for controlling Adafruit SSD1306-based OLED displays.
  - [GitHub Repository](https://github.com/adafruit/Adafruit_SSD1306)

- ArduinoHttpClient Library
  - Library for making HTTP requests from Arduino.
  - [GitHub Repository](https://github.com/arduino-libraries/ArduinoHttpClient)

- ESP32WiFi Library
  - Core library for ESP32 Wi-Fi functionality.
  - Included with the ESP32 board support package in the Arduino IDE.
  - To add ESP32 Board support package, follow the following steps.
       1. Open Arduino IDE and Go to Tools.
       2. Click on **Manage Libraries**
       3. In the **Library manager** window, *search esp32*
       4. Install the library released by Espresso

- SD Library
  - Library for reading and writing to SD cards.
  - Included with the Arduino IDE.

## Usage
- Connect the ESP32-CAM module to power and ensure it is connected to Wi-Fi.

- Enroll a fingerprint by pressing the enrollment button (ENROLLMENT_PIN) and following the on-screen instructions.

- Search for fingerprints by releasing the enrollment button.

- View the associated roll numbers and names on the Serial Monitor.

## Known Issues
Mention any known issues or limitations of the current implementation.

## Contributing
If you'd like to contribute to this project, please fork the repository and create a pull request with your changes.
