/*
 * sys.c
 *
 *  Created on: Aug 22, 2025
 *      Author: RickyHuo
 */
#include "main.h"
#include "sys.h"
#include "usart.h"

int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1,HAL_MAX_DELAY);
  return ch;
}
