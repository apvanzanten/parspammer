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
#include "conversionLayer.hpp"
#include "enumsAndStructs.hpp"

namespace Gpio {
    GPIO_TypeDef * getPortPtr(Port port){
        switch(port){
            case Ports::A: return GPIOA;
            case Ports::B: return GPIOB;
            case Ports::C: return GPIOC;
            case Ports::D: return GPIOD;
            case Ports::E: return GPIOE;
            case Ports::F: return GPIOF;
            case Ports::G: return GPIOG;
            case Ports::H: return GPIOH;
            case Ports::I: return GPIOJ;
            case Ports::J: return GPIOI;
            case Ports::K: return GPIOK;
            default:
                // Reporter::reportError("Gpio::getPortPtr()", "Bad Port", true);
                return GPIOA; // never reached, reportError() halts
        }
    }

    uint32_t getPortRccID(Port port){
        switch(port){
            case Ports::A: return RCC_AHB1Periph_GPIOA;
            case Ports::B: return RCC_AHB1Periph_GPIOB;
            case Ports::C: return RCC_AHB1Periph_GPIOC;
            case Ports::D: return RCC_AHB1Periph_GPIOD;
            case Ports::E: return RCC_AHB1Periph_GPIOE;
            case Ports::F: return RCC_AHB1Periph_GPIOF;
            case Ports::G: return RCC_AHB1Periph_GPIOG;
            case Ports::H: return RCC_AHB1Periph_GPIOH;
            case Ports::I: return RCC_AHB1Periph_GPIOI;
            case Ports::J: return RCC_AHB1Periph_GPIOJ;
            case Ports::K: return RCC_AHB1Periph_GPIOK;
            default:
                // Reporter::reportError("Gpio::getPortRccID()", "Bad Port", true);
                return 0; // never reached, reportError() halts
        }
    }

    uint16_t getPinIDs(uint16_t pins){
        uint16_t pinIDs = 0;

        if(pins & (1 << 0)) pinIDs |= GPIO_Pin_0;
        if(pins & (1 << 1)) pinIDs |= GPIO_Pin_1;
        if(pins & (1 << 2)) pinIDs |= GPIO_Pin_2;
        if(pins & (1 << 3)) pinIDs |= GPIO_Pin_3;
        if(pins & (1 << 4)) pinIDs |= GPIO_Pin_4;
        if(pins & (1 << 5)) pinIDs |= GPIO_Pin_5;
        if(pins & (1 << 6)) pinIDs |= GPIO_Pin_6;
        if(pins & (1 << 7)) pinIDs |= GPIO_Pin_7;
        if(pins & (1 << 8)) pinIDs |= GPIO_Pin_8;
        if(pins & (1 << 9)) pinIDs |= GPIO_Pin_9;
        if(pins & (1 << 10)) pinIDs |= GPIO_Pin_10;
        if(pins & (1 << 11)) pinIDs |= GPIO_Pin_11;
        if(pins & (1 << 12)) pinIDs |= GPIO_Pin_12;
        if(pins & (1 << 13)) pinIDs |= GPIO_Pin_13;
        if(pins & (1 << 14)) pinIDs |= GPIO_Pin_14;
        if(pins & (1 << 15)) pinIDs |= GPIO_Pin_15;

        return pinIDs;
    }

    uint16_t getPinIDByNo(uint8_t pinNo) {
        switch(pinNo){
            case 0: return GPIO_Pin_0;
            case 1: return GPIO_Pin_1;
            case 2: return GPIO_Pin_2;
            case 3: return GPIO_Pin_3;
            case 4: return GPIO_Pin_4;
            case 5: return GPIO_Pin_5;
            case 6: return GPIO_Pin_6;
            case 7: return GPIO_Pin_7;
            case 8: return GPIO_Pin_8;
            case 9: return GPIO_Pin_9;
            case 10: return GPIO_Pin_10;
            case 11: return GPIO_Pin_11;
            case 12: return GPIO_Pin_12;
            case 13: return GPIO_Pin_13;
            case 14: return GPIO_Pin_14;
            case 15: return GPIO_Pin_15;
            default:
                // Reporter::reportError("getPinIDByNo()", "Bad pin nr", true);
                return 0; // never reached, reportError() halts
        }
    }

    GPIOMode_TypeDef getModeID(Mode mode){
        switch(mode){
            case Modes::DIGITAL_IN: return GPIO_Mode_IN;
            case Modes::DIGITAL_OUT: return GPIO_Mode_OUT;
            case Modes::ALT_FUNCTION: return GPIO_Mode_AF;
            case Modes::ANALOG: return GPIO_Mode_AN;
            default:
                // Reporter::reportError("getModeID()", "Bad mode", true);
                return GPIO_Mode_IN; // never reached, reportError() halts
        }
    }

    GPIOOType_TypeDef getOutputTypeID(OutputType outputType){
        switch(outputType){
            case OutputTypes::PUSH_PULL: return GPIO_OType_PP;
            case OutputTypes::OPEN_DRAIN: return GPIO_OType_OD;
            default:
                // Reporter::reportError("getOutputTypeID()", "Bad type", true);
                return GPIO_OType_PP; // never reached, reportError() halts
        }
    }

    GPIOSpeed_TypeDef getSpeedID(Speed speed){
        switch(speed){
            case Speeds::LOW: return GPIO_Low_Speed;
            case Speeds::MEDIUM: return GPIO_Medium_Speed;
            case Speeds::HIGH: return GPIO_High_Speed;
            case Speeds::FAST: return GPIO_Fast_Speed;
            default:
                // Reporter::reportError("getSpeedID()", "Bad speed", true);
                return GPIO_Low_Speed; // never reached, reportError() halts
        }
    }

    GPIOPuPd_TypeDef getPullSettingID(PullSetting pullSetting){
        switch(pullSetting){
            case PullSettings::NOPULL: return GPIO_PuPd_NOPULL;
            case PullSettings::UP: return GPIO_PuPd_UP;
            case PullSettings::DOWN: return GPIO_PuPd_DOWN;
            default:
                // Reporter::reportError("getPullSettingID()", "Bad setting", true);
                return GPIO_PuPd_NOPULL; // never reached, reportError() halts
        }
    }
}