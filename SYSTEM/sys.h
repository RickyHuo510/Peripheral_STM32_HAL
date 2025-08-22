/*
 * sys.h
 *
 *  Created on: Aug 22, 2025
 *      Author: RickyHuo
 */

#ifndef SYS_H_
#define SYS_H_

#include "main.h"

#define u32 uint32_t
#define u16 uint16_t
#define u8 uint8_t
/*
 * if you want to use this function to redirect the output stream,
 * put "sys.h" before <stdio.h>
 * */

int __io_putchar(int ch);

#endif /* SYS_H_ */
