#ifndef LIGHT_HANDLER_H
#define LIGHT_HANDLER_H

#define G_PIN 2
#define Y_PIN GPIO_NUM_18
#define R_PIN 4

void initializeLED();
void yellowOn();
void yellowOff();
void toggleGreen();
void toggleRed();

#endif // LIGHT_HANDLER_H
