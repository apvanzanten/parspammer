/*  Copyright (c) 2015 Arjen Pieter van Zanten
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*	SOFTWARE.
*/

#include "DigitalPin.hpp"
#include "abstractionLayer.hpp"

namespace Gpio {

    void DigitalPin::updateConfiguration() {
        configurePins(config);
    }

    DigitalPin::DigitalPin(Port port,
                           uint8_t pinNo,
                           Direction direction,
                           Speed speed,
                           OutputType outputType,
                           PullSetting pullSetting
    ) : pinNo(pinNo), config() {
        config.port = port;
        config.pins = getPinByNo(pinNo);

        if(direction == IN) {
            config.mode = Modes::DIGITAL_IN;
        } else if (direction == OUT){
            config.mode = Modes::DIGITAL_OUT;
        } else {
            // TODO better error handling
        }

        config.speed = speed;
        config.outputType = outputType;
        config.pullSetting = pullSetting;

        enablePeriphClock(port);
        updateConfiguration();

        clear(); // initialize output in off mode
    }

    DigitalPin::~DigitalPin() {
        if(getDirection() == OUT) clear();
        // TODO figure out how to properly deinit an individual pin
    }

    void DigitalPin::setDirection(DigitalPin::Direction newDirection) {
        config.mode = (Mode)newDirection;
        updateConfiguration();
    }

    void DigitalPin::setSpeed(Speed newSpeed) {
        config.speed = newSpeed;
        updateConfiguration();
    }

    void DigitalPin::setOutputType(OutputType newOutputType) {
        config.outputType = newOutputType;
        updateConfiguration();
    }

    void DigitalPin::setPullSetting(PullSetting newPullSetting) {
        config.pullSetting = newPullSetting;
        updateConfiguration();
    }

    bool DigitalPin::read() {
        if (getDirection() == IN){
            return readInputPinByNo(config.port, pinNo);
        } else {
            return state;
        }
    }

    void DigitalPin::setState(bool newState) {
        if(getDirection() == OUT) {
            if (newState) {
                setPins(config.port, config.pins);
            } else {
                clearPins(config.port, config.pins);
            }
            state = newState;
        } else {
            // No error handling here because all feasible output methods use this very function.
        }
    }

    void DigitalPin::set() {
        setState(true);
    }

    void DigitalPin::clear() {
        setState(false);
    }

    void DigitalPin::toggle() {
        setState(!state);
    }
}
