#include <Arduino.h>
#include "light_handler.h"

void initializeLED() {
    // initialize digital pins for green, yellow, and red
  pinMode(G_PIN, OUTPUT);
  pinMode(Y_PIN, OUTPUT);
  pinMode(R_PIN, OUTPUT);
}

void yellowOn() {
    digitalWrite(Y_PIN, HIGH);
}

void yellowOff() {
    digitalWrite(Y_PIN, LOW);
}

void toggleGreen() {
    yellowOff();
    digitalWrite(G_PIN, HIGH);
    delay(1000); 
    digitalWrite(G_PIN, LOW);
}

void toggleRed() {
    digitalWrite(R_PIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000); 
    digitalWrite(R_PIN, LOW);
}