/*  Copyright (c) 2015 Arjen Pieter van Zanten
 *
 *   License: MIT
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a
 *copy of this software and associated documentation files (the "Software"), to
 *deal in the Software without restriction, including without limitation the
 *rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *sell copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *IN THE SOFTWARE.
 **/

#include <misc.h>
#include <stm32f4xx_dma.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>

#define GREEN_PIN (GPIO_Pin_12)
#define ORANGE_PIN (GPIO_Pin_13)

#define DESIRED_PLL_M (4)   // 2-53
#define DESIRED_PLL_N (360) // 50-432
#define DESIRED_PLL_P (4)   // 2,4,6,8
#define DESIRED_PLL_Q (15)  // 2-15
#define DESIRED_HCLK_DIV (RCC_SYSCLK_Div1)
#define DESIRED_APB1_PRESCALER (RCC_HCLK_Div8)
#define DESIRED_APB2_PRESCALER (RCC_HCLK_Div2)

#define DESIRED_SYSCLK (180000000)
#define DESIRED_HCLK (180000000)
#define DESIRED_PCLK1 (22500000)
#define DESIRED_PCLK2 (90000000)

#define DESIRED_TMR_RATE (DESIRED_PCLK2 * 2)
#define DESIRED_SAMPLE_RATE (20 * 1000 * 1000)
#define TMR_PERIOD (DESIRED_TMR_RATE / DESIRED_SAMPLE_RATE)

#define RCC_DMA_PERIPH_CLK (RCC_AHB1Periph_DMA2)
#define DMA_STREAM (DMA2_Stream5)
#define DMA_CHANNEL (DMA_Channel_6)

#define BUFFER_SIZE (4096)

int check_clocks() {
  RCC_ClocksTypeDef clocks;
  RCC_GetClocksFreq(&clocks);

  return ((clocks.SYSCLK_Frequency == DESIRED_SYSCLK) &&
          (clocks.HCLK_Frequency == DESIRED_HCLK) &&
          (clocks.PCLK1_Frequency == DESIRED_PCLK1) &&
          (clocks.PCLK2_Frequency == DESIRED_PCLK2)); // all clocks are equal to
                                                      // the desired clocks if
                                                      // this is true.
}

void init_clocks() {
  // enable HSI and set it as the system clock source
  RCC_HSICmd(ENABLE);
  while (!(RCC->CR & RCC_CR_HSIRDY))
    ; // wait for it to be ready
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

  // disable PLL and PLLI2S
  RCC_PLLCmd(DISABLE);
  // RCC_PLLI2SCmd(DISABLE);

  // disable HSE and CSS (disabling the HSE also disables CSS)
  RCC_HSEConfig(RCC_HSE_OFF);

  // Configure PLL values and set source to HSE
  RCC_PLLConfig(RCC_PLLSource_HSE, DESIRED_PLL_M, DESIRED_PLL_N, DESIRED_PLL_P,
                DESIRED_PLL_Q);

  // set AHB, APB1, APB2 prescalers
  RCC_HCLKConfig(DESIRED_HCLK_DIV);
  RCC_PCLK1Config(DESIRED_APB1_PRESCALER);
  RCC_PCLK2Config(DESIRED_APB2_PRESCALER);

  // enable HSE
  RCC_HSEConfig(RCC_HSE_ON);
  if (RCC_WaitForHSEStartUp() == ERROR)
    RCC_DeInit(); // SHUT DOWN, EVERYTHING!

  // enable PLL
  RCC_PLLCmd(ENABLE);
  while (!(RCC->CR & RCC_CR_PLLRDY))
    ; // wait for ready

  // set system clock source to PLL
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

  if (!check_clocks()) {
    while (1) {
    }
  }
}

void init_timer_and_dma(uint16_t *buffer, uint16_t buffer_size) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  TIM_TimeBaseInitTypeDef initStruct;
  initStruct.TIM_Prescaler = 1 - 1; // prescaler 1:1 -> same as PCLK2
  initStruct.TIM_CounterMode = TIM_CounterMode_Up;
  initStruct.TIM_ClockDivision =
      TIM_CKD_DIV1; // clock division 1:1 -> same as PCLK2

  // - 1: timer counts up to (and incl.) the TIM*->ARR and then resets, thereby
  // the period is actually (TIM*->ARR)+1
  initStruct.TIM_Period = (uint32_t)(TMR_PERIOD - 1);

  initStruct.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &initStruct);
  TIM_Cmd(TIM1, ENABLE);

  // DMA init

  DMA_DeInit(DMA_STREAM);
  RCC_AHB1PeriphClockCmd(RCC_DMA_PERIPH_CLK, ENABLE);

  DMA_InitTypeDef dmaInitStruct;
  dmaInitStruct.DMA_Channel = DMA_CHANNEL;
  dmaInitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&GPIOD->ODR);
  dmaInitStruct.DMA_Memory0BaseAddr = (uint32_t)buffer;
  dmaInitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  dmaInitStruct.DMA_BufferSize = buffer_size;
  dmaInitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dmaInitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dmaInitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  dmaInitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  dmaInitStruct.DMA_Mode = DMA_Mode_Circular;
  dmaInitStruct.DMA_Priority = DMA_Priority_High;
  dmaInitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
  dmaInitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  dmaInitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  dmaInitStruct.DMA_PeripheralBurst = DMA_MemoryBurst_Single;
  DMA_Init(DMA_STREAM, &dmaInitStruct);

  DMA_Cmd(DMA_STREAM, ENABLE);

  TIM_DMACmd(TIM1, TIM_DMA_Update, ENABLE);
}

void init_gpio() {
  GPIO_InitTypeDef initStruct;
  initStruct.GPIO_Pin = GPIO_Pin_All;
  initStruct.GPIO_Mode = GPIO_Mode_OUT;
  initStruct.GPIO_Speed = GPIO_High_Speed;
  initStruct.GPIO_OType = GPIO_OType_PP;
  initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &initStruct);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
}

void greet() {
  for (int i = 0; i < 10; i++) {
    GPIO_ToggleBits(GPIOD, ORANGE_PIN);
    for (int i = 0; i < 5000000; i++) {
      __asm__("nop");
    }
  }
}

int main(void) {

  uint16_t buffer[BUFFER_SIZE];
  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = i;
  }

  init_clocks();
  init_gpio();

  greet();

  init_timer_and_dma(buffer, BUFFER_SIZE);

  while (1) {
    __asm__("nop");
  }

  return 0;
}