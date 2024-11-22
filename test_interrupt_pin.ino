#include <LowPower.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);
const int interruptPin = 2; // Pin connected to PN532 IRQ
volatile bool interruptOccurred = false;

void setup() {
  Serial.begin(38400);
  while (!Serial); // Wait for Serial port to be ready (optional)

  pinMode(interruptPin, INPUT_PULLUP); // Configure pin as input with pull-up resistor

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("PN53x board not found");
    while (1); // Halt
  }
  
  // Attach interrupt to detect changes on the interrupt pin
  attachInterrupt(digitalPinToInterrupt(interruptPin), interruptHandler, FALLING);
  
  // Configure the PN532 to generate an interrupt when a card is detected
  nfc.SAMConfig();

  // Start passive target detection -> needs to start being aware that it could be reading cards
  nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);  
  Serial.println("Monitoring interrupt pin for changes...");
}

void loop() {
  // Read the state of the interrupt pin
  int irqState = digitalRead(interruptPin);
//  Serial.print("Pin state changed: ");
//  Serial.println(String(pinStateChanged));
  Serial.print("Interrupt pin state: ");
  Serial.println(irqState == HIGH ? "HIGH" : "LOW");
  if (interruptOccurred) {
    Serial.println("Interrupt Occurred!");
    
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
  
//    attachInterrupt(digitalPinToInterrupt(wakeUpPin), wakeUp, FALLING);

    interruptOccurred = false;
    Serial.print("Now Interrupted Occurred: ");
    Serial.println(String(interruptOccurred));
  }
  // Add a delay to make the output readable
  delay(500);
}

// Interrupt Service Routine
void interruptHandler() {
  interruptOccurred = true;
  return;
}
