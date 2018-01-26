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
#include <stm32f4xx_tim.h>
#include "time.hpp"
#include "clocks.hpp"

namespace Time {
    bool gIsInitialized = false;

    // calls the clocks initialisation function and then initializes and starts TIM2 running at 84mhz with a period of 0xffffffff (roughly 51 seconds)
    void init(){
        if(gIsInitialized) {
            return; // already initialized, do nothing.
        }

        initClocks();

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
        TIM_TimeBaseInitTypeDef initStruct;
        initStruct.TIM_Prescaler = 1 - 1; // prescaler 1:1 -> 84MHz
        initStruct.TIM_CounterMode = TIM_CounterMode_Up;
        initStruct.TIM_ClockDivision = TIM_CKD_DIV1; // clock division 1:1 -> 84MHz

        // - 1: timer counts up to (and incl.) the TIM*->ARR and then resets, thereby the period is actually (TIM*->ARR)+1
        initStruct.TIM_Period = (uint32_t)(TMR_PERIOD - 1);

        TIM_TimeBaseInit(TIM2,&initStruct);
        TIM_Cmd(TIM2,ENABLE);

        gIsInitialized = true;
    }

    uint32_t getTicks(){
        if(!gIsInitialized) init();

        return TIM2->CNT;
    }

    uint32_t getUS(){
        return getTicks() / TICKS_PER_US;
    }

    uint32_t getMS(){
        return getTicks() / TICKS_PER_MS;
    }

    void delayTicks(uint32_t delayTime){
        // Overflow protection assumes that delayTime < 2 * TMR_PERIOD at all times.
        // So, as long as TMR_PERIOD is more than half of UINT32_MAX it should be fine.
        if(delayTime == 0) return;

        uint32_t start = getTicks();
        uint64_t desiredEnd = start + delayTime;
        if(desiredEnd >= TMR_PERIOD){ // we have overflow
            desiredEnd -= TMR_PERIOD;
            while(getTicks() > desiredEnd); // wait for our timer to overflow as well
        }

        while(getTicks() < desiredEnd);
    }

    void delayTicksLong(uint64_t delayTime) {
        while(delayTime > MAX_DELAY_IN_TICKS){
            delayTicks(MAX_DELAY_IN_TICKS);
            delayTime -= MAX_DELAY_IN_TICKS;
        }
        delayTicks(delayTime);
    }

    void delayUS(uint32_t delayTime){
        delayTicksLong(delayTime * TICKS_PER_US);
    }

    void delayMS(uint32_t delayTime){
        delayTicksLong(delayTime * TICKS_PER_MS);
    }

    void delayS(float delayTime) {
        delayTicksLong((uint64_t)(delayTime * TICKS_PER_S));
    }


    uint32_t getTicksSince(uint32_t startTime) {
        uint32_t currentTime = getTicks();
        if(currentTime < startTime) {
            return (uint64_t) currentTime + (uint64_t)PERIOD_IN_TICKS - (uint64_t)startTime;
        } else {
            return currentTime - startTime;
        }
    }

    uint32_t getUSSince(uint32_t startTime) {
        uint32_t currentTime = getUS();
        if(currentTime < startTime) {
            return (uint64_t)currentTime + (uint64_t)PERIOD_IN_US - (uint64_t)startTime;
        } else {
            return currentTime - startTime;
        }
    }

    uint32_t getMSSince(uint32_t startTime) {
        uint32_t currentTime = getMS();
        if(currentTime < startTime) {
            return (uint64_t)currentTime + (uint64_t)PERIOD_IN_MS - (uint64_t)startTime;
        } else {
            return currentTime - startTime;
        }
    }
}