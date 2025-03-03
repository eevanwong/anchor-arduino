#include <Arduino.h>
#include "motor_handler.h"

Stepper myStepper = Stepper(stepsPerRevolution, 16, 17, 5, 18);

void setupMotor() {
  delay(1000);
  myStepper.setSpeed(60);
  pinMode(Mosfet, OUTPUT);
}

void activate_motor() {
  // step one revolution  in one direction:
  digitalWrite(Mosfet, HIGH);
  myStepper.step(stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  myStepper.step(-stepsPerRevolution);
  digitalWrite(Mosfet, LOW);
  delay(5000);
}

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
