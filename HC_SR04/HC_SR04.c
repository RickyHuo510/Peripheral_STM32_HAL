/*
 * HC_SR04.c
 *
 *  Created on: Jun 17, 2025
 *      Author: RickyHuo
 */

/*************************************************
 * Before using the lib: turn on IC mode on TIM3,
 * set PC7 as HC_trig GPIO
 * set CHANNEL3 as direct mode, CH4 as indirect mode
 * set presecular 72-1 , period of 1us
 ******************************************************/
#include "main.h"
#include "tim.h"

int upEdge=0,downEdge=0;
float distance=0.0;

/*
 * This function is used in void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
 * check if(htim==&htim3 && htim->Channel==HAL_TIM_ACTIVE_CHANNEL_4)
 * before using this function
 */
void HC_SR04_INT_Handler()
{
	upEdge=HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_3);
	downEdge=HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_4);
	distance=(downEdge-upEdge)*0.034/2.0;
}

void HC_Start_Measure()
{
	HAL_GPIO_WritePin(HC_trig_GPIO_Port, HC_trig_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(HC_trig_GPIO_Port, HC_trig_Pin, GPIO_PIN_RESET);
	__HAL_TIM_SET_COUNTER(&htim3,0);
}

void HC_SR04_Start()
{
	  HAL_TIM_Base_Start(&htim3);
	  HAL_TIM_IC_Start(&htim3, TIM_CHANNEL_3);
	  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
}
