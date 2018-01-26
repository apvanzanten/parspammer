/*  Copyright (c) 2015 Arjen Pieter van Zanten
*   
*   License: MIT
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
*   Created on 18-8-2015
**/

#include <stm32f4xx_rcc.h>
#include "clocks.hpp"

namespace Time {
    bool checkClocks() {
        RCC_ClocksTypeDef clocks;
        RCC_GetClocksFreq(&clocks);

        return (
                (clocks.SYSCLK_Frequency == DESIRED_SYSCLK) &&
                (clocks.HCLK_Frequency == DESIRED_HCLK) &&
                (clocks.PCLK1_Frequency == DESIRED_PCLK1) &&
                (clocks.PCLK2_Frequency == DESIRED_PCLK2)
        ); // all clocks are equal to the desired clocks if this is true.
    }

    void initClocks() {
        // enable HSI and set it as the system clock source
        RCC_HSICmd(ENABLE);
        while(!(RCC->CR & RCC_CR_HSIRDY)); // wait for it to be ready
        RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

        // disable PLL and PLLI2S
        RCC_PLLCmd(DISABLE);
        RCC_PLLI2SCmd(DISABLE);

        // disable HSE and CSS (disabling the HSE also disables CSS)
        RCC_HSEConfig(RCC_HSE_OFF);

        // Configure PLL values and set source to HSE
        RCC_PLLConfig(
                RCC_PLLSource_HSE,
                DESIRED_PLL_M,
                DESIRED_PLL_N,
                DESIRED_PLL_P,
                DESIRED_PLL_Q
        );

        // set PLL I2S to our new values
        RCC_PLLI2SConfig(
                DESIRED_PLL_I2S_N,
                DESIRED_PLL_I2S_R
        );

        // set I2S clock source to PLLI2S
        RCC_I2SCLKConfig(RCC_I2S2CLKSource_PLLI2S);

        // set AHB, APB1, APB2 prescalers
        RCC_HCLKConfig(DESIRED_HCLK_DIV);
        RCC_PCLK1Config(DESIRED_PCLK1_DIV);
        RCC_PCLK2Config(DESIRED_PCKL2_DIV);

        // enable HSE
        RCC_HSEConfig(RCC_HSE_ON);
        if(RCC_WaitForHSEStartUp() == ERROR) RCC_DeInit(); // SHUT DOWN, EVERYTHING!

        // enable CSS
        RCC_ClockSecuritySystemCmd(ENABLE);

        // enable PLL
        RCC_PLLCmd(ENABLE);
        while(!(RCC->CR & RCC_CR_PLLRDY)); // wait for ready

        // enable PLL I2S
        RCC_PLLI2SCmd(ENABLE);

        // set system clock source to PLL
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        if(!checkClocks()){
            // No actual error reporting done here because we don't want to depend on GPIO etc
            while(1);
        }

    }
}