#include <Arduino.h>
#include "nfc_handler.h"

// Define NFC-related objects
PN532_I2C pn532i2c(Wire);
//NfcAdapter nfcAdapter(pn532i2c);
PN532 pn532temp(pn532i2c);

// Function to initialize NFC
void initializeNFC() {
    pn532temp.begin();

    uint32_t versiondata = pn532temp.getFirmwareVersion();
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

    pn532temp.setPassiveActivationRetries(0xFF); // Ensure reliable activation
    pn532temp.SAMConfig();                       // Configure the SAM
}

// Function to read a card's UID
bool readCardUID(uint8_t* uid, uint8_t& uidLength) {
    // If the NFC is not ready, we might consider placing the device into sleep here,
    // or using other esp_sleep_* APIs for power saving. For now, it just reads the card.
    // Display some basic information about the card
    return pn532temp.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
}

// Function to start NFC card detection
bool startCardDetection() {
    return pn532temp.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
}
