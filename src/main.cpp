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
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>

constexpr auto GREEN_PIN = GPIO_Pin_12;
constexpr auto ORANGE_PIN = GPIO_Pin_13;

constexpr uint32_t DESIRED_PLL_M = 4;
constexpr uint32_t DESIRED_PLL_N = 120;
constexpr uint32_t DESIRED_PLL_P = 2;
constexpr uint32_t DESIRED_PLL_Q = 5;
constexpr uint32_t DESIRED_PLL_I2S_N = 50;
constexpr uint32_t DESIRED_PLL_I2S_R = 2;
constexpr uint32_t DESIRED_HCLK_DIV = 0; // HCLK_DIV aka AHBx Prescaler set to 1
constexpr uint32_t DESIRED_PCLK1_DIV =
    0x00001400; // PCLK1_DIV aka APB1 Prescaler set to 4
constexpr uint32_t DESIRED_PCKL2_DIV =
    0x00001000; // PCLK2_DIV aka APB2 Prescaler set to 2
constexpr uint32_t DESIRED_SYSCLK = 120000000;
constexpr uint32_t DESIRED_HCLK = 120000000;
constexpr uint32_t DESIRED_PCLK1 = 30000000;
constexpr uint32_t DESIRED_PCLK2 = 60000000;

// constexpr uint32_t TMR_PERIOD = 60 * 1000 * 1000;
constexpr uint32_t DESIRED_INT_RATE = 1 * 1000 * 1000;
constexpr uint32_t TMR_PERIOD = DESIRED_PCLK2 / DESIRED_INT_RATE;

bool check_clocks() {
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
  RCC_PLLI2SCmd(DISABLE);

  // disable HSE and CSS (disabling the HSE also disables CSS)
  RCC_HSEConfig(RCC_HSE_OFF);

  // Configure PLL values and set source to HSE
  RCC_PLLConfig(RCC_PLLSource_HSE, DESIRED_PLL_M, DESIRED_PLL_N, DESIRED_PLL_P,
                DESIRED_PLL_Q);

  // set AHB, APB1, APB2 prescalers
  RCC_HCLKConfig(DESIRED_HCLK_DIV);
  RCC_PCLK1Config(DESIRED_PCLK1_DIV);
  RCC_PCLK2Config(DESIRED_PCKL2_DIV);

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
void init_timer_and_interrupt() {
  NVIC_InitTypeDef nvicStructure;
  nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
  nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
  nvicStructure.NVIC_IRQChannelSubPriority = 1;
  nvicStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicStructure);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TIM_TimeBaseInitTypeDef initStruct;
  initStruct.TIM_Prescaler = 1 - 1; // prescaler 1:1 -> same as PCLK2
  initStruct.TIM_CounterMode = TIM_CounterMode_Up;
  initStruct.TIM_ClockDivision =
      TIM_CKD_DIV1; // clock division 1:1 -> same as PCLK2

  // - 1: timer counts up to (and incl.) the TIM*->ARR and then resets, thereby
  // the period is actually (TIM*->ARR)+1
  initStruct.TIM_Period = (uint32_t)(TMR_PERIOD - 1);

  TIM_TimeBaseInit(TIM2, &initStruct);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
}

void init_gpio() {
  GPIO_InitTypeDef initStruct;
  // initStruct.GPIO_Pin = GREEN_PIN | ORANGE_PIN;
  initStruct.GPIO_Pin = GPIO_Pin_All;
  initStruct.GPIO_Mode = GPIO_Mode_OUT;
  initStruct.GPIO_Speed = GPIO_Fast_Speed;
  initStruct.GPIO_OType = GPIO_OType_PP;
  initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &initStruct);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
}

extern "C" void TIM2_IRQHandler() {
  static uint16_t i = 0;
  constexpr uint16_t MASK = (uint16_t)~TIM_IT_Update; 
  TIM2->SR = MASK; // clear interrupt pending bit
  GPIOD->ODR = i++; // write i to GPIOD
}

void blink_forever() {
  while (true) {
    GPIO_ToggleBits(GPIOD, ORANGE_PIN);
    for (int i = 0; i < 1000000; i++) {
      asm("nop");
    }
  }
}

int main(void) {

  init_clocks();
  init_timer_and_interrupt();
  init_gpio();

  while (1) {
    asm("nop");
  }
  // blink_forever();

  return 0;
}

void *__dso_handle; // NOTE: get rid of this as soon as platformio fixes their
                    // toolchain