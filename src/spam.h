#ifndef SPAM_H
#define SPAM_H

#define NUM_SAMPLES_DEFAULT (256)
#define NUM_SAMPLES_MAX (16384)

#include "serial.h"
#include "time_and_dma.h"
#include <stm32f4xx_gpio.h>

enum Modes {
  MODE_CONT,
  MODE_ONESHOT,
  MODE_ONESHOT_REPEAT_1HZ,
  MODE_ONESHOT_REPEAT_10HZ,
  MODE_ONESHOT_REPEAT_100HZ,
  MODE_ONE_SHOT_REPEAT_1000HZ,
  MODE_DEFAULT = 0x64,
};

void init_spam_gpio();
void start_spam(char mode, uint16_t a_num_samples);
void TIM5_IRQHandler();

#endif // SPAM_H
