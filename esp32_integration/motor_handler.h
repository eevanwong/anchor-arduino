#ifndef MOTOR_HANDLER_H
#define MOTOR_HANDLER_H

const int stepsPerRevolution = 2048;

#define IN1_PIN GPIO_NUM_5
#define IN2_PIN GPIO_NUM_18
#define IN3_PIN GPIO_NUM_19
#define IN4_PIN GPIO_NUM_21

void motor_unlock();
void motor_lock();

#endif // MOTOR_HANDLER_H
