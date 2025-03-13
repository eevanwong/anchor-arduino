#ifndef MOTOR_HANDLER_H
#define MOTOR_HANDLER_H
#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
const int Mosfet = 25;    // the number of the LED pin

#define MOSFET GPIO_NUM_25
#define IN1_PIN GPIO_NUM_13
#define IN2_PIN GPIO_NUM_12
#define IN3_PIN GPIO_NUM_14
#define IN4_PIN GPIO_NUM_27

void motor_unlock();
void motor_lock();
void setupMotor();
void activate_motor();

#endif // MOTOR_HANDLER_H
