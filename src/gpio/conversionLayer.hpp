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
*   Created on 17-8-2015
**/

#ifndef PROJECT_CONVERSIONLAYER_HPP
#define PROJECT_CONVERSIONLAYER_HPP

#include <stm32f4xx_gpio.h>
#include "enumsAndStructs.hpp"

namespace Gpio {
    GPIO_TypeDef * getPortPtr(Port port);
    uint32_t getPortRccID(Port port);
    uint16_t getPinIDs(uint16_t pins);
    uint16_t getPinIDByNo(uint8_t pinNo);
    GPIOMode_TypeDef getModeID(Mode mode);
    GPIOOType_TypeDef getOutputTypeID(OutputType outputType);
    GPIOSpeed_TypeDef getSpeedID(Speed speed);
    GPIOPuPd_TypeDef getPullSettingID(PullSetting pullSetting);
}

#endif //PROJECT_CONVERSIONLAYER_HPP
