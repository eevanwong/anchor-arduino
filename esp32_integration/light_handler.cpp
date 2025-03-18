#include <Arduino.h>
#include "light_handler.h"

void initializeLED() {
  // initialize digital pins for green, yellow, and red
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);
  pinMode(RED_R_PIN, OUTPUT);
}
//
void yellowOn() {
    delay(500);
    setColour(128, 0, 128);
}

void yellowOff() {
    delay(500);
    setColour(0, 0, 0);
}

void redOn() {
    delay(500);
    setColour(255, 0, 0);
}

void redOff() {
    delay(500);
    setColour(0, 0, 0);
}

void greenOn() {
  delay(500);
  setColour(0, 255, 0);
}

void greenOff() {
  delay(500);
  setColour(0, 255, 0); 
}

void greenOpenOn() {
  delay(500);
  digitalWrite(RED_R_PIN, HIGH);  // turn the LED on (HIGH is the voltage level)
}

void greenOpenOff() {
  delay(500);
  digitalWrite(RED_R_PIN, LOW);  // turn the LED on (HIGH is the voltage level)
}

void setColour(int red, int green, int blue) {
  analogWrite(RGB_R_PIN, red);
  analogWrite(RGB_G_PIN, green);
  analogWrite(RGB_B_PIN, blue);
}
