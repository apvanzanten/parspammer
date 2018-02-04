#include "clocks.h"
#include "serial.h"
#include "time_and_dma.h"
#include "spam.h"
#include <stdint.h>

#define ORANGE_PIN (GPIO_Pin_13)

void greet_led() {
  // NOTE: This function assumes GPIOD is defined as output.
  for (int i = 0; i < 10; i++) {
    GPIO_ToggleBits(GPIOD, ORANGE_PIN);
    wait(0.1);
  }
}

void greet_serial() {
  // NOTE: This function assumes the relevant USART peripheral is initialized.
  serial_puts("Hello!\n");
}

void greet() {
  // NOTE: This function assumes GPIOD is defined as output and the relevant
  // USART peripheral is initialized.
  greet_serial();
  greet_led();
}

void init_and_start_spam(){
  serial_puts("Welcome to parspammer, please enter the following, as raw data "
              "(little endian) with no delimiters:\n");
  serial_puts("1. mode (one byte), pick from:\n");
  serial_puts("\t0x00: continuous send\n");
  serial_puts("\t0x01: one-shot send\n");
  serial_puts("\t0x02: repeated one-shot send @ 1Hz\n");
  serial_puts("\t0x03: repeated one-shot send @ 10Hz\n");
  serial_puts("\t0x04: repeated one-shot send @ 100Hz\n");
  serial_puts("\t0x05: repeated one-shot sned @ 1000Hz\n");
  serial_puts(
      "\t0x64 ('d'): defaults (ignores given sample number and data)\n");
  serial_puts("2. number of samples (unsigned integer, 2 bytes, max 8192)\n");
  serial_puts("3. the actual samples of data (2 bytes each)\n");
  serial_puts("RDY\n");

  char mode = serial_get_blocking();
  uint16_t num_samples = 0;

  if (mode != MODE_DEFAULT) {
    char num_samples_upper = serial_get_blocking();
    char num_samples_lower = serial_get_blocking();
    num_samples = (num_samples_upper << 8) | num_samples_lower;

    if (num_samples > NUM_SAMPLES_MAX) {
      serial_puts(
          "ERROR: sample number too high. You'll have to reboot me now.");
      while (1)
        ;
    }
  }

  start_spam(mode, num_samples);
}

int main(void) {
  init_clocks();
  init_spam_gpio();
  init_main_timer();
  init_serial();

  greet();

  init_and_start_spam();

  return 0;
}