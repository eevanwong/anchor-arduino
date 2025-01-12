#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <AESLib.h>
#include <Base64.h>

// Network credentials.
const char* ssid = "";
const char* password = "";

AES aes;
byte aesKey[32] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
                   0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32};
byte iv[16];

struct LockRequest {
  int rack_id;
  String user_name;
  String user_email;
  String user_phone;
};

struct UnlockRequest {
  int rack_id;
  String user_name;
  String user_email;
  String user_phone;
};

struct LockResponse {
  int rack_id;
  int user_id;
  bool lock_success;
  String error;
};

struct UnlockResponse {
  int rack_id;
  int user_id;
  bool unlock_success;
  String error;
};

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.print("Starting...");

  // Connect to WiFi.
  connectToNetwork(ssid, password);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equalsIgnoreCase("LOCK")) {
      Serial.println("Locking Bike at Rack 1...");
      LockRequest req = {1, "John Doe", "johndoe@gmail.com", "1234561234"};
      LockResponse res = lock(req);

      if (res.error == "") {
        Serial.printf("Rack ID: %d\n", res.rack_id);
        Serial.printf("User ID: %d\n", res.user_id);
        Serial.printf("Lock Success: %s\n", res.lock_success ? "true" : "false");
      }
    } 
    if (command.equalsIgnoreCase("UNLOCK")) {
      Serial.println("Unlocking Bike at Rack 1...");
      UnlockRequest req = {1, "John Doe", "johndoe@gmail.com", "1234561234"};
      UnlockResponse res = unlock(req);

      if (res.error == "") {
        Serial.printf("Rack ID: %d\n", res.rack_id);
        Serial.printf("User ID: %d\n", res.user_id);
        Serial.printf("Unlock Success: %s\n", res.unlock_success ? "true" : "false");
      }
    }
  }
}

void generateIV() {
  for (int i = 0; i < 16; i++) {
    iv[i] = random(0, 256);
  }
}

String padPayload(String payload) {
  size_t blockSize = 16;
  size_t padding = blockSize - (payload.length() % blockSize);
  for (size_t i = 0; i < padding; i++) {
    payload += (char)padding;
  }
  return payload;
}

String encryptPayload(String payload) {
  generateIV();

  // Create a copy of the IV since the original IV will be modified during encryption: https://forum.arduino.cc/t/encryption-with-aeslib/1268840.
  byte iv_copy[16];
  memcpy(iv_copy, iv, 16);
  payload = padPayload(payload);

  // Encrypt payload.
  byte encryptedPayload[256];
  aes.do_aes_encrypt((byte*)payload.c_str(), payload.length(), encryptedPayload, aesKey, 256, iv);

  // base64 encode the encrypted payload.
  String encryptedBase64 = base64::encode(encryptedPayload, payload.length());

  // base64 encode the IV for server to decrypt with later.
  String ivBase64 = base64::encode(iv_copy, sizeof(iv_copy));

  return ivBase64 + ":" + encryptedBase64;
}

void connectToNetwork(const char* networkSSID, const char* networkPassword) {
  WiFi.begin(networkSSID, networkPassword);
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

LockResponse lock( const LockRequest& request) {
  LockResponse res = {0, 0, false, ""};

  if (WiFi.status() != WL_CONNECTED) {
    res.error = "Network connection lost!";
    Serial.println(res.error);
    return res;
  }
  
  HTTPClient http;

  http.begin("http://192.168.86.27:8080/api/lock");
  http.addHeader("Content-Type", "application/json");

  DynamicJsonDocument doc(256);
  doc["rack_id"] = request.rack_id;
  doc["user_name"] = request.user_name;
  doc["user_email"] = request.user_email;
  doc["user_phone"] = request.user_phone;
  
  String payload;
  serializeJson(doc, payload);
  String encryptedPayload = "{\"data\":\"" + encryptPayload(payload) + "\"}";
  // Serial.println(encryptedPayload);

  int httpResponseCode = http.POST(encryptedPayload);

  if (httpResponseCode <= 0) {
    res.error = http.errorToString(httpResponseCode);
    Serial.printf("HTTP Error: %s\n", res.error.c_str());
    http.end();
    return res;
  }

  Serial.printf("HTTP Response Code: %d\n", httpResponseCode);
  String response = http.getString();

  if (httpResponseCode == 200) {
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, response);
    if (!error) {
      res.rack_id = doc["rack_id"];
      res.user_id = doc["user_id"];
      res.lock_success = doc["lock_success"];
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

UnlockResponse unlock(const UnlockRequest request) {
  UnlockResponse res = {0, 0, false, ""};

  if (WiFi.status() != WL_CONNECTED) {
    res.error = "Network connection lost!";
    Serial.println(res.error);
    return res;
  }

  HTTPClient http;

  http.begin("http://192.168.86.27:8080/api/unlock");
  http.addHeader("Content-Type", "application/json");

  DynamicJsonDocument doc(256);
  doc["rack_id"] = request.rack_id;
  doc["user_name"] = request.user_name;
  doc["user_email"] = request.user_email;
  doc["user_phone"] = request.user_phone;
  
  String payload;
  serializeJson(doc, payload);
  String encryptedPayload = "{\"data\":\"" + encryptPayload(payload) + "\"}";
  // Serial.println(encryptedPayload);

  int httpResponseCode = http.POST(encryptedPayload);

  if (httpResponseCode <= 0) {
    res.error = http.errorToString(httpResponseCode);
    Serial.printf("HTTP Error: %s\n", res.error.c_str());
    http.end();
    return res;
  }

  Serial.printf("HTTP Response Code: %d\n", httpResponseCode);
  String response = http.getString();

  if (httpResponseCode == 200) {
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, response);
    if (!error) {
      res.rack_id = doc["rack_id"];
      res.user_id = doc["user_id"];
      res.unlock_success = doc["unlock_success"];
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


