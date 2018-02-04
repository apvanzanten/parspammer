#include "spam.h"

void init_spam_gpio() {
  GPIO_InitTypeDef initStruct;
  initStruct.GPIO_Pin = GPIO_Pin_All;
  initStruct.GPIO_Mode = GPIO_Mode_OUT;
  initStruct.GPIO_Speed = GPIO_High_Speed;
  initStruct.GPIO_OType = GPIO_OType_PP;
  initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &initStruct);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
}

void receive_samples(uint16_t *buffer, uint16_t num_samples) {
  for (uint16_t i = 0; i < num_samples; i++) {
    const char upper_byte = serial_get_blocking();
    const char lower_byte = serial_get_blocking();
    buffer[i] = (upper_byte << 8) | lower_byte;
  }
}

void generate_default_samples(uint16_t *buffer, uint16_t num_samples) {
  for (uint16_t i = 0; i < num_samples; i++) {
    buffer[i] = i;
  }
}

// needs to be stored globally so it can be used in interrupt handler.
uint16_t g_num_samples = NUM_SAMPLES_DEFAULT;

void start_spam(char mode, uint16_t num_samples) {
  if (num_samples != 0) {
    g_num_samples = num_samples;
  }
  uint16_t buffer[num_samples];

  init_spam_timer();
  init_spam_dma(buffer, (mode == MODE_CONT));

  switch (mode) {
  case MODE_CONT:
    receive_samples(buffer, num_samples);
    serial_puts("Thanks! Initiating continuous spam!");
    start_spam_dma(num_samples);
    break;
  case MODE_ONESHOT:
    receive_samples(buffer, num_samples);
    serial_puts("Thanks! Initiating one-shot spam!");
    start_spam_dma(num_samples);
    break;
  case MODE_ONESHOT_REPEAT_1HZ:
    receive_samples(buffer, num_samples);
    serial_puts("Thanks! Initiating repeated one-shot (1Hz) spam!");
    init_spam_repeat_timer(REPEAT_TMR_RATE);
    break;
  case MODE_ONESHOT_REPEAT_10HZ:
    receive_samples(buffer, num_samples);
    serial_puts("Thanks! Initiating repeated one-shot (10Hz) spam!");
    init_spam_repeat_timer(REPEAT_TMR_RATE / 10);
    break;
  case MODE_ONESHOT_REPEAT_100HZ:
    receive_samples(buffer, num_samples);
    serial_puts("Thanks! Initiating repeated one-shot (100Hz) spam!");
    init_spam_repeat_timer(REPEAT_TMR_RATE / 100);
    break;
  case MODE_DEFAULT:
    generate_default_samples(buffer, num_samples);
    serial_puts("Thanks! Initiating default spam!");
    start_spam_dma(num_samples);
    break;
  default:
    serial_puts("ERROR: unknown mode. You'll have to reboot me now.");
    break;
  }

  while (1) {
    __asm__("nop");
    // we don't want to do anything here, don't disturb the spam!
  }
}

void TIM5_IRQHandler() {
  if (TIM_GetITStatus(REPEAT_TMR, TIM_IT_Update) != RESET) {
    TIM_ClearITPendingBit(REPEAT_TMR, TIM_IT_Update);
    start_spam_dma(g_num_samples);
  }
}
