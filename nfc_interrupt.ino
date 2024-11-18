#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <ArduinoLowPower.h>
PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

#define PN532_IRQ_PIN 2  // IRQ connected to digital pin 2

volatile bool cardDetected = false;

// Interrupt Service Routine for IRQ pin
void cardInterrupt() {
    Serial.println("Interrupt activated");
    // activate handler
    // authenticate the card
    // send card data to service via esp32 web service
    // receive state (unlock vs lock)
    // if occuppied 
    cardDetected = true;
}

void setup() {
    Serial.begin(9600);
    Serial.println("NFC Reader Initialized");

    // Start NFC reader
    nfc.begin();

    // Check if NFC reader is present
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.println("PN53x board not found");
        while (1);  // Halt if NFC module is not found
    }

    // Configure the NFC module in "SAM" mode (page 89 of the manual)
    nfc.SAMConfig();

    // Set up IRQ pin as input
    pinMode(PN532_IRQ_PIN, INPUT_PULLUP);

    // Attach interrupt on IRQ pin, trigger on FALLING (when a card is detected)
    LowPower.attachInterruptWakeup(digitalPinToInterrupt(PN532_IRQ_PIN), cardInterrupt, FALLING);

    // Start detection for the first card
    nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
}

void loop() {
    if (cardDetected) {
        Serial.println("Card detected!");

        uint8_t success;
        uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer for UID
        uint8_t uidLength;  // Length of the UID

        // Read the UID of the card
        success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

        if (success) {
            Serial.println("Found an NFC card:");
            Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
            Serial.print("UID Value: ");
            for (int i = 0; i < uidLength; i++) {
                Serial.print(uid[i], HEX); Serial.print(" ");
            }
            Serial.println();
        } else {
            Serial.println("No card detected, re-enabling detection.");
        }

        // Re-enable card detection after reading
        nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);

        // Reset the cardDetected flag
        cardDetected = false;
    }
    Serial.println("Card not detected!");

}