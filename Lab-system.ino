#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Fingerprint.h>
#include <SD.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define FINGERPRINT_SENSOR_ADDRESS 0x70
#define ENROLLMENT_PIN 5
#define SDA_PIN 21  // Change according to your wiring
#define SCL_PIN 22  // Change according to your wiring
#define SSID "your_ssid"  // Replace with your Wi-Fi SSID
#define PASSWORD "your_password"  // Replace with your Wi-Fi password
#define PHP_SERVER "http://your_php_server_address/php_script.php"  // Replace with your PHP script URL

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Wire);
File fingerprintFile;

Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("Adafruit Fingerprint sensor test");

  pinMode(ENROLLMENT_PIN, INPUT);

  connectToWiFi();

  if (!finger.begin(FINGERPRINT_SENSOR_ADDRESS)) {
    Serial.println("Couldn't find fingerprint sensor :(");
    while (1);
  }
  Serial.println("Found fingerprint sensor!");

  Serial.println("Initializing SD card...");

  if (!SD.begin(SS)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }
  Serial.println("SD card initialized successfully!");

  Serial.println("Initializing OLED display...");
  if (!display.begin(SSD1306_I2C_ADDRESS, SDA_PIN, SCL_PIN)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();

  Serial.println("Ready for operation...");
}

void loop() {
  int enrollmentPinState = digitalRead(ENROLLMENT_PIN);

  if (enrollmentPinState == HIGH) {
    enrollFingerprint();
  } else {
    searchFingerprint();
  }

  delay(1000);
}

void connectToWiFi() {
  Serial.println("Connecting to Wi-Fi...");

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }

  Serial.println("Connected to Wi-Fi");
}

void enrollFingerprint() {
  Serial.println("Waiting for a valid finger to enroll...");

  if (finger.getImage()) {
    Serial.println("Image taken");
    if (finger.image2Tz(1) == FINGERPRINT_OK) {
      Serial.println("Image converted");
      if (finger.createModel() == FINGERPRINT_OK) {
        Serial.println("Fingerprints matched!");

        // Prompt the user to enter their roll number
        String rollNumber='ffffffff';
        if (finger.storeModel(1) == FINGERPRINT_OK) {
          Serial.println("Fingerprint enrolled successfully!");

          // Open the SD card file for writing
          fingerprintFile = SD.open("fingerprint_data.txt", FILE_WRITE);

          if (fingerprintFile) {
            // Write roll number and fingerprint to the SD card
            fingerprintFile.print(rollNumber + ", ");
            fingerprintFile.println(finger.downloadModel(1));
            fingerprintFile.close();
            Serial.println("Fingerprint and roll number saved to SD card!");

            // Display the roll number on the OLED display
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0, 0);
            display.println("Roll Number:");
            display.setTextSize(2);
            display.setCursor(0, 16);
            display.println(rollNumber);
            display.display();
          } else {
            Serial.println("Error opening file on SD card!");
          }

          // Access PHP script to get name and system number
          String phpURL = PHP_SERVER + "?roll=" + rollNumber;
          HTTPClient http;

          Serial.print("Accessing PHP script: ");
          Serial.println(phpURL);

          http.begin(phpURL);
          int httpCode = http.GET();

          if (httpCode > 0) {
            if (httpCode == HTTP_CODE_OK) {
              String payload = http.getString();
              Serial.println("PHP script response: " + payload);

              // Parse the response and display name and system number on the OLED display
              // Modify this part according to the response format from your PHP script
              display.setTextSize(1);
              display.setCursor(0, 32);
              display.println("Name: " + payload.substring(0, payload.indexOf(',')));
              display.setCursor(0, 48);
              display.println("System: " + payload.substring(payload.indexOf(',') + 1));
              display.display();
            }
          } else {
            Serial.println("Error accessing PHP script");
          }

          http.end();
        } else {
          Serial.println("Error storing fingerprint");
        }
      } else {
        Serial.println("Error creating model");
      }
    } else {
      Serial.println("Error converting image");
    }
  }
}

void searchFingerprint() {
  Serial.println("Waiting for a finger to search...");

  if (finger.getImage()) {
    Serial.println("Image taken");
    if (finger.image2Tz(1) == FINGERPRINT_OK) {
      Serial.println("Image converted");
      int found = finger.fingerFastSearch();

      if (found == FINGERPRINT_OK) {
        Serial.println("Fingerprint found!");
        Serial.println("User details:");

        // Open the SD card file for reading
        fingerprintFile = SD.open("fingerprint_data.txt", FILE_READ);

        if (fingerprintFile) {
          // Read and print user details
          while (fingerprintFile.available()) {
            String line = fingerprintFile.readStringUntil('\n');
            Serial.println(line);

            // Extract and display the roll number on the OLED display
            int commaIndex = line.indexOf(',');
            if (commaIndex != -1) {
              String rollNumber = line.substring(0, commaIndex);
              display.clearDisplay();
              display.setTextSize(2);
              display.setTextColor(SSD1306_WHITE);
              display.setCursor(0, 0);
              display.println("Matched Roll:");
              display.setTextSize(2);
              display.setCursor(0, 16);
              display.println(rollNumber);
              display.display();
            }

            // Access PHP script to get name and system number
            String phpURL = PHP_SERVER + "?roll=" + rollNumber;
            HTTPClient http;

            Serial.print("Accessing PHP script: ");
            Serial.println(phpURL);

            http.begin(phpURL);
            int httpCode = http.GET();

            if (httpCode > 0) {
              if (httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println("PHP script response: " + payload);

                // Parse the response and display name and system number on the OLED display
                // Modify this part according to the response format from your PHP script
                display.setTextSize(1);
                display.setCursor(0, 32);
                display.println("Name: " + payload.substring(0, payload.indexOf(',')));
                display.setCursor(0, 48);
                display.println("System: " + payload.substring(payload.indexOf(',') + 1));
                display.display();
              }
            } else {
              Serial.println("Error accessing PHP script");
            }

            http.end();
          }
          fingerprintFile.close();
        } else {
          Serial.println("Error opening file on SD card!");
        }
      } else {
        Serial.println("Fingerprint not found");
      }
    } else {
      Serial.println("Error converting image");
    }
  }
}
