#ifndef TIME_AND_DMA_H
#define TIME_AND_DMA_H

#include <stdint.h>
#include <stm32f4xx_dma.h>
#include <stm32f4xx_tim.h>

#include "clocks.h"

#define SPAM_SAMPLE_RATE (20 * 1000 * 1000)

#define SPAM_TMR_CLOCK_SOURCE (DESIRED_PCLK2 * 2)
#define SPAM_TMR_PRESCALER (1)
#define SPAM_TMR_RATE (SPAM_TMR_CLOCK_SOURCE / SPAM_TMR_PRESCALER)
#define SPAM_TMR_PERIOD (SPAM_TMR_RATE / SPAM_SAMPLE_RATE)

#define MAIN_TMR_CLOCK_SOURCE (DESIRED_PCLK1 * 2)
#define MAIN_TMR_PRESCALER (MAIN_TMR_CLOCK_SOURCE / (1000 * 1000))
#define MAIN_TMR_RATE (MAIN_TMR_CLOCK_SOURCE / MAIN_TMR_PRESCALER) // 1MHz
#define MAIN_TMR_PERIOD (UINT32_MAX)
#define MAIN_TMR_TICKS_PER_US (1)

#define RCC_DMA_PERIPH_CLK (RCC_AHB1Periph_DMA2)
#define DMA_STREAM (DMA2_Stream5)
#define DMA_CHANNEL (DMA_Channel_6)

void init_spam(uint16_t * buffer, uint16_t buffer_size);
void init_main_timer();
void wait_us(uint32_t time_us);
void wait(float time_sec);

#endif // TIME_H
