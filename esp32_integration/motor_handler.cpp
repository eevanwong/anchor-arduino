#include <Arduino.h>
#include "motor_handler.h"

Stepper myStepper(stepsPerRevolution, IN1_PIN, IN3_PIN, IN2_PIN, IN4_PIN);
ezButton limitSwitch(LIMIT_SWITCH); 
int LOCK_STATE = 0; // Set LOCK_STATE BASED ON ITS STATE

void setupMotor() {
  limitSwitch.setDebounceTime(50);
  myStepper.setSpeed(60);
  
  pinMode(MOSFET, OUTPUT);

  // check lock_state based on limit switch
  if (limitSwitch.getState() == HIGH) {
    LOCK_STATE = UNLOCKED;
  } else {
    LOCK_STATE = LOCKED; 
  }

  Serial.print("Lock state:");
  Serial.println(LOCK_STATE);
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
  delay(1000);
  Serial.println("Lock start");
  digitalWrite(Mosfet, HIGH);
  Serial.println(limitSwitch.getState());
  while (limitSwitch.getState() == HIGH) {
    myStepper.step(400);
    limitSwitch.loop();
    Serial.println(limitSwitch.getState());
    delay(1000);
  }
  digitalWrite(Mosfet, LOW);
  delay(1000);         // ...for 1 sec
}

void motor_unlock() {
  delay(1000);
  Serial.println("unlock start");
  digitalWrite(Mosfet, HIGH);
  while (limitSwitch.getState() == LOW) {
    myStepper.step(400);
    limitSwitch.loop();
    Serial.println(limitSwitch.getState());
    delay(1000);
  }
  digitalWrite(Mosfet, LOW);
  delay(1000);
}
