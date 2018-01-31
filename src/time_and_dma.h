#ifndef TIME_AND_DMA_H
#define TIME_AND_DMA_H

#include <stdint.h>
#include <misc.h>
#include <stm32f4xx_dma.h>
#include <stm32f4xx_tim.h>

#include "clocks.h"

#define SPAM_SAMPLE_RATE (20 * 1000 * 1000)
// #define SPAM_SAMPLE_RATE (1 * 1000 * 1000)

#define SPAM_TMR (TIM1)
#define SPAM_TMR_RCC (RCC_APB2Periph_TIM1)
#define SPAM_TMR_CLOCK_SOURCE (DESIRED_PCLK2 * 2)
#define SPAM_TMR_PRESCALER (1)
#define SPAM_TMR_RATE (SPAM_TMR_CLOCK_SOURCE / SPAM_TMR_PRESCALER)
#define SPAM_TMR_PERIOD (SPAM_TMR_RATE / SPAM_SAMPLE_RATE)

#define MAIN_TMR (TIM2)
#define MAIN_TMR_RCC (RCC_APB1Periph_TIM2)
#define MAIN_TMR_CLOCK_SOURCE (DESIRED_PCLK1 * 2)
#define MAIN_TMR_PRESCALER (MAIN_TMR_CLOCK_SOURCE / (1000 * 1000))
#define MAIN_TMR_RATE (MAIN_TMR_CLOCK_SOURCE / MAIN_TMR_PRESCALER) // 1MHz
#define MAIN_TMR_PERIOD (UINT32_MAX)
#define MAIN_TMR_TICKS_PER_US (1)

#define REPEAT_TMR (TIM5)
#define REPEAT_TMR_RCC (RCC_APB1Periph_TIM5)
#define REPEAT_TMR_IRQ (TIM5_IRQn)
#define REPEAT_TMR_IRQ_HANDLER (TIM5_IRQHandler)
#define REPEAT_TMR_PRESCALER (MAIN_TMR_PRESCALER)
#define REPEAT_TMR_RATE (MAIN_TMR_RATE) // 1MHz

#define RCC_DMA_PERIPH_CLK (RCC_AHB1Periph_DMA2)
#define DMA_STREAM (DMA2_Stream5)
#define DMA_CHANNEL (DMA_Channel_6)
#define DMA_FLAGS (DMA_FLAG_DMEIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_HTIF5 | DMA_FLAG_TCIF5 | DMA_FLAG_TEIF5)

void init_main_timer();
void init_spam_timer();
void init_spam_repeat_timer();
void init_spam_dma(uint16_t * buffer, uint16_t buffer_size, int is_continuous);
void wait(float time_sec);

#endif // TIME_H
