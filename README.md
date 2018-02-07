# Parspammer
This is a little STM32F4 project that spams arbitrary data (can be passed in via UART on startup) 16 bits at a time, at 20 megasamples per second. To achieve this it uses one of the DMA controllers to transfer data from a buffer to one of the GPIO ports. NOTE: Some minor overclocking was needed to get the right data rate, read below.

### Why?
Long story short, I am working on a project where we have an ADC sampling at 20MHz, and I want a controllable (read capable of producing specific given data) stand-in for this ADC for testing purposes. There's probably a whole heap of devices capable of doing this better than an STM32F4 can, but I happened to have this STM32F4 Discovery board lying around, and it seemed fun to see if it could do it.

### Hardware
STM32F4 Discovery board. For the purposes of this project that equates to: an STM32F407VG chip with an external crystal at 8MHz hooked up to the HSE clock input. This project will probably run reasonably well on other STM32F4 chips and boards, though probably some changes to the clock configuration will have to be made.

### Libraries used
STM32F4 Standard Peripheral Library. Old stuff, I know. I should be using the Cube HAL. However, the last time I did something with this chip, the situation was different, and I wanted to be able to reuse some old code.

### Toolchain/build system
To build the code, I use platformio. As far as I can tell, this project should build just fine out-of-the-box on with any up-to-date install of platformio, on any platform (it'll even download the toolchain and libraries for you, platformio is nice like that!).

To build:

    platformio run

To build and upload:

    platformio run --target upload

To clean:

    platformio run --target clean

### Overclock
To get the DMA clocked at 20MHz some minor overclocking was required:
* The system/core clock is at 180MHz, which is about 12MHz above what the STM32CubeMX utility indicates as maximum (though the STM reference manual seems to suggest that 180MHz is just within the maximum).
* The APB2 peripheral clock is at 90MHz, which is about 6MHz above what the STM32CubeMX utility indicates as maximum. As a result, certain peripherals can't be used, I haven't extensively looked into which or tested them, but I can at least say that I failed to get USART1 to work (probably just the baud rate is off).
* The APB2 timer clock is at 180MHz, which is about 12 MHz above what the STM32CubeMX indicates as maximum.

My particular STM32F407VG chip seems to run absolutely fine at these clock rates, and I suspect that the same goes for other STM32F4 chips, as I have overclocked it far higher (240MHz) without any issue (and have heard of other people doing this as well). That said, **I am not responsible for any problems or damage running this software might cause.**

### Usage
1. Hook USART2 pins up to whatever device you use as a serial terminal
2. Set the baud rate to 115200 (if a different baud rate is required, this is easily changable in serial.h via the SERIAL_BAUD_RATE define).
3. Boot up the STM32F4. If you have a STM32F4 Discovery board (or just happen to have a LED attached to pin D13 for some other reason), you should see a LED blink a few times, after which you should see the following message on your terminal:
```
    Hello! Welcome to parspammer, please enter the following, as raw data (little endian) with no delimiters:
    1. mode (one byte), pick from:
        0x00: continuous send
        0x01: one-shot send
        0x02: repeated one-shot send @ 1Hz
        0x03: repeated one-shot send @ 10Hz
        0x04: repeated one-shot send @ 100Hz
        0x05: repeated one-shot send @ 1000Hz
        0x64 ('d'): defaults (ignores given sample number and data)
    2. number of samples (unsigned integer, 2 bytes, max 8192)
    3. the actual samples of data (2 bytes each)
    RDY
```
After the RDY signal (that's what it's there for) you can start sending your mode, samples and data. It will start spamming the moment all data has been received.

### Configuring for other sample rates
If your desired rate is an integer divisor of 180MHz and no higher than 20MHz (and no lower than whatever 180MHz/65536 is, as the trigger timer uses a 16-bit counter), you should be able to just change the SPAM_SAMPLE_RATE define in time_and_dma.h. If it is not a divsior or if it is higher than 20MHz you will have to customize the clock configuration. In most cases this should just be a matter of adjusting some of the defines in clocks.h (see comments in clocks.h). In terms of going higher than 20MHz, I have not tried but I suspect that with more overclocking a sample rate of 25MHz or more could be reached.

### License
See the LICENSE file.

### Contact
E-mail me at [ap.vanzanten@gmail.com](mailto:ap.vanzanten@gmail.com).