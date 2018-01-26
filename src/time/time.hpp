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

#ifndef PROJECT_TIME_HPP
#define PROJECT_TIME_HPP

#include <stdint.h>

/** Namespace containing constants and functions pertaining to timing. Specifically the clock configuration and the
 * control and use of the hardware timer.
 */
namespace Time {
    static const uint32_t TMR_SPEED_MHZ = 60;
    static const uint64_t TMR_PERIOD = UINT32_MAX + (uint64_t)1;

    static const uint32_t TICKS_PER_US = TMR_SPEED_MHZ;
    static const uint32_t TICKS_PER_MS = TICKS_PER_US * 1000;
    static const uint32_t TICKS_PER_S = TICKS_PER_MS * 1000;

    static const uint64_t PERIOD_IN_TICKS = TMR_PERIOD;
    static const uint32_t PERIOD_IN_US = PERIOD_IN_TICKS / TICKS_PER_US;
    static const uint32_t PERIOD_IN_MS = PERIOD_IN_US / TICKS_PER_MS;

    static const uint32_t MAX_DELAY_IN_TICKS = UINT32_MAX;

    extern bool gIsInitialized; ///< boolean variable that indicates whether or not the timer has been initialized.

    /** Initializes the timer if it isn't already initialized.
     */
    void init();

    /** Returns the current 'time' (state of timer) in ticks.
     * @return uint32_t current time in ticks.
     */
    uint32_t getTicks();

    /** Returns the current 'time' (state of timer) in microseconds.
     * @return uint32_t current time in microseconds.
     */
    uint32_t getUS();

    /** Returns the current 'time' (state of timer) in milliseconds.
     * @return uint32_t current time in milliseconds.
     */
    uint32_t getMS();

    /** Delays (i.e. blocking wait) for roughly the given number of ticks.
     * @param uint32_t delayTime The amount of time to delay in ticks.
     */
    void delayTicks(uint32_t delayTime);

    /** Delays (i.e. blocking wait) for roughly the given number of ticks.
     * Allows for 64-bit input for longer delays.
     * @param uint64_t delayTime The amount of time to delay in ticks.
     */
    void delayTicksLong(uint64_t delayTime);

    /** Delays (i.e. blocking wait) for the given number of microseconds. Almost certainly accurate to less than a
     * microsecond.
     * @param uint32_t delayTime The amount of time to delay in microseconds.
     */
    void delayUS(uint32_t delayTime);

    /** Delays (i.e. blocking wait) for the given number of milliseconds.
     * @param uint32_t delayTime The amount of time to delay in milliseconds.
     */
    void delayMS(uint32_t delayTime);

    /** Delays (i.e. blocking wait) for the given number of seconds. Suspected accurate to less than 10^-8 seconds.
     * @param float seconds The amount of time to delay in seconds.
     */
    void delayS(float seconds);

    /** Calculates and returns the number of ticks that have passed since the given startTime. If the given startTime is
     * a higher number than the current time it is assumed that an overflow has occurred.
     * @param startTime The time from which to count the passed ticks.
     * @return the number of ticks that have passed.
     */
    uint32_t getTicksSince(uint32_t startTime);

    /** Calculates and returns the number of microseconds that have passed since the given startTime. If the given
     * startTime is a higher number than the current time it is assumed that an overflow has occurred.
     * @param startTime The time from which to count the passed microseconds.
     * @return the number of microseconds that have passed.
     */
    uint32_t getUSSince(uint32_t startTime);

    /** Calculates and returns the number of milliseconds that have passed since the given startTime. If the given
     * startTime is a higher number than the current time it is assumed that an overflow has occurred.
     * @param startTime The time from which to count the passed milliseconds.
     * @return the number of milliseconds that have passed.
     */
    uint32_t getMSSince(uint32_t startTime);
}

#endif //PROJECT_TIME_HPP
