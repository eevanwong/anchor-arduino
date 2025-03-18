#ifndef BUZZER_HANDLER_H
#define BUZZER_HANDLER_H

#define PIEZO_BUZZER GPIO_NUM_2

void setupBuzzer();
void playTone(int frequency, int duration);

void tapCardTune();
void successTune();
void failureTune();
void lockingCompleteTune();

#endif // BUZZER_HANDLER_H
