/*
 * delay.h
 *
 *  Created on: Jun 27, 2025
 *      Author: RickyHuo
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "main.h"

void delay_us_nop(uint32_t);
void delay_ms_none_int(uint16_t);
void delay_us_none_int(uint32_t);
void delay_us(uint32_t nus);

void delay_ms(uint16_t nms);

#endif /* DELAY_H_ */
