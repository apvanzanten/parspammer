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

#ifndef PROJECT_ENUMSANDSTRUCTS_HPP
#define PROJECT_ENUMSANDSTRUCTS_HPP

#include <stdint.h>

/** Namespace containing everything to do with the GPIO pins.
 */
namespace Gpio {

    /** Contains enum for port IDs (PORT A through K).
     */
    namespace Ports {
        enum value {
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K
        };
    }

    /** Typedef for port ID enum.
     */
    typedef Ports::value Port;

    /** Enum containing GPIO Pin IDs, can be combined with bitwise OR (e.g. PIN_0 | PIN_1).
     */
    enum Pins {
        PIN_0   = 1 << 0,
        PIN_1   = 1 << 1,
        PIN_2   = 1 << 2,
        PIN_3   = 1 << 3,
        PIN_4   = 1 << 4,
        PIN_5   = 1 << 5,
        PIN_6   = 1 << 6,
        PIN_7   = 1 << 7,
        PIN_8   = 1 << 8,
        PIN_9   = 1 << 9,
        PIN_10  = 1 << 10,
        PIN_11  = 1 << 11,
        PIN_12  = 1 << 12,
        PIN_13  = 1 << 13,
        PIN_14  = 1 << 14,
        PIN_15  = 1 << 15,
        PIN_ALL = 0xff
    };

    /** Contains enum of different GPIO pin modes.
     */
    namespace Modes {
        enum value {
            DIGITAL_IN,
            DIGITAL_OUT,
            ALT_FUNCTION,
            ANALOG
        };
    }

    /** Typedef for GPIO pin mode enum.
     */
    typedef Modes::value Mode;

    /** Contains enumeration of GPIO pin output types.
     */
    namespace OutputTypes {
        enum value {
            PUSH_PULL,
            OPEN_DRAIN
        };
    }

    /** Typedef for GPIO pin output types.
     */
    typedef OutputTypes::value OutputType;

    /** Contains enumeration of GPIO pin speed settings.
     */
    namespace Speeds {
        enum value {
            LOW,
            MEDIUM,
            FAST,
            HIGH
        };
    }

    /** Typedef for GPIO pin speed settings.
     */
    typedef Speeds::value Speed;

    /** Contains enum of GPIO pin pullup/pulldown resistor settings.
     */
    namespace PullSettings {
        enum value{
            NOPULL,
            UP,
            DOWN
        };
    }

    /** Typedef for GPIO pin pullup/pulldown resistor settings.
     */
    typedef PullSettings::value PullSetting;

    /** Struct containing a configuration of one or more pins in a GPIO port.
     */
    struct Configuration {
        Port port;
        uint16_t pins;
        Mode mode;
        OutputType outputType;
        Speed speed;
        PullSetting pullSetting;
    };
}

#endif //PROJECT_ENUMSANDSTRUCTS_HPP
