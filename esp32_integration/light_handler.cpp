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
    setColour(255, 255, 0);
}

void yellowOff() {
    setColour(0, 0, 0);
}

void toggleGreen() {
    setColour(0, 255, 0);
}

void greenOff() {
    setColour(0, 0, 0);
}

void toggleRed() {
    digitalWrite(RED_R_PIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(2000); 
    digitalWrite(RED_R_PIN, LOW);
}

void setColour(int red, int green, int blue) {
  analogWrite(RGB_R_PIN, red);
  analogWrite(RGB_G_PIN, green);
  analogWrite(RGB_B_PIN, blue);
}
