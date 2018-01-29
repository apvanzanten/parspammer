#ifndef SERIAL_H
#define SERIAL_H

#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>

#define SERIAL_TX_GPIO_RCC (RCC_AHB1Periph_GPIOA)
#define SERIAL_TX_GPIO_PORT (GPIOA)
#define SERIAL_TX_GPIO_PIN (GPIO_Pin_2)
#define SERIAL_TX_GPIO_SRC GPIO_PinSource2

#define SERIAL_RX_GPIO_RCC (RCC_AHB1Periph_GPIOA)
#define SERIAL_RX_GPIO_PORT (GPIOA)
#define SERIAL_RX_GPIO_PIN (GPIO_Pin_3)
#define SERIAL_RX_GPIO_SRC GPIO_PinSource3

#define SERIAL_USART_PERIPH (USART2)
#define SERIAL_USART_RCC (RCC_APB1Periph_USART2)
#define SERIAL_AF_MAPPING (GPIO_AF_USART2)
#define SERIAL_BAUD_RATE (9600)

void init_serial();

int serial_check_transmission_complete();
void serial_wait_for_transmission_complete();
int serial_check_for_data();

void serial_put(char c);
void serial_puts(const char *str);
char serial_get();
char serial_get_blocking();

#endif // SERIAL_H
