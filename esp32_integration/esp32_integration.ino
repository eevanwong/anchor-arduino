#include <Arduino.h>
#include <esp_sleep.h>
#include "nfc_handler.h"
#include "wifi_handler.h"
#include "motor_handler.h"
#include "light_handler.h"
#include "buzzer_handler.h"

void setup() {
  Serial.begin(115200);
  // Connect to WiFi.
  connectToNetwork();
  initializeNFC();
  setupMotor();  
  setupBuzzer();
  initializeLED();

  // Configure external wake-up
  esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO, LOW); // 0 = Low, 1 = High

  // Attempt to read a card UID - Assume tag is Mifare Classic Card
  uint8_t uid[7] = {0};   // Buffer to store the UID
  uint8_t uidLength = 0;  // Will be set by readNFCCard()
  bool card_detected = readCardUID(uid, uidLength);
  
  if (card_detected) {
    uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    uint8_t block_authenticated = pn532.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
    if (block_authenticated) {
      // Trigger light LED - yellow -> loading
//      Serial.println(F("Authenticated block 0 (Sector 0) successfully!"));
      Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
      uint8_t data[14];

      tapCardTune();

      // Try to read the contents of block 4
      uint8_t block_read = pn532.mifareclassic_ReadDataBlock(4, data);

      if (block_read) {
        pn532.inRelease(0); // nfc will not read past this LOC

        Serial.println(F("Reading Block 0:"));
        pn532.PrintHexChar(data, 14);
        Serial.println();

        // convert to a string
        char emailStr[15];  // 14 bytes for data + 1 for the null terminator
        memcpy(emailStr, data, 14);
        emailStr[14] = '\0';  // Null-terminate the string
        String email(emailStr);  // Convert to Arduino String object, if desired
        
        /// HERE -> WIFI REQUEST BASED ON LOCK STATE
        LockResponse res;
        
        if (LOCK_STATE == UNLOCKED) {
          Serial.println(F("Locking Bike at Rack 1..."));
          LockRequest req = {1, "test", email, "test"};
          res = lock(req);
    
          if (res.error == "") {
            // Serial.printf(F("Rack ID: %d\n"), res.rack_id);
            // Serial.printf(F("User ID: %d\n"), res.user_id);
            Serial.printf(F("Lock Success: %s\n"), res.lock_success ? "true" : "false");
            LOCK_STATE = LOCKED;
            motor_lock();
            successTune();
          }
        } else {
          Serial.println(F("Unlocking Bike at Rack 1..."));
          UnlockRequest req = {1, "test", email, "test"};
          UnlockResponse res = unlock(req);
          if (res.error == "") {
            // Serial.printf(F("Rack ID: %d\n"), res.rack_id);
            // Serial.printf(F("User ID: %d\n"), res.user_id);
            Serial.printf(F("Unlock Success: %s\n"), res.unlock_success ? "true" : "false");
            LOCK_STATE = UNLOCKED;
            motor_unlock();
            successTune();
          }
        }

        if (res.error != "") {
          Serial.printf("ERROR :%s \n", res.error);
          failureTune();
        }
        
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

  // Sstart detection again (will not finish before deep sleep)
  if (startCardDetection()) {
    Serial.println(F("Started passive target detection"));
  } else {
    Serial.println(F("Failed to start passive target detection"));
  }

  // Go to sleep
  Serial.println("Going to sleep now...");
  Serial.flush();
  delay(500);

  esp_deep_sleep_start();
}

void loop() {
  // Empty. We never get here because we deep sleep in setup().
}
