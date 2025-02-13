#include <Arduino.h>
#include "motor_handler.h"

Stepper myStepper = Stepper(stepsPerRevolution, IN1_PIN, IN3_PIN, IN2_PIN, IN4_PIN);

void motor_lock() {
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500);
}

void motor_unlock() {
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(500);
}
