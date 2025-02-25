#ifndef NFC_HANDLER_H
#define NFC_HANDLER_H

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>

// Declare NFC-related objects
extern PN532_I2C pn532i2c;
//extern NfcAdapter nfcAdapter;
extern PN532 pn532temp;

// Function declarations
void initializeNFC();
bool readCardUID(uint8_t* uid, uint8_t& uidLength);
bool startCardDetection();

// Data structure to hold NFC credentials.
struct NFCCredentials {
  String user_name;
  String user_email;
  String user_phone;
};

#endif // NFC_HANDLER_H
