/*
 * oled.h
 *
 *  Created on: Aug 21, 2024
 *      Author: R1195
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "i2c.h"

void OLED_Test();
void OLED_Init(void);
void OLED_ShowFrame();
void OLED_GRAM_Refresh(uint8_t);
void OLED_Draw_Point(int x,int y,uint8_t color);
void OLED_showImage_CR(uint8_t *p, uint8_t w, uint8_t h, uint8_t x, uint8_t y );
void OLED_showImage_CC(uint8_t *p, uint8_t w, uint8_t h, uint8_t x, uint8_t y);
extern uint8_t coor_x;
extern uint8_t coor_y;
void OLED_printstr(char *str, uint8_t char_size, uint8_t x, uint8_t y);
void OLED_graph(float *p, int arr_length, uint8_t width, uint8_t height, uint8_t start_x, uint8_t start_y);

#endif /* INC_OLED_H_ */
