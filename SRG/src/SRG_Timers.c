/*
 * SRG_Timers.c
 *
 *  Created on: Jun 14, 2018
 *      Author: Haris
 */

#include <stdint.h>
#include "SRG_Timers.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"

OnePulseSettings TIMOnePulse;

static void updateTimer(TIM_TypeDef* TIMx);

/**
 * ReadMe:
 * Ton = thetaON/45 * T; thetaON = (0 - 45)
 * Tof = thetaOFF/45 * T
 * T = counter * TIM5_Freq
 * TIM_Pulse = Ton/TIMx_CLK
 * TIM_Period = Toff*TIMx_CLK
 */

/***
 * Timer 2 Initialization
 */
void Timer2_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);

	/* TIM2 OnePulse Mode (initial values, will be changed through firmware)
	 * External signal is connected to TIM2_CH2 pin (PA1)
	 * The Rising edge is used as active edge
	 * The One Pulse signal is output on TIM2_CH1 pin (PA0)
	 * The TIM_Pulse defines the delay value
	 * The (TIM_Period - TIM_Pulse) defines the One Pulse value
	 * TIM2CLK = 42MHz, we want to get TIM2 counter clock at 16MHz:
	 * - Prescaler = (TIM2CLK / TIM2 counter clock) - 1
	 * The Autoreload is 65535, maximum frequency value to trigger the
	 * TIM2 input is 16000000/65535 = 244.144Hz
	 * The delay time is defined by TIM_Pulse: 16383/16000000 = 1.023ms
	 * One Pulse Value = (TIM_Period - TIM_Pulse) / TIM2 counter clock = 3.072ms
	 * */
	// Time Base configuration
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	uint16_t PrescalerValue = (uint16_t)(RCC_Clocks.PCLK1_Frequency / 31250) - 1;
	TIMOnePulse.TIM_Prescaler = PrescalerValue;
	updateTimer(TIM2);
}

/***
 * Timer 3 Initialization
 */
void Timer3_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);

	// Time Base configuration
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	uint16_t PrescalerValue = (uint16_t)(RCC_Clocks.PCLK1_Frequency / 31250) - 1;
	TIMOnePulse.TIM_Prescaler = PrescalerValue;
	updateTimer(TIM3);
}

/***
 * Timer 4 Initialization
 */
void Timer4_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	uint16_t PrescalerValue = (uint16_t)(RCC_Clocks.PCLK1_Frequency / 31250) - 1;
	TIMOnePulse.TIM_Prescaler = PrescalerValue;
	updateTimer(TIM4);
}

/***
 * Update Timers function
 * After changing TIMOnePulse struct values, need to call updateTimers function
 */
void updateTimers(void){
	updateTimer(TIM2);
	updateTimer(TIM3);
	updateTimer(TIM4);
}

static void updateTimer(TIM_TypeDef* TIMx){
	TIM_DeInit(TIMx);
	TIM_TimeBaseInitTypeDef TIM_TBStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_TBStruct.TIM_Period = TIMOnePulse.TIM_Period;
	TIM_TBStruct.TIM_Prescaler = TIMOnePulse.TIM_Prescaler;
	TIM_OCInitStruct.TIM_Pulse = TIMOnePulse.TIM_Pulse;
	TIM_TBStruct.TIM_ClockDivision = 0;
	TIM_TBStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &TIM_TBStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIMx, &TIM_OCInitStruct);
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICFilter = 0;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);
	TIM_SelectOnePulseMode(TIMx, TIM_OPMode_Single);
	TIM_SelectInputTrigger(TIMx, TIM_TS_TI2FP2);
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Trigger);
}
