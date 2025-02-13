#include <Arduino.h>     // Needed for Serial, String, etc.
#include "wifi_handler.h"
#include <AESLib.h>      // For AES encryption
#include <Base64.h>      // For base64 encode/decode

// optimizations 
#define ARDUINOJSON_DECODE_UNICODE 0
#define ARDUINOJSON_USE_LONG_LONG 0
#include <ArduinoJson.h> // For JSON handling

// ---------------------------------------------------------------------
// Private (static) helper functions
// ---------------------------------------------------------------------

// Pad the payload to match AES block size (16 bytes)
static String padPayload(String payload) {
  size_t blockSize = 16;
  size_t padding = blockSize - (payload.length() % blockSize);
  for (size_t i = 0; i < padding; i++) {
    payload += static_cast<char>(padding);
  }
  return payload;
}

// Encrypt the payload and return a Base64 string
static String encryptPayload(String payload) {
  static AES aes;
  static byte aesKey[32] = {
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
    0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34,
    0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32
  };

  static byte iv[16];
  // generate iv
  for (int i = 0; i < 16; i++) {
    iv[i] = random(0, 256);
  }

  // Copy the IV so it's not overwritten during encryption
  byte iv_copy[16];
  memcpy(iv_copy, iv, 16);

  // Pad the payload to a multiple of 16
  payload = padPayload(payload);

  // Encrypt the payload
  byte encryptedPayload[256];
  aes.do_aes_encrypt((byte*)payload.c_str(), payload.length(), encryptedPayload, aesKey, 256, iv);


  // Base64-encode the encrypted payload and the IV
  String encryptedBase64 = base64::encode(encryptedPayload, payload.length());
  String ivBase64        = base64::encode(iv_copy, sizeof(iv_copy));

  // Return them in the format "IV:EncryptedData"
  return ivBase64 + ":" + encryptedBase64;
}

// ---------------------------------------------------------------------
// Public functions (declared in wifi_handler.h)
// ---------------------------------------------------------------------


// Function to connect to WiFi
void connectToNetwork() {
  // Network credentials - scoped to this file
  static const char* ssid     = "Pixel_2809";
  static const char* password = "test123!";
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startTime > 10000) {
      Serial.println("Network connection timed out!");
      return;
    }
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nNetwork connection success!");
}

// Function to lock a bike
LockResponse lock(const LockRequest& request) {
//  Serial.print("ESP32 IP: ");
//  Serial.println(WiFi.localIP());
  
  LockResponse res = {0, 0, false, ""};

  if (WiFi.status() != WL_CONNECTED) {
    res.error = "Network connection lost!";
    Serial.println(res.error);
    return res;
  }

  // Prepare HTTP request
  HTTPClient http;
  http.begin("http://192.168.0.223:8080/api/lock");
  http.addHeader("Content-Type", "application/json");

  // Build JSON payload
  DynamicJsonDocument doc(256);
  doc["rack_id"]    = request.rack_id;
  doc["user_name"]  = request.user_name;
  doc["user_email"] = request.user_email;
  doc["user_phone"] = request.user_phone;

  String payload;
  serializeJson(doc, payload);
  String encryptedPayload = "{\"data\":\"" + encryptPayload(payload) + "\"}";

  // Send POST request
  int httpResponseCode = http.POST(encryptedPayload);
  if (httpResponseCode <= 0) {
    res.error = http.errorToString(httpResponseCode);
    Serial.printf("HTTP Error: %s\n", res.error.c_str());
    http.end();
    return res;
  }

  // Parse response
  String response = http.getString();
  if (httpResponseCode == 200) {
    DynamicJsonDocument resDoc(1024);
    DeserializationError error = deserializeJson(resDoc, response);
    if (!error) {
      res.rack_id     = resDoc["rack_id"];
      res.user_id     = resDoc["user_id"];
      res.lock_success = resDoc["lock_success"];
    } else {
      res.error = "JSON Parsing Error: " + String(error.c_str());
      Serial.println(res.error);
    }
  } else {
    res.error = response;
    Serial.println(res.error);
  }

  http.end();
  return res;
}

// Function to unlock a bike
UnlockResponse unlock(const UnlockRequest& request) {
  UnlockResponse res = {0, 0, false, ""};

  if (WiFi.status() != WL_CONNECTED) {
    res.error = "Network connection lost!";
    Serial.println(res.error);
    return res;
  }

  // Prepare HTTP request
  HTTPClient http;
  http.begin("http://192.168.0.223:8080/api/unlock");
  http.addHeader("Content-Type", "application/json");

  // Build JSON payload
  DynamicJsonDocument doc(256);
  doc["rack_id"]    = request.rack_id;
  doc["user_name"]  = request.user_name;
  doc["user_email"] = request.user_email;
  doc["user_phone"] = request.user_phone;

  String payload;
  serializeJson(doc, payload);
  String encryptedPayload = "{\"data\":\"" + encryptPayload(payload) + "\"}";

  // Send POST request
  int httpResponseCode = http.POST(encryptedPayload);
  if (httpResponseCode <= 0) {
    res.error = http.errorToString(httpResponseCode);

    #ifdef DEBUG
      Serial.printf("HTTP Error: %s\n", res.error.c_str());
    #endif
    
    http.end();
    return res;
  }

  // Parse response
  String response = http.getString();
  if (httpResponseCode == 200) {
    DynamicJsonDocument resDoc(1024);
    DeserializationError error = deserializeJson(resDoc, response);
    if (!error) {
      res.rack_id      = resDoc["rack_id"];
      res.user_id      = resDoc["user_id"];
      res.unlock_success = resDoc["unlock_success"];
    } else {
      res.error = "JSON Parsing Error: " + String(error.c_str());
      #ifdef DEBUG
        Serial.println(res.error);
      #endif
    }
  } else {
    res.error = response;

    #ifdef DEBUG
      Serial.println(res.error);
    #endif
  }

  http.end();
  return res;
}
