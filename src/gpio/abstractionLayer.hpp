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

#ifndef PROJECT_ABSTRACTIONLAYER_HPP
#define PROJECT_ABSTRACTIONLAYER_HPP

#include "enumsAndStructs.hpp"


namespace Gpio {

    /** Returns the ID (from the enum in enumsAndStructs.h) for the pin of the given number.
     *  @return uint16_t The ID
     */
    uint16_t getPinByNo(uint8_t pinNo);

    /** Enables the AHB1 clock for the given port.
     */
    void enablePeriphClock(Port port);

    /** Disables the AHB1 clock for the given port.
     */
    void disablePeriphClock(Port port);

    /** Configure GPIO pins based on the given Configuration struct.
     */
    void configurePins(Configuration configuration);

    /** Configure GPIO pins based on the given parameters.
     * @param Port port The port which the pins are part of.
     * @param uint16_t pins The pins to be configured as identified by a value or multiple OR'd values from Gpio::Pins.
     * @param Mode mode The mode to be used (e.g. input/output)
     * @param Speed speed The speed of the pins. Defaults to Speeds::FAST.
     * @param OutputType outputType The type of output. Defaults to OutputTypes::PUSH_PULL.
     * @param PullSetting pullSetting The pullup/pulldown setting. Defaults to PullSettings::NOPULL.
     */
    void configurePins(
            Port port,
            uint16_t pins,
            Mode mode,
            Speed speed = Speeds::FAST,
            OutputType outputType = OutputTypes::PUSH_PULL,
            PullSetting pullSetting = PullSettings::NOPULL
    );

    /**
     * Sets the given pins (makes them high).
     * @param Port port The port which the pins are part of.
     * @param uint16_t pins The pins to be set as identified by a value or multiple OR'd values from Gpio::Pins.
     */
    void setPins(Port port, uint16_t pins = PIN_ALL);

    /**
     * Clears the given pins (makes them low).
     * @param Port port The port which the pins are part of.
     * @param uint16_t pins The pins to be cleared as identified by a value or multiple OR'd values from Gpio::Pins.
     */
    void clearPins(Port port, uint16_t pins = PIN_ALL);

    /**
     * Toggles the given pins (makes them high if low and low if high).
     * @param Port port The port which the pins are part of.
     * @param uint16_t pins The pins to be toggled as identified by a value or multiple OR'd values from Gpio::Pins.
     */
    void togglePins(Port port, uint16_t pins = PIN_ALL);

    /**
     * Writes the given value to the given pins (single bool value, meaning all given pins will be made either high or low).
     * @param Port port The port which the pins are part of.
     * @param uint16_t pins The pins to be written to as identified by a value or multiple OR'd values from Gpio::Pins.
     * @param bool newValue The value to be written.
     */
    void writePins(Port port, uint16_t pins, bool newValue);

    /**
     * Writes the given value to the given port. Port should first be configured entirely for output.
     * @param Port port The port to write to.
     * @param uint16_t value The value to be written to the port.
     */
    void writePort(Port port, uint16_t value);

    /**
     * Reads the given input pin (where pin is identified by value from Gpio::Pins enum) and returns its value, behaviour for multiple pin input undefined.
     * @return bool Pin value.
     */
    bool readInputPin(Port port, uint16_t pin);

    /**
     * Reads the given input pin (where pin is identified by pin number) and returns its value.
     * @return bool Pin value.
     */
    bool readInputPinByNo(Port port, uint8_t pinNo);
}

#endif //PROJECT_ABSTRACTIONLAYER_HPP
