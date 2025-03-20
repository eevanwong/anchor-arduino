#include <Arduino.h>
#include "nfc_handler.h"

// Define NFC-related objects
PN532_I2C pn532i2c(Wire);
//NfcAdapter nfcAdapter(pn532i2c);
PN532 pn532(pn532i2c);

// Function to initialize NFC
void initializeNFC() {
    pn532.begin();

    uint32_t versiondata = pn532.getFirmwareVersion();
    if (!versiondata) {
        Serial.println("Didn't find PN53x board");
        while (1); // Halt if no NFC module is found
    }

    Serial.print("Found chip PN5");
    Serial.println((versiondata >> 24) & 0xFF, HEX);
    Serial.print("Firmware ver. ");
    Serial.print((versiondata >> 16) & 0xFF, DEC);
    Serial.print('.');
    Serial.println((versiondata >> 8) & 0xFF, DEC);

    pn532.setPassiveActivationRetries(0xFF); // Ensure reliable activation
    pn532.SAMConfig();                       // Configure the SAM
    pinMode(WAKEUP_GPIO, INPUT);  // For interrupt
}

// Function to read a card's UID
bool readCardUID(uint8_t* uid, uint8_t& uidLength) {
    // If the NFC is not ready, we might consider placing the device into sleep here,
    // or using other esp_sleep_* APIs for power saving. For now, it just reads the card.
    // Display some basic information about the card
    return pn532.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
}

// Function to start NFC card detection
bool startCardDetection() {
    return pn532.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
}


String uint8ArrayToHexString(uint8_t* data, size_t len) {
  String hexString = "";  // Initialize an empty string to store the result
  for (size_t i = 0; i < len; i++) {
    // Convert each byte to a 2-digit hexadecimal string and append to the result string
    hexString += String(data[i], HEX);  // HEX format ensures it's hexadecimal
  }
  return hexString;
}
