#include <Arduino.h>
#include "buzzer_handler.h"

void setupBuzzer() {
  pinMode(PIEZO_BUZZER, OUTPUT);
  digitalWrite(PIEZO_BUZZER, LOW);
}

// Helper function to play a tone with a brief pause between notes
void playTone(int frequency, int duration) {
  tone(PIEZO_BUZZER, frequency, duration);
  delay(duration);
  noTone(PIEZO_BUZZER);
  delay(50); // short pause between notes
}

// Tap card event: 2-note tune
void tapCardTune() {
  playTone(400, 300);  // C5 for 300ms (adjust duration as needed)
}

// Lock/Unlock success: 2-note tune
void successTune() {
  playTone(300, 150);  // G4 for 150ms
  playTone(550, 150);  // C5 for 150ms
}

// Locking complete: 3-note tune
void failureTune() {
  playTone(400, 150);  // First note (C5) for 150ms
  playTone(200, 150);  // Lower note (A4) for 150ms
}

void lockingCompleteTune() {
  playTone(659, 100);  // E5 for 100ms
  playTone(587, 100);  // D5 for 100ms
  playTone(523, 100);  // C5 for 100ms
}
