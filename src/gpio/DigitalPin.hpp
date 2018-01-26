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

#ifndef SACROSONICMK3_DIGITALPIN_HPP
#define SACROSONICMK3_DIGITALPIN_HPP

#include "enumsAndStructs.hpp"

namespace Gpio {
    /**
     * Class representing a digital pin (i.e. a GPIO pin configured as a digital in or out). Handles initialisiation of
     * the pin and facilitates interaction with it.
     */
    class DigitalPin {
    private:
        uint8_t pinNo;
        Configuration config;
        bool state;

        void updateConfiguration();

    public:

        /**
         * Enumeration containing the possible directions (i.e. input or output).
         */
        enum Direction {
            IN, OUT
        };

        /**
         * Constructor for the DigitalPin class. Takes care of hardware initialisation.
         * @param port The GPIO port the pin is part of (e.g. voor PA2, this would be A).
         * @param pinNo The number of the pin (e.g. voor PA2 this would be 2), should be at least 0 and at most 15.
         * @param direction The direction (input/output) of the pin.
         * @param speed The speed to set the pin to.
         * @param outputType The output type (e.g. push-pull, open drain).
         * @param pullSetting The pull setting (e.g. pull up/down).
         */
        DigitalPin(
                Port port,
                uint8_t pinNo,
                Direction direction,
                Speed speed,
                OutputType outputType,
                PullSetting pullSetting
        );

        /**
         * Destructor. Currently does not deinitialise hardware (TODO)
         */
        ~DigitalPin();

        /**
         * Sets the direction of the pin.
         * @param newDirection The new direction.
         */
        void setDirection(Direction newDirection);

        /**
         * Sets the speed of the pin.
         * @param newSpeed The new speed.
         */
        void setSpeed(Speed newSpeed);

        /**
         * Sets the outputType of the pin.
         *
         */
        void setOutputType(OutputType newOutputType);
        void setPullSetting(PullSetting newPullSetting);

        bool read();

        void setState(bool newState);

        void set();
        void clear();
        void toggle();

        const Port getPort() const {
            return config.port;
        }

        uint16_t getPin() const {
            return (uint16_t) config.pins;
        }

        uint8_t getPinNo() const {
            return pinNo;
        }

        Direction getDirection() const {
            return (Direction)(config.mode == Modes::DIGITAL_OUT);
        }

        Speed getSpeed() const {
            return config.speed;
        }

        OutputType getOutputType() const {
            return config.outputType;
        }


        PullSetting getPullSetting() const {
            return config.pullSetting;
        }
    };
}

#endif //SACROSONICMK3_DIGITALOUT_HPP
