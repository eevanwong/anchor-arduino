#ifndef MOTOR_HANDLER_H
#define MOTOR_HANDLER_H

#include <ezButton.h>
#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
const int Mosfet = 25;    // the number of the LED pin

#define MOSFET GPIO_NUM_25
#define IN1_PIN GPIO_NUM_13
#define IN2_PIN GPIO_NUM_12
#define IN3_PIN GPIO_NUM_14
#define IN4_PIN GPIO_NUM_27
#define LIMIT_SWITCH GPIO_NUM_15
#define PEDO_BUZZER GPIO_NUM_2

void motor_unlock();
void motor_lock(ezButton &limitSwitch);
void setupMotor();
void activate_motor();

#endif // MOTOR_HANDLER_H
