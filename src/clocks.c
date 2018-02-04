#include "clocks.h"

/* Function that asks the SPL for the current system clocks, and compares them
 * to the clocks defined in DESIRED_xCLKy.
 */
int check_clocks() {
  RCC_ClocksTypeDef clocks;
  RCC_GetClocksFreq(&clocks);

  return ((clocks.SYSCLK_Frequency == DESIRED_SYSCLK) &&
          (clocks.HCLK_Frequency == DESIRED_HCLK) &&
          (clocks.PCLK1_Frequency == DESIRED_PCLK1) &&
          (clocks.PCLK2_Frequency == DESIRED_PCLK2)); 
}

/* Initialize the clocks according the the configuration defined in DESIRED_x macros.
 * Will halt (nop loop) if the configuration fails.
*/
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
