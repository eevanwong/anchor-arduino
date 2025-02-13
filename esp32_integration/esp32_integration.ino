#include <Arduino.h>
#include "nfc_handler.h"
#include "wifi_handler.h"
#include "motor_handler.h"
#include "light_handler.h"
#include <esp_sleep.h>

// ------------------------------------------
// Constants & Global Variables
// ------------------------------------------
#define WAKEUP_GPIO GPIO_NUM_2  // RTC IO pin for wake-up

uint8_t LOCK_STATE = 0; // 0 - not locked
// ------------------------------------------
// Setup Function
// ------------------------------------------
void setup() {
  Serial.begin(115200);

  // Connect to WiFi.
  connectToNetwork();
  
  // Configure wake-up pin
  pinMode(WAKEUP_GPIO, INPUT);  // For debugging
  
  // Initialize NFC and LEDs
  initializeNFC();
  initializeLED();

  // Configure external wake-up
  esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO, LOW); // 0 = Low, 1 = High

  // Attempt to read a card UID - Assume tag is Mifare Classic Card
  uint8_t uid[7] = {0};   // Buffer to store the UID
  uint8_t uidLength = 0;  // Will be set by readNFCCard()
  bool success = readCardUID(uid, uidLength);
  yellowOn();
  if (success) {
//    Serial.println(F("Found an ISO14443A card"));
//    Serial.print(F("  UID Length: "));Serial.print(F(uidLength), DEC));Serial.println(F(" bytes"));
//    Serial.print(F("  UID Value: "));
//    pn532temp.PrintHex(uid, uidLength);
    uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    success = pn532temp.mifareclassic_AuthenticateBlock(uid, uidLength, 0, 0, keya);
    if (success) {
      // Trigger light LED - yellow -> loading
      Serial.println(F("Authenticated block 0 (Sector 0) successfully!"));
      Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
      uint8_t data[16];
        
      // Try to read the contents of block 4
      success = pn532temp.mifareclassic_ReadDataBlock(0, data);
    
      if (success) {
        pn532temp.inRelease(0);

        // Serial.println(F("Reading Block 0:"));
        pn532temp.PrintHexChar(data, 16);
        Serial.println();
                  
        /// HERE -> WIFI REQUEST BASED ON LOCK STATE
        LockResponse res;
        if (LOCK_STATE == 0) {
          Serial.println(F("Locking Bike at Rack 1..."));
          LockRequest req = {1, "John Doe", "johndoe@gmail.com", "1234561234"};
          res = lock(req);
    
          if (res.error == "") {
            // Serial.printf(F("Rack ID: %d\n"), res.rack_id);
            // Serial.printf(F("User ID: %d\n"), res.user_id);
            Serial.printf(F("Lock Success: %s\n"), res.lock_success ? "true" : "false");
            LOCK_STATE = 1;
            motor_lock();
          }
        } else {
          UnlockRequest req = {1, "John Doe", "johndoe@gmail.com", "1234561234"};
          res = unlock(req);
          if (res.error == "") {
            // Serial.printf(F("Rack ID: %d\n"), res.rack_id);
            // Serial.printf(F("User ID: %d\n"), res.user_id);
            Serial.printf(F("Unlock Success: %s\n"), res.unlock_success ? "true" : "false");
            LOCK_STATE = 0;
            motor_unlock();
          }
        }

        /// WIFI REQUEST COMPONENT END
        // if err -> red light flashes
        if (res.error == "") {
          toggleGreen();
        } else {
          toggleRed();
        }
        
        delay(1000);
      } else {
        // at any failure activate red light (should be a function)
        Serial.println(F("Read failed"));
      }
    } else {
      Serial.println(F("Authentication failed"));
    }
  } else {
    Serial.println(F("Card not found"));
  }
  yellowOff();

  // Sstart detection again (will not finish before deep sleep)
  if (startCardDetection()) {
    Serial.println(F("Started passive target detection"));
  } else {
    Serial.println(F("Failed to start passive target detection"));
  }

  // Double-check pin state for debugging
//  wakeupPinState = digitalRead(WAKEUP_GPIO);
//  Serial.print("Post-detection WAKEUP_GPIO state: ");
//  Serial.println(wakeupPinState);

  // Go to sleep
  Serial.println("Going to sleep now...");
  esp_deep_sleep_start();

  // This line should not execute after deep_sleep
}

// ------------------------------------------
// Loop Function
// ------------------------------------------
void loop() {
  // Empty. We never get here because we deep sleep in setup().
}
