/*
 * aht20.c
 *
 *  Created on: Jul 30, 2024
 *      Author: RickyHuo
 */


#include "aht20.h"

#define AHT20_ADDR 0x38<<1

void AHT20_Init(void)
{
	HAL_Delay(40);
	uint8_t readBuffer;
	HAL_I2C_Master_Receive(&hi2c2, AHT20_ADDR, &readBuffer, 1, HAL_MAX_DELAY);
	if((readBuffer&0x08)==0x00)
	{
		uint8_t sendBuffer[]={0xBE,0x08,0x00};
		HAL_I2C_Master_Transmit(&hi2c2, AHT20_ADDR, sendBuffer, 3, HAL_MAX_DELAY);
	}
}

void AHT20_read(float *temp,float *hum)
{
	uint8_t send[]={0xAC,0x33,0x00};
	uint8_t read[6];
	HAL_I2C_Master_Transmit(&hi2c2, AHT20_ADDR, send, 3, HAL_MAX_DELAY);
	HAL_Delay(75);
	HAL_I2C_Master_Receive(&hi2c2, AHT20_ADDR, read, 6, HAL_MAX_DELAY);

	if((read[0]&0x80)==0x00)//NOTICE: AND
	{
		uint32_t data;
		data= ((uint32_t)read[1]<<12)+((uint32_t)read[2]<<4)+((uint32_t)read[3]>>4);//NOTICE: MUST convert to uint32_t
		*hum= data*100.0f/(1<<20);

		data=(((uint32_t)(read[3]&0x0F))<<16)+((uint32_t)read[4]<<8)+((uint32_t)read[5]);
		*temp=data*200.0f/(1<<20)-50;
	}
}
