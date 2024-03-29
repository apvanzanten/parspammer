#include "time_and_dma.h"

void init_main_timer() {
  RCC_APB1PeriphClockCmd(MAIN_TMR_RCC, ENABLE);
  TIM_TimeBaseInitTypeDef initStruct;
  initStruct.TIM_Prescaler = MAIN_TMR_PRESCALER - 1;
  initStruct.TIM_CounterMode = TIM_CounterMode_Up;
  initStruct.TIM_ClockDivision = TIM_CKD_DIV1;

  // - 1: timer counts up to (and incl.) the TIM*->ARR and then resets, thereby
  // the period is actually (TIM*->ARR)+1
  initStruct.TIM_Period = (uint32_t)(MAIN_TMR_PERIOD - 1);

  TIM_TimeBaseInit(MAIN_TMR, &initStruct);
  TIM_Cmd(MAIN_TMR, ENABLE);
}

void init_spam_timer() {
  RCC_APB2PeriphClockCmd(SPAM_TMR_RCC, ENABLE);
  TIM_TimeBaseInitTypeDef initStruct;
  initStruct.TIM_Prescaler = SPAM_TMR_PRESCALER - 1;
  initStruct.TIM_CounterMode = TIM_CounterMode_Up;
  initStruct.TIM_ClockDivision = TIM_CKD_DIV1; // clock division 1:1

  // - 1: timer counts up to (and incl.) the TIM*->ARR and then resets, thereby
  // the period is actually (TIM*->ARR)+1
  initStruct.TIM_Period = (uint32_t)(SPAM_TMR_PERIOD - 1);

  initStruct.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(SPAM_TMR, &initStruct);
  TIM_Cmd(SPAM_TMR, ENABLE);
}

void init_spam_repeat_interrupt(){
  // Initialize interrupt, the handler of which will reset the DMA.
  NVIC_InitTypeDef nvicStructure;
  nvicStructure.NVIC_IRQChannel = REPEAT_TMR_IRQ;
  nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
  nvicStructure.NVIC_IRQChannelSubPriority = 1;
  nvicStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicStructure);
}

void init_spam_repeat_timer(uint32_t period) {
  init_spam_repeat_interrupt();

  // Initialize the timer 
  RCC_APB1PeriphClockCmd(REPEAT_TMR_RCC, ENABLE);
  TIM_TimeBaseInitTypeDef initStruct;
  initStruct.TIM_Prescaler = REPEAT_TMR_PRESCALER - 1;
  initStruct.TIM_CounterMode = TIM_CounterMode_Up;
  initStruct.TIM_ClockDivision = TIM_CKD_DIV1;

  // - 1: timer counts up to (and incl.) the TIM*->ARR and then resets, thereby
  // the period is actually (TIM*->ARR)+1
  initStruct.TIM_Period = (uint32_t)(period - 1);

  TIM_TimeBaseInit(REPEAT_TMR, &initStruct);
  TIM_ITConfig(REPEAT_TMR, TIM_IT_Update, ENABLE); 
  TIM_Cmd(REPEAT_TMR, ENABLE);
}

void init_spam_dma(uint16_t *buffer, int is_continuous) {
  // Initialize the DMA stream to transfer from the given buffer to GPIOD.
  DMA_DeInit(DMA_STREAM);
  RCC_AHB1PeriphClockCmd(RCC_DMA_PERIPH_CLK, ENABLE);

  DMA_InitTypeDef dmaInitStruct;
  dmaInitStruct.DMA_Channel = DMA_CHANNEL;
  dmaInitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&GPIOD->ODR);
  dmaInitStruct.DMA_Memory0BaseAddr = (uint32_t)buffer;
  dmaInitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  dmaInitStruct.DMA_BufferSize = 0; // buffer size will be set later
  dmaInitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dmaInitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dmaInitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  dmaInitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  dmaInitStruct.DMA_Mode = is_continuous ? DMA_Mode_Circular : DMA_Mode_Normal;
  dmaInitStruct.DMA_Priority = DMA_Priority_High;
  dmaInitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
  dmaInitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  dmaInitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  dmaInitStruct.DMA_PeripheralBurst = DMA_MemoryBurst_Single;
  DMA_Init(DMA_STREAM, &dmaInitStruct);

  // set timer to send trigger events to DMA.
  TIM_DMACmd(SPAM_TMR, TIM_DMA_Update, ENABLE);
}

void start_spam_dma(uint16_t buffer_size) {
  DMA_SetCurrDataCounter(DMA_STREAM, buffer_size);
  DMA_ClearFlag(DMA_STREAM, DMA_FLAGS);
  DMA_Cmd(DMA_STREAM, ENABLE);
}

uint32_t get_main_tmr_ticks() { return MAIN_TMR->CNT; }

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
