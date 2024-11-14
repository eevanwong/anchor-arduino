//#include <Wire.h>
//#include <PN532_I2C.h>
//#include <NfcAdapter.h>
//
//#define TNF_WELL_KNOWN 0x01
//
//// Initialize the PN532 using the I2C interface
//PN532_I2C pn532i2c(Wire);
//NfcAdapter nfcAdapter(pn532i2c); // Pass the PN532_I2C interface to NfcAdapter
//
//void setup(void) {
//  Serial.begin(115200);
//  Serial.println("NFC Reader Initializing...");
//
//  nfcAdapter.begin(); // Initialize the NfcAdapter
//
//  Serial.println("Waiting for an NFC card with NDEF data...");
//}
//
//void loop(void) {
//  if (nfcAdapter.tagPresent()) {
//    NfcTag tag = nfcAdapter.read();
//
//    Serial.println("Found a card!");
//    Serial.print("UID: ");
//    tag.print(); // Print the UID
//    Serial.print("Type: ");
//    Serial.println(tag.getTagType()); // Print the tag type
//
//    if (tag.hasNdefMessage()) {
//      Serial.println("NDEF Message found:");
//      NdefMessage message = tag.getNdefMessage();
//
//      for (int i = 0; i < message.getRecordCount(); i++) {
//        NdefRecord record = message.getRecord(i);
//        int payloadLength = record.getPayloadLength();
//        byte payload[payloadLength];
//        record.getPayload(payload);
//
//        Serial.print("Record "); Serial.print(i); Serial.print(": ");
//
//        // Handle Text Records (TNF_WELL_KNOWN with type 'T')
//        if (record.getTnf() == TNF_WELL_KNOWN && record.getType()[0] == 'T') {
//          // Text record payload parsing
//          byte statusByte = payload[0];
//          int languageCodeLength = statusByte & 0x3F; // Bits 5..0 contain the length of the language code
//          int textLength = payloadLength - 1 - languageCodeLength;
//          Serial.print("Text: ");
//
//          for (int j = 1 + languageCodeLength; j < payloadLength; j++) {
//            Serial.print((char)payload[j]);
//          }
//          Serial.println();
//        }
//        // Handle URI Records (TNF_WELL_KNOWN with type 'U')
//        else if (record.getTnf() == TNF_WELL_KNOWN && record.getType()[0] == 'U') {
//          // URI record payload parsing
//          byte identifierCode = payload[0];
//          Serial.print("URI: ");
//
//          // Mapping of identifier codes to URI prefixes (per NDEF spec)
//          const char* uriPrefixes[] = {
//            "", "http://www.", "https://www.", "http://", "https://",
//            "tel:", "mailto:", "ftp://anonymous:anonymous@", "ftp://ftp.", "ftps://",
//            "sftp://", "smb://", "nfs://", "ftp://", "dav://", "news:",
//            "telnet://", "imap:", "rtsp://", "urn:", "pop:", "sip:", "sips:",
//            "tftp:", "btspp://", "btl2cap://", "btgoep://", "tcpobex://",
//            "irdaobex://", "file://", "urn:epc:id:", "urn:epc:tag:",
//            "urn:epc:pat:", "urn:epc:raw:", "urn:epc:", "urn:nfc:"
//          };
//
//          String uriPrefix = "";
//          if (identifierCode < sizeof(uriPrefixes) / sizeof(uriPrefixes[0])) {
//            uriPrefix = uriPrefixes[identifierCode];
//          }
//
//          Serial.print(uriPrefix);
//
//          for (int j = 1; j < payloadLength; j++) {
//            Serial.print((char)payload[j]);
//          }
//          Serial.println();
//        }
//        // Handle other record types
//        else {
//          Serial.print("Unsupported record type");
//          Serial.println();
//        }
//      }
//    } else {
//      Serial.println("No NDEF Message found on this tag.");
//    }
//  } else {
//    // Serial.println("Waiting for an NFC card...");
//  }
//
//  delay(1000); // Delay for a second before retrying
//}
#if 0
#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_SPI pn532spi(SPI, 10);
NfcAdapter nfc = NfcAdapter(pn532spi);
#else

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
#endif

void setup(void) {
    Serial.begin(9600);
    Serial.println("   Reader");
    nfc.begin();
}

void loop(void) {
    Serial.println("\nScan a NFC tag\n");
    if (nfc.tagPresent())
    {
        NfcTag tag = nfc.read();
        tag.print();
    }
    delay(5000);
}