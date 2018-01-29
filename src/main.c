#include <stm32f4xx_gpio.h>
#include <stdint.h>
#include "clocks.h"
#include "time_and_dma.h"

#define GREEN_PIN (GPIO_Pin_12)
#define ORANGE_PIN (GPIO_Pin_13)
#define BUFFER_SIZE (4096)

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

void greet() {
  for (int i = 0; i < 10; i++) {
    GPIO_ToggleBits(GPIOD, ORANGE_PIN);
    wait(0.1);
  }
}

int main(void) {
  init_clocks();
  init_gpio();
  init_main_timer();

  greet();


  uint16_t buffer[BUFFER_SIZE];
  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = i;
  }

  init_spam(buffer, BUFFER_SIZE);

  while (1) {
    __asm__("nop");
  }

  return 0;
}