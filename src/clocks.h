#ifndef CLOCKS_H
#define CLOCKS_H

#include <stm32f4xx_rcc.h>

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

int check_clocks();
void init_clocks();

#endif // CLOCKS_H
