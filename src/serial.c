#include "serial.h"

void init_serial_gpio() {
  // RCC
  RCC_AHB1PeriphClockCmd(SERIAL_TX_GPIO_RCC, ENABLE);
  RCC_AHB1PeriphClockCmd(SERIAL_RX_GPIO_RCC, ENABLE);

  // Ports/pins
  GPIO_InitTypeDef gpioInitStruct;
  gpioInitStruct.GPIO_Pin = SERIAL_TX_GPIO_PIN;
  gpioInitStruct.GPIO_Mode = GPIO_Mode_AF;
  gpioInitStruct.GPIO_OType = GPIO_OType_PP;
  gpioInitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  gpioInitStruct.GPIO_Speed = GPIO_Fast_Speed;
  GPIO_Init(SERIAL_TX_GPIO_PORT, &gpioInitStruct);

  gpioInitStruct.GPIO_Pin = SERIAL_RX_GPIO_PIN;
  gpioInitStruct.GPIO_Mode = GPIO_Mode_AF;
  gpioInitStruct.GPIO_OType = GPIO_OType_PP;
  gpioInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  gpioInitStruct.GPIO_Speed = GPIO_Fast_Speed;
  GPIO_Init(SERIAL_RX_GPIO_PORT, &gpioInitStruct);

  // Alternate functions
  GPIO_PinAFConfig(SERIAL_TX_GPIO_PORT, SERIAL_TX_GPIO_SRC, SERIAL_AF_MAPPING);
  GPIO_PinAFConfig(SERIAL_RX_GPIO_PORT, SERIAL_RX_GPIO_SRC, SERIAL_AF_MAPPING);
}

void init_usart() {
  // RCC
  RCC_APB1PeriphClockCmd(SERIAL_USART_RCC, ENABLE);

  // UART
  USART_InitTypeDef usartInitStruct;
  usartInitStruct.USART_BaudRate = SERIAL_BAUD_RATE;
  usartInitStruct.USART_WordLength = USART_WordLength_8b;
  usartInitStruct.USART_StopBits = USART_StopBits_1;
  usartInitStruct.USART_Parity = USART_Parity_No;
  usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(SERIAL_USART_PERIPH, &usartInitStruct);

  USART_Cmd(SERIAL_USART_PERIPH, ENABLE);
}

void init_serial() {
  init_serial_gpio();
  init_usart();
}

int serial_check_transmission_complete() {
  return USART_GetFlagStatus(SERIAL_USART_PERIPH, USART_FLAG_TC);
}

void serial_wait_for_transmission_complete() {
  while (!serial_check_transmission_complete())
    ;
}

int serial_check_for_data() {
  return USART_GetFlagStatus(SERIAL_USART_PERIPH, USART_FLAG_RXNE);
}

void serial_put(char c) { USART_SendData(SERIAL_USART_PERIPH, c); }

void serial_puts(const char *str) {
  for (int i = 0; str[i] != '\0'; ++i) {
    serial_wait_for_transmission_complete();
    serial_put(str[i]);
  }
}

char serial_get() {
  return (char)(USART_ReceiveData(SERIAL_USART_PERIPH) & 0xff);
}
