#ifndef CLOCKS_H
#define CLOCKS_H

#include <stm32f4xx_rcc.h>

// CLOCK configuration, general things to remember:
// - HSE (external crystal) on STM32F4 runs at 8MHz
// - C1 = HSE / PLL_M (internal to the pll, not used directly)
// - C2 = C1 * PLL_N (interal to the pll, not used directly)
// - SYSCLK = C2 / PLL_P
// - HCLK = SYSCLK / HCLK_DIV
// - 48MHz peripheral clock = C2 / PLL_Q (I am not sure which peripherals
// actually depend on it, but STM seems to be pretty clear that this clock
// should run at 48MHz)
// - PCLK1 = SYSCLK / APB1_PRESCALER
// - PCLK2 = SYSCLK / APB2_PRESCALER
// - APB1 Timer clocks = PCLK1 * 2 (for timer 2 through 7 and 12 through 14)
// - APB2 Timer clocks = PCLK2 * 2 (for timer 1 and 8 through 11)
// - PLL_M can be anything from 2 through 53
// - PLL_N can be anything from 50 through 432
// - PLL_P can be 2, 4, 6, or 8
// - PLL_Q can be anything from 2 through 15
// - For HCLK_DIV, APB1_PRESCALER and APB2_PRESCALER, use the defines provided
// by the spl.

#define DESIRED_PLL_M (4)   
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

int check_clocks();
void init_clocks();

#endif // CLOCKS_H
