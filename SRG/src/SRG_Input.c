/*
 * SRG_Input.c
 *
 *  Created on: Jun 21, 2018
 *      Author: haris
 */

#include <stdint.h>
#include "SRG_Input.h"
#include "SRG_USART.h"
#include "SRG_Timers.h"
#include "LED.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

OnePulseSettings TIMOnePulse;
Angles angles;

/***
 * External Interrupt and NVIC Initialization
 */
void Input_EXTI_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource4);
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

/***
 * Speed Meter Initialization
 * Timer 5 (32-bit timer, 84MHz, cca 11ns resolution)
 */
void SpeedCounter_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_InitStruct;

	TIM_InitStruct.TIM_Prescaler = 0;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = 0xffffffff;
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5, &TIM_InitStruct);
	TIM_Cmd(TIM5, ENABLE);
	TIM_SetCounter(TIM5, 0);
}

static void calculateTIMValues(void){
	TIMOnePulse.TIM_Pulse = (angles.onAngle * TIMOnePulse.T)/(60480); //This is OK!!
	TIMOnePulse.TIM_Period = (angles.offAngle * TIMOnePulse.T)/(60480);
	print("%d", TIMOnePulse.TIM_Period);
	print("\r\n%d\r\n", TIMOnePulse.TIM_Pulse);
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	//koji preskaler treba biti da bi od ivice do ivice stalo 65535 impulsa????
	uint16_t PrescalerValue = (uint16_t)(RCC_Clocks.PCLK1_Frequency * 2);
}

/***
 * External Interrupt handler for Speed Meter
 */
void EXTI4_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line4) != RESET){
		ToggleOrange();
		uint32_t counter = TIM_GetCounter(TIM5);
		TIM_SetCounter(TIM5, 0);
		uint32_t RPM = TIM5_FREQ * 15 / (counter * 2);
		print("!SSpeed in RPM: %d\r\n", RPM);
		TIMOnePulse.T = counter; // update period for pulses
		calculateTIMValues();
		updateTimers();
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}
