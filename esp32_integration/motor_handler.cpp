#include <Arduino.h>
#include "motor_handler.h"

Stepper myStepper(stepsPerRevolution, IN1_PIN, IN3_PIN, IN2_PIN, IN4_PIN);

void setupMotor() {
  delay(1000);
  myStepper.setSpeed(60);
  pinMode(MOSFET, OUTPUT);
  // digitalWrite(MOSFET, HIGH);
  // myStepper.step(1);
  // digitalWrite(MOSFET, LOW);
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

void motor_lock(ezButton &limitSwitch) {
  Serial.println("clockwise");

  while (limitSwitch.getState() == HIGH) {
    myStepper.step(1);
    limitSwitch.loop();
    delay(5);
  }
}

void motor_unlock() {
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(500);
}
