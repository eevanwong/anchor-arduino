#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <Arduino.h>    // For String, etc.
#include <WiFi.h>       // For WiFi operations
#include <HTTPClient.h> // For HTTP requests

// Forward-declaring to avoid including encryption headers in your .h
// (You can include them if you need them here, but it's often neater to keep them in .cpp)
struct LockRequest;
struct UnlockRequest;
struct LockResponse;
struct UnlockResponse;

// Declarations of functions you want to call from your main code
void connectToNetwork();
LockResponse lock(const LockRequest& request);
UnlockResponse unlock(const UnlockRequest& request);

// Data structures shared with other files
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

#endif // WIFI_HANDLER_H
