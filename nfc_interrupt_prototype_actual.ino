// Code for testing what mode the PN532 IRQ pin is
#include <LowPower.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

const int wakeUpPin = 2; // Connect PN532 IRQ to pin 2
volatile bool cardDetected = false;


void wakeUp() {
  Serial.println("interrupt");
  cardDetected = true;
}

void setup() {
  Serial.begin(9600);
  pinMode(wakeUpPin, INPUT_PULLUP); // w pullup resistor, more stable

  // start listening for nfc
  // Start NFC reader
  nfc.begin();

  // Check if NFC reader is present
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
      Serial.println("PN53x board not found");
      while (1);  // Halt if NFC module is not found
  }

  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
  attachInterrupt(digitalPinToInterrupt(wakeUpPin), wakeUp, FALLING);

  nfc.SAMConfig();
}

void loop() {
  Serial.println("IRQ State:" + String(digitalRead(wakeUpPin)));
  Serial.println("Card Detected:" + String(cardDetected));
  
//  detachInterrupt(digitalPinToInterrupt(wakeUpPin));

// Serial.println("Entering Sleep Mode");
// Serial.flush();
//  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
//  Serial.println("Woke up from sleep");
  if (cardDetected) {
//    detachInterrupt(digitalPinToInterrupt(wakeUpPin));
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

//        Serial.println("Curr state: " + String(irqState == HIGH ? "HIGH" : "LOW"));

    } else {
        Serial.println("No card detected, re-enabling detection.");
    }

    // Start detection again
    if (nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)) {
        Serial.println("Started passive target detection");
    } else {
        Serial.println("Failed to start passive target detection");
    }
  
    Serial.println("New IRQ State:" + String(digitalRead(wakeUpPin)));
//    attachInterrupt(digitalPinToInterrupt(wakeUpPin), wakeUp, FALLING);

    cardDetected = false;
//    attachInterrupt(digitalPinToInterrupt(wakeUpPin), wakeUp, FALLING);

  }
  delay(500);
}
