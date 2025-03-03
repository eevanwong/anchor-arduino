#ifndef MOTOR_HANDLER_H
#define MOTOR_HANDLER_H

#include <ezButton.h>
#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
const int Mosfet =  13;    // the number of the LED pin

#define IN1_PIN GPIO_NUM_5
#define IN2_PIN GPIO_NUM_18
#define IN3_PIN GPIO_NUM_19
#define IN4_PIN GPIO_NUM_21

void motor_unlock();
void motor_lock(ezButton &limitSwitch);
void setupMotor();
void activate_motor();

#endif // MOTOR_HANDLER_H
