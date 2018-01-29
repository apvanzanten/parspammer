#ifndef TIME_AND_DMA_H
#define TIME_AND_DMA_H

#include <stm32f4xx_dma.h>
#include <stm32f4xx_tim.h>

#include "clocks.h"

#define DESIRED_TMR_RATE (DESIRED_PCLK2 * 2)
#define DESIRED_SAMPLE_RATE (20 * 1000 * 1000)
#define TMR_PERIOD (DESIRED_TMR_RATE / DESIRED_SAMPLE_RATE)

#define RCC_DMA_PERIPH_CLK (RCC_AHB1Periph_DMA2)
#define DMA_STREAM (DMA2_Stream5)
#define DMA_CHANNEL (DMA_Channel_6)

void init_spam_timer_and_dma(uint16_t * buffer, uint16_t buffer_size);
void init_main_timer();

#endif // TIME_H
