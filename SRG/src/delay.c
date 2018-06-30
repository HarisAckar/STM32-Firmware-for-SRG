/*
 * delay.c
 *
 *  Created on: Jun 16, 2018
 *      Author: haris
 */

#include <stdint.h>
#include "delay.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

static void TIM12_InterruptEnable(void);

static uint8_t TIM12_Started = 0x00;

/***
 * Starting Timer 12 for User delay
 */
void TIM12_Start(void){
	TIM12_InterruptEnable();
}

static void TIM12_InitMS(uint32_t period_ms){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);

	TIM_InitStruct.TIM_Prescaler = 1000;
	TIM_InitStruct.TIM_Period = ((RCC_Clocks.PCLK1_Frequency * 2  / 1000) * period_ms) / 1000 - 1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM12, &TIM_InitStruct);
	TIM_ITConfig(TIM12, TIM_IT_Update, ENABLE);
	TIM12_InterruptEnable();
	TIM12_Started = 0x01;
}

static void TIM12_InitUS(uint32_t period_us){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);

	TIM_InitStruct.TIM_Prescaler = 1000;
	TIM_InitStruct.TIM_Period = ((RCC_Clocks.PCLK1_Frequency   / 1000) * period_us) / 10000 - 1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM12, &TIM_InitStruct);
	TIM_ITConfig(TIM12, TIM_IT_Update, ENABLE);
	TIM12_InterruptEnable();
	TIM12_Started = 0x01;
}

static void TIM12_DisableAll(void){
	TIM_ITConfig(TIM12, TIM_IT_Update, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, DISABLE);
	TIM_ITConfig(TIM12, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM12, DISABLE);
}

static void TIM12_InterruptEnable(void){
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM8_BRK_TIM12_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStruct);
	TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
	TIM_Cmd(TIM12, ENABLE);
}

/***
 * User delay in miliseconds
 */
void delay_ms(uint32_t time_ms){
	TIM12_InitMS(time_ms);
	while(TIM12_Started != 0x00);
	TIM12_DisableAll();
}

/***
 * User delay in microseconds
 */
void delay_us(uint32_t time_us){
	TIM12_InitUS((time_us * 17) / 10);
	while(TIM12_Started != 0x00);
	TIM12_DisableAll();
}

/***
 * Interrupt handler for TIM12
 */
void TIM8_BRK_TIM12_IRQHandler(void){
	if(TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET){
		TIM12_Started = 0x00;
		TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
	}
}




