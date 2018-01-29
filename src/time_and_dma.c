#include "time_and_dma.h"

void init_spam_timer() {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  TIM_TimeBaseInitTypeDef initStruct;
  initStruct.TIM_Prescaler = SPAM_TMR_PRESCALER - 1; // prescaler 1:1
  initStruct.TIM_CounterMode = TIM_CounterMode_Up;
  initStruct.TIM_ClockDivision = TIM_CKD_DIV1; // clock division 1:1

  // - 1: timer counts up to (and incl.) the TIM*->ARR and then resets, thereby
  // the period is actually (TIM*->ARR)+1
  initStruct.TIM_Period = (uint32_t)(SPAM_TMR_PERIOD - 1);

  initStruct.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &initStruct);
  TIM_Cmd(TIM1, ENABLE);
}

void init_spam_dma(uint16_t *buffer, uint16_t buffer_size) {
  // DMA init
  DMA_DeInit(DMA_STREAM);
  RCC_AHB1PeriphClockCmd(RCC_DMA_PERIPH_CLK, ENABLE);

  DMA_InitTypeDef dmaInitStruct;
  dmaInitStruct.DMA_Channel = DMA_CHANNEL;
  dmaInitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&GPIOD->ODR);
  dmaInitStruct.DMA_Memory0BaseAddr = (uint32_t)buffer;
  dmaInitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  dmaInitStruct.DMA_BufferSize = buffer_size;
  dmaInitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dmaInitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dmaInitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  dmaInitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  dmaInitStruct.DMA_Mode = DMA_Mode_Circular;
  dmaInitStruct.DMA_Priority = DMA_Priority_High;
  dmaInitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
  dmaInitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  dmaInitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  dmaInitStruct.DMA_PeripheralBurst = DMA_MemoryBurst_Single;
  DMA_Init(DMA_STREAM, &dmaInitStruct);

  DMA_Cmd(DMA_STREAM, ENABLE);
  TIM_DMACmd(TIM1, TIM_DMA_Update, ENABLE);
}

void init_main_timer() {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TIM_TimeBaseInitTypeDef initStruct;
  initStruct.TIM_Prescaler = MAIN_TMR_PRESCALER - 1;
  initStruct.TIM_CounterMode = TIM_CounterMode_Up;
  initStruct.TIM_ClockDivision = TIM_CKD_DIV1;

  // - 1: timer counts up to (and incl.) the TIM*->ARR and then resets, thereby
  // the period is actually (TIM*->ARR)+1
  initStruct.TIM_Period = (uint32_t)(MAIN_TMR_PERIOD - 1);

  TIM_TimeBaseInit(TIM2, &initStruct);
  TIM_Cmd(TIM2, ENABLE);
}

uint32_t get_main_tmr_ticks() { return TIM2->CNT; }

void wait_ticks(uint32_t time_ticks) {
  // Overflow protection assumes that time_ticks < 2 * TMR_PERIOD at all times.
  // So, as long as TMR_PERIOD is more than half of UINT32_MAX it should be
  // fine.
  if (time_ticks == 0)
    return;

  uint32_t start = get_main_tmr_ticks();
  uint64_t desiredEnd = start + time_ticks;
  if (desiredEnd >= MAIN_TMR_PERIOD) { // we have overflow
    desiredEnd -= MAIN_TMR_PERIOD;
    while (get_main_tmr_ticks() > desiredEnd)
      ; // wait for our timer to overflow as well
  }

  while (get_main_tmr_ticks() < desiredEnd)
    ;
}

void wait(float time_sec) { wait_ticks(time_sec * MAIN_TMR_RATE); }
