#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <esp_sleep.h>

#define WAKEUP_GPIO GPIO_NUM_2 // RTC IO pin for wakeup
PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

RTC_DATA_ATTR int bootCount = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello!");
  pinMode(WAKEUP_GPIO, INPUT); // Set the wakeup GPIO as input for debugging
  bootCount++;
  Serial.println("Boot num:" + String(bootCount));

  nfc.begin();
//  print_wakeup_reason();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }

  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();


  esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO, LOW); // 0 = Low, 1 = High
  delay(1000);
  
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  int wakeupPinState = digitalRead(WAKEUP_GPIO); // Read the state of the wakeup pin
  Serial.print("WAKEUP_GPIO state: ");
  Serial.println(wakeupPinState); // Debugging output
//  print_wakeup_reason();
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  if (success) {
    Serial.println("Found a card!");
    Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
    Serial.print("UID Value: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x"); Serial.print(uid[i], HEX);
    }
    Serial.println("");
    delay(100);
  } else {
    Serial.println("Timed out waiting for a card");
  }

    // Start detection again
  if (nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)) {
      Serial.println("Started passive target detection");
  } else {
      Serial.println("Failed to start passive target detection");
  }
  
  wakeupPinState = digitalRead(WAKEUP_GPIO); // Read the state of the wakeup pin
  Serial.print("Post: WAKEUP_GPIO state: ");
  Serial.println(wakeupPinState); // Debugging output
  
  // Go to sleep now
  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
  Serial.println("Waiting for an ISO14443A card");
}

void loop() {
}