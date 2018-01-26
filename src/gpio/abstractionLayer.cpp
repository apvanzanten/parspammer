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

#include <stm32f4xx_rcc.h>
#include "abstractionLayer.hpp"
#include "conversionLayer.hpp"

namespace Gpio {


    uint16_t getPinByNo(uint8_t pinNo) {
        switch(pinNo){
            case 0: return PIN_0;
            case 1: return PIN_1;
            case 2: return PIN_2;
            case 3: return PIN_3;
            case 4: return PIN_4;
            case 5: return PIN_5;
            case 6: return PIN_6;
            case 7: return PIN_7;
            case 8: return PIN_8;
            case 9: return PIN_9;
            case 10: return PIN_10;
            case 11: return PIN_11;
            case 12: return PIN_12;
            case 13: return PIN_13;
            case 14: return PIN_14;
            case 15: return PIN_15;
            default:
                return 0; // TODO better error handling
        }
    }

    void enablePeriphClock(Port port) {
        RCC_AHB1PeriphClockCmd(getPortRccID(port), ENABLE);
    }

    void disablePeriphClock(Port port) {
        RCC_AHB1PeriphClockCmd(getPortRccID(port), DISABLE);
    }

    void configurePins(Configuration configuration) {
        GPIO_InitTypeDef initStruct;
        initStruct.GPIO_Pin = getPinIDs(configuration.pins);
        initStruct.GPIO_Mode = getModeID(configuration.mode);
        initStruct.GPIO_Speed = getSpeedID(configuration.speed);
        initStruct.GPIO_OType = getOutputTypeID(configuration.outputType);
        initStruct.GPIO_PuPd = getPullSettingID(configuration.pullSetting);
        GPIO_Init(getPortPtr(configuration.port), &initStruct);
    }

    void configurePins(Port port, uint16_t pins, Mode mode, Speed speed, OutputType outputType, PullSetting pullSetting) {
        GPIO_InitTypeDef initStruct;
        initStruct.GPIO_Pin = getPinIDs(pins);
        initStruct.GPIO_Mode = getModeID(mode);
        initStruct.GPIO_Speed = getSpeedID(speed);
        initStruct.GPIO_OType = getOutputTypeID(outputType);
        initStruct.GPIO_PuPd = getPullSettingID(pullSetting);
        GPIO_Init(getPortPtr(port), &initStruct);
    }

    void setPins(Port port, uint16_t pins) {
        GPIO_SetBits(getPortPtr(port), getPinIDs(pins));
    }

    void clearPins(Port port, uint16_t pins) {
        GPIO_ResetBits(getPortPtr(port), getPinIDs(pins));
    }

    void togglePins(Port port, uint16_t pins) {
        GPIO_ToggleBits(getPortPtr(port), getPinIDs(pins));
    }

    void writePins(Port port, uint16_t pins, bool newValue) {
        if(newValue){
            setPins(port, pins);
        } else {
            clearPins(port, pins);
        }
    }

    void writePort(Port port, uint16_t value) {
        GPIO_Write(getPortPtr(port), value);
    }

    bool readInputPin(Port port, uint16_t pin) {
        return GPIO_ReadInputDataBit(getPortPtr(port), getPinIDs(pin));
    }

    bool readInputPinByNo(Port port, uint8_t pinNo) {
        return GPIO_ReadInputDataBit(getPortPtr(port), getPinIDByNo(pinNo));
    }
}
