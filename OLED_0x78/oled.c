/*
 * oled.c
 *
 *  Created on: Aug 21, 2024
 *      Author: R1195
 */

#include "i2c.h"
#include <string.h>
#include "asc.h"
// #include "words.h"

#define OLED_ADDR 0x78

uint8_t GRAM[8][128] = {0}; // 8 is y axis, 128 is x axis
uint8_t coor_x=0,coor_y=0;

void OLED_GRAM_Refresh(uint8_t val)
{
	memset(GRAM, val, sizeof(GRAM));
	coor_x=0;
	coor_y=0;
}

void OLED_SendCmd(uint8_t cmd)
{
	uint8_t sendBuffer[2];
	sendBuffer[0] = 0x00;
	sendBuffer[1] = cmd;
	HAL_I2C_Master_Transmit(&hi2c2, OLED_ADDR, sendBuffer, 2, HAL_MAX_DELAY);
}

void OLED_Init(void)
{
	OLED_SendCmd(0xAE);
	OLED_SendCmd(0x02);
	OLED_SendCmd(0x10);
	OLED_SendCmd(0x40);
	OLED_SendCmd(0x81);
	OLED_SendCmd(0xCF);
	OLED_SendCmd(0xA1);
	OLED_SendCmd(0xC8);
	OLED_SendCmd(0xA6);
	OLED_SendCmd(0xA8);
	OLED_SendCmd(0x3F);
	OLED_SendCmd(0xD3);
	OLED_SendCmd(0x00);
	OLED_SendCmd(0xD5);
	OLED_SendCmd(0x80);
	OLED_SendCmd(0xD9);
	OLED_SendCmd(0xF1);
	OLED_SendCmd(0xDA);
	OLED_SendCmd(0x12);
	OLED_SendCmd(0xDB);
	OLED_SendCmd(0x30);
	OLED_SendCmd(0x20);
	OLED_SendCmd(0x02);
	OLED_SendCmd(0x8D);
	OLED_SendCmd(0x14);
	OLED_SendCmd(0xAF);
	OLED_GRAM_Refresh(0x00);
}

void OLED_Test()
{
	OLED_SendCmd(0xB0);
	OLED_SendCmd(0x02);
	OLED_SendCmd(0x10);

	uint8_t sendBuffer[] = {0x40, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
	HAL_I2C_Master_Transmit(&hi2c2, OLED_ADDR, sendBuffer, sizeof(sendBuffer), HAL_MAX_DELAY);
}

void OLED_ShowFrame()
{
	uint8_t sendBuffer[129];
	sendBuffer[0] = 0x40;
	for (uint8_t i = 0; i < 8; i++)
	{
		for (uint8_t j = 0; j < 128; j++)
		{
			sendBuffer[j + 1] = GRAM[i][j];
		}
		OLED_SendCmd(0xB0 + i);
		OLED_SendCmd(0x02); // Already added 2 , now the boarder is 0~127, 0~63
		OLED_SendCmd(0x10);
		HAL_I2C_Master_Transmit(&hi2c2, OLED_ADDR, sendBuffer, sizeof(sendBuffer), HAL_MAX_DELAY);
	}
}

void OLED_Draw_Point(int x, int y, uint8_t color) // for THIS oled, 0<=x<=125, 0<=y<=63
{
	if (color == 1)
		GRAM[y / 8][x] |= (1 << (y % 8));
	else if (color == 0)
		GRAM[y / 8][x] &= (uint8_t)(0xFEFF >> (8 - y % 8));
	else
		return;
}

void OLED_showImage_CR(uint8_t *p, uint8_t w, uint8_t h, uint8_t x, uint8_t y) //_CR = First column then row
{
	for (uint8_t i = 0; i < w; i++)
	{ // which column
		for (uint8_t j = 0; j < h / 8 + 1; j++)
		{ // which row, more specifically, with the help of the "if" here, j refers to which page
			for (uint8_t k = 0; k < 8; k++)
			{ // which bit
				if (j * 8 + k >= h)
					break; // boundary check
				OLED_Draw_Point(x + i, y + j * 8 + k, ((p[i + j * w] >> k) & 0x01));
			}
		}
	}
}

void OLED_showImage_CC(uint8_t *p, uint8_t w, uint8_t h, uint8_t x, uint8_t y) //_CC = Column after column
{
	uint8_t r = h / 8;
	if (h % 8 != 0)
		r++;
	for (uint8_t i = 0; i < w; i++)
		for (uint8_t j = 0; j < r; j++)
			for (uint8_t k = 0; k < 8; k++)
			{
				if (j * 8 + k >= h)
					break;
				OLED_Draw_Point(x + i, y + j * 8 + k, ((p[i * r + j] >> k) & 0x01));
			}
}


void OLED_printstr(char *str, uint8_t char_size, uint8_t x, uint8_t y)
{
	uint8_t k=16,max_char;
	uint8_t length=strlen(str);
	k=(char_size/2)*2;
	uint8_t (*cha)[k];
	max_char=128/(k/2);
	switch(char_size)
	{
		case 16: cha=ascii_16;break;
		case 15: cha=ascii_15;break;
		case 14: cha=ascii_14;break;
		case 13: cha=ascii_13;break;
		case 12: cha=ascii_12;break;
	}
	for(uint8_t i=0; i<length; i++)
	{
		if(*(str+i)-31<=0||*(str+i)-31>=96)continue;
		OLED_showImage_CC(cha[*(str+i)-31],char_size/2,char_size,x,y);
		x+=char_size/2;
		if((i+1)%max_char==0)
			{y+=char_size;x=0;}
	}
	coor_x=x;
	coor_y=y;
}

static float findMax(float *p, int length)
{
	float m;
	for(int i=0;i<length;i++)
	{
		if(p[i]>m)
			m=p[i];
	}
	return m;
}

static float findMin(float *p, int length)
{
	float m=1000.0;
	for(uint8_t i=0;i<length;i++)
	{
		if(p[i]<m)
		{
			m=p[i];
		}

	}
	return m;
}
void OLED_graph(float *p, int arr_length, uint8_t width, uint8_t height, uint8_t start_x, uint8_t start_y)
{
	float bot=findMin(p, arr_length);
	float top=findMax(p, arr_length);
	float gap=(top-bot)/(float)height;
	float mid=(top+bot)/2.0;
	int height_ofpoint=0;
	for(uint8_t i=arr_length-width;i<arr_length;i++)
	{
		height_ofpoint=0;
		while(p[i]>mid+gap*height_ofpoint)height_ofpoint++;
		while(p[i]<mid+gap*height_ofpoint)height_ofpoint--;
		OLED_Draw_Point(start_x+i-(arr_length-width), start_y+height/2-height_ofpoint, 1);
	}
}
