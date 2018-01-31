#include "clocks.h"
#include "serial.h"
#include "time_and_dma.h"
#include <stdint.h>
#include <stm32f4xx_gpio.h>

#define GREEN_PIN (GPIO_Pin_12)
#define ORANGE_PIN (GPIO_Pin_13)
#define NUM_SAMPLES_DEFAULT (4096)
#define NUM_SAMPLES_MAX (8192)

uint16_t num_samples = NUM_SAMPLES_DEFAULT;

enum Modes {
  MODE_CONT,
  MODE_ONESHOT,
  MODE_ONESHOT_REPEAT_1HZ,
  MODE_ONESHOT_REPEAT_10HZ,
  MODE_ONESHOT_REPEAT_100HZ,
  MODE_DEFAULT = 0x64,
};

void init_gpio() {
  GPIO_InitTypeDef initStruct;
  initStruct.GPIO_Pin = GPIO_Pin_All;
  initStruct.GPIO_Mode = GPIO_Mode_OUT;
  initStruct.GPIO_Speed = GPIO_High_Speed;
  initStruct.GPIO_OType = GPIO_OType_PP;
  initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &initStruct);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
}

void greet_led() {
  for (int i = 0; i < 10; i++) {
    GPIO_ToggleBits(GPIOD, ORANGE_PIN);
    wait(0.1);
  }
}

void greet_serial() { serial_puts("Hello!\n"); }

void greet() {
  greet_serial();
  greet_led();
}

void start_spam(uint16_t *buffer, uint16_t buffer_size, char mode) {
  init_spam_timer();

  switch (mode) {
  case MODE_CONT:
    init_spam_dma(buffer, buffer_size, 1);
    DMA_Cmd(DMA_STREAM, ENABLE);
    break;
  case MODE_ONESHOT:
    init_spam_dma(buffer, buffer_size, 0);
    DMA_Cmd(DMA_STREAM, ENABLE);
    break;
  case MODE_ONESHOT_REPEAT_1HZ:
    init_spam_dma(buffer, buffer_size, 0);
    init_spam_repeat_timer(REPEAT_TMR_RATE);
    break;
  case MODE_ONESHOT_REPEAT_10HZ:
    init_spam_dma(buffer, buffer_size, 0);
    init_spam_repeat_timer(REPEAT_TMR_RATE / 10);
    break;
  case MODE_ONESHOT_REPEAT_100HZ:
    init_spam_dma(buffer, buffer_size, 0);
    init_spam_repeat_timer(REPEAT_TMR_RATE / 100);
    break;
  default:
    init_spam_dma(buffer, buffer_size, 0);
    DMA_Cmd(DMA_STREAM, ENABLE);
    break;
  }

  while (1) {
    __asm__("nop");
    // we don't want to do anything here, don't disturb the spam!
  }
}

void receive_samples(uint16_t *buffer, uint16_t num_samples) {
  for (uint16_t i = 0; i < num_samples; i++) {
    buffer[i] = (serial_get_blocking() << 8) | serial_get_blocking();
  }
}

void TIM5_IRQHandler() {
  if (TIM_GetITStatus(REPEAT_TMR, TIM_IT_Update) != RESET) {
    TIM_ClearITPendingBit(REPEAT_TMR, TIM_IT_Update);
    DMA_SetCurrDataCounter(DMA_STREAM, num_samples);
    DMA_ClearFlag(DMA_STREAM, DMA_FLAGS);
    DMA_Cmd(DMA_STREAM, ENABLE);
  }
}

int main(void) {
  init_clocks();
  init_gpio();
  init_main_timer();
  init_serial();

  greet();

  serial_puts("Welcome to parspammer, please enter the following, as raw data "
              "(little endian) with no delimiters:\n");
  serial_puts("1. mode (one byte), pick from:\n");
  serial_puts("\t0x00: continuous send\n");
  serial_puts("\t0x01: one-shot send\n");
  serial_puts("\t0x02: repeated one-shot send @ 1Hz\n");
  serial_puts("\t0x03: repeated one-shot send @ 10Hz\n");
  serial_puts("\t0x04: repeated one-shot send @ 100Hz\n");
  serial_puts(
      "\t0x64 ('d'): defaults (ignores given sample number and data)\n");
  serial_puts("2. number of samples (unsigned integer, 2 bytes, max 8192)\n");
  serial_puts("3. the actual samples of data (2 bytes each)\n");
  serial_puts("RDY\n");

  char mode = serial_get_blocking();

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

  uint16_t buffer[num_samples];

  switch (mode) {
  case MODE_CONT:
    receive_samples(buffer, num_samples);
    serial_puts("Thanks! Initiating continuous spam!");
    break;
  case MODE_ONESHOT:
    receive_samples(buffer, num_samples);
    serial_puts("Thanks! Initiating one-shot spam!");
    break;
  case MODE_ONESHOT_REPEAT_1HZ:
    receive_samples(buffer, num_samples);
    serial_puts("Thanks! Initiating repeated one-shot (1Hz) spam!");
    break;
  case MODE_ONESHOT_REPEAT_10HZ:
    receive_samples(buffer, num_samples);
    serial_puts("Thanks! Initiating repeated one-shot (10Hz) spam!");
    break;
  case MODE_ONESHOT_REPEAT_100HZ:
    receive_samples(buffer, num_samples);
    serial_puts("Thanks! Initiating repeated one-shot (100Hz) spam!");
    break;
  case MODE_DEFAULT:
    mode = MODE_CONT;
    for (uint16_t i = 0; i < num_samples; i++) {
      buffer[i] = i;
    }
    serial_puts("Thanks! Initiating default spam!");
    break;
  default:
    serial_puts("ERROR: unknown mode. You'll have to reboot me now.");
    while (1)
      ;
  }

  start_spam(buffer, num_samples, mode);

  return 0;
}