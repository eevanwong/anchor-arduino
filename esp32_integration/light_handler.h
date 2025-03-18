#ifndef LIGHT_HANDLER_H
#define LIGHT_HANDLER_H

#define RGB_R_PIN GPIO_NUM_19
#define RGB_B_PIN GPIO_NUM_5
#define RGB_G_PIN GPIO_NUM_18
#define RED_R_PIN GPIO_NUM_4

//#define G_PIN GPIO_NUM_5
//#define R_PIN GPIO_NUM_19

// UPDATE THESE
void initializeLED();

// need to update these
void yellowOn();
void yellowOff();
void greenOn();
void greenOff();
void redOn();
void redOff();
void greenOpenOn();
void greenOpenOff();
void setColour(int red, int green, int blue);

#endif // LIGHT_HANDLER_H
