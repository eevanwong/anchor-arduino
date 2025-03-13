#include <Arduino.h>
#include <esp_sleep.h>
#include "nfc_handler.h"
#include "wifi_handler.h"
#include "motor_handler.h"
#include "light_handler.h"
#include <esp_sleep.h>
#include <ezButton.h>

// ------------------------------------------
// Constants & Global Variables
// ------------------------------------------
uint8_t LOCK_STATE = 0; // 0 - not locked

ezButton limitSwitch(LIMIT_SWITCH); // Limit switch pin

// ------------------------------------------
// Setup Function
// ------------------------------------------
void setup() {
  Serial.begin(115200);
  // Connect to WiFi.
  // ConnectToNetwork();
  
  initializeNFC();
  setupMotor();  
  initializeLED();

  // Configure external wake-up
  esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO, LOW); // 0 = Low, 1 = High

  // Set limit switch debounce time.
  limitSwitch.setDebounceTime(50);

  // Attempt to read a card UID - Assume tag is Mifare Classic Card
  uint8_t uid[7] = {0};   // Buffer to store the UID
  uint8_t uidLength = 0;  // Will be set by readNFCCard()
  bool success = readCardUID(uid, uidLength);
  
  if (success) {
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
            motor_lock(limitSwitch);
          }
        } else {
          UnlockRequest req = {1, "John Doe", "johndoe@gmail.com", "1234561234"};
          UnlockResponse res = unlock(req);
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
        } else {
          Serial.println("Locking motor");
          motor_lock(limitSwitch);
          toggleRed();
        }
        toggleGreen();

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
