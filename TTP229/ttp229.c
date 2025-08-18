#include "ttp229.h"
#include "usart.h"
#include "main.h"
#include "delay.h"
void TTP229_Init(void){
//
//  //SDO  当ttp229接收到触摸后，向MCU发出接触信号
//  GPIO_InitTypeDef  GPIO_InitStructure;
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//  GPIO_InitStructure.GPIO_Pin = TTP229_SDO ;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_Init(TTP229_PORT, &GPIO_InitStructure);
//
//  //SCL 当MCU接收到触摸信号后，向ttp229发送时序，读取键值
//  GPIO_InitStructure.GPIO_Pin = TTP229_SCL ;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//
//  GPIO_Init(TTP229_PORT, &GPIO_InitStructure);
//
//  //当TP1=0,TP2=0时，应在SDO和SCL初始化时将电平拉低
	HAL_GPIO_WritePin(TTP229_SCL_GPIO_Port, TTP229_SCL_Pin, RESET);
	HAL_GPIO_WritePin(TTP229_SDO_GPIO_Port, TTP229_SDO_Pin, RESET);
//
}

uint8_t ttp229_read(void)
{
	//uint16_t n=0;
	uint8_t i;

	if(HAL_GPIO_ReadPin(TTP229_SDO_GPIO_Port, TTP229_SDO_Pin))
	{
		HAL_Delay(0);
		for(i=0;i<16;i++)
		{
			HAL_GPIO_WritePin(TTP229_SCL_GPIO_Port, TTP229_SCL_Pin, GPIO_PIN_SET);
			HAL_Delay(0);
			HAL_GPIO_WritePin(TTP229_SCL_GPIO_Port, TTP229_SCL_Pin, GPIO_PIN_RESET);
			HAL_Delay(0);
			if(HAL_GPIO_ReadPin(TTP229_SDO_GPIO_Port,TTP229_SDO_Pin))
			{
				break;
			}
		}
		if(i<16)
		{

			return i+1;
		}
		else return 0;
	}
	return 0;
}

//-------------------------配置引脚中断----------------------------
//uint8_t INT_MARK;

//void ttp229_INT_INIT(void){
//
//  NVIC_InitTypeDef  NVIC_InitStruct;	//定义结构体变量
//	EXTI_InitTypeDef  EXTI_InitStruct;
//
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //启动GPIO时钟 （需要与复用时钟一同启动）
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);//配置端口中断需要启用复用时钟
//
////第1个中断
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);  //定义 GPIO  中断
//
//	EXTI_InitStruct.EXTI_Line=EXTI_Line7;  //定义中断线
//	EXTI_InitStruct.EXTI_LineCmd=ENABLE;              //中断使能
//	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;     //中断模式为 中断
//	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;   //上升沿触发
//
//	EXTI_Init(& EXTI_InitStruct);
//
//	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;   //中断线
//	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //使能中断
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //抢占优先级 2
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;     //子优先级  2
//	NVIC_Init(& NVIC_InitStruct);
//
//}
//
//void  EXTI9_5_IRQHandler(void){
//
//	if(EXTI_GetITStatus(EXTI_Line7)!=RESET){//判断某个线上的中断是否发生
//		INT_MARK=2;//标志位置1，表示有按键中断
//		EXTI_ClearITPendingBit(EXTI_Line7);   //清除 LINE 上的中断标志位
//	}
//}
























