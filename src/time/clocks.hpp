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

#ifndef PROJECT_CLOCKS_HPP
#define PROJECT_CLOCKS_HPP

#include <stdint.h>

namespace Time {
    // TODO consider splitting up this namespace (clocks/timer).

    // TODO consider putting these variables in a separate namespace
    const uint32_t DESIRED_PLL_M = 4;
    const uint32_t DESIRED_PLL_N = 120;
    const uint32_t DESIRED_PLL_P = 2;
    const uint32_t DESIRED_PLL_Q = 5;
    const uint32_t DESIRED_PLL_I2S_N = 50;
    const uint32_t DESIRED_PLL_I2S_R = 2;
    const uint32_t DESIRED_HCLK_DIV = 0; // HCLK_DIV aka AHBx Prescaler set to 1
    const uint32_t DESIRED_PCLK1_DIV = 0x00001400; // PCLK1_DIV aka APB1 Prescaler set to 4
    const uint32_t DESIRED_PCKL2_DIV = 0x00001000; // PCLK2_DIV aka APB2 Prescaler set to 2
    const uint32_t DESIRED_SYSCLK = 120000000;
    const uint32_t DESIRED_HCLK = 120000000;
    const uint32_t DESIRED_PCLK1 = 30000000;
    const uint32_t DESIRED_PCLK2 = 60000000;

    // TODO make this more flexible (maybe at some point some time in the future maybe)

    /** Checks whether clocks are set as desired.
     * @return boolean indicating whether the clocks are set as desired.
     */
    bool checkClocks();

    /** Initializes clocks based on the desired variables. Halts on failure (infinite loop).
     */
    void initClocks();
}

#endif //PROJECT_CLOCKS_HPP
