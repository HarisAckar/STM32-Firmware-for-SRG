/**
  ******************************************************************************
  * @file    main.c
  * @author  Haris Ackar
  * @version V1.0
  * @date    14.06.2018
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "SRG_USART.h"
#include "SRG_Timers.h"
#include "SRG_Input.h"
#include "LED.h"
#include "delay.h"

OnePulseSettings TIMOnePulse;


/***
 * High Speed External Oscilator (HSE) = 8MHz
 * Main PLL -> SYSCLK = 168MHz
 * AHB = 168MHz
 * APB1 peripheral clock = 42MHz;
 * APB1 Timer clock = 84MHz;
 * APB2 peripheral clock = 84MHz;
 * APB2 Timer clock = 168MHz;
 * PLL Settings (M = /8; N = x226; P = /2; Q = /7)
 */
void setFastClockMode(void){
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	if(RCC_WaitForHSEStartUp() == ERROR){
		return;
	}

	RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, 2, 7);
	RCC_PLLCmd(ENABLE);
	while((RCC->CR & RCC_CR_PLLRDY) == 0);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div4);
	RCC_PCLK2Config(RCC_HCLK_Div2);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	SystemCoreClockUpdate();
}
			

int main(void)
{
	// "!S" Service message for APP
	// Setting fast clock mode
	setFastClockMode();

	// USART3 Initialization
	USART3_Init();
	print("-----> Program is starting <-----\r\n");
	print("!SFast Clock Mode Initialized\r\n");
	print("!SUSART Initialized\r\n");

	// Debug LEDs initialization
	LED_Init();
	print("!SDebug LEDs Initialized\r\n");

	// Start Timer for User delay
	TIM12_Start();
	print("!SUser Delay Initialized\r\n");

	//TIMOnePulse.TIM_Period = 65535;
	//TIMOnePulse.TIM_Pulse = 16383;

	// Timer 2 Initialization
	Timer2_Init();
	print("!STimer 2 Initialized\r\n");

	// Timer 3 Initialization
	Timer3_Init();
	print("!STimer 3 Initialized\r\n");

	// Timer 4 Initialization
	Timer4_Init();
	print("!STimer4 Initialized\r\n");

	//EXTI Interrupt Initialization
	Input_EXTI_Init();
	print("!SInput EXTI Initialized\r\n");

	//Speed Meter Initialization
	SpeedCounter_Init();
	print("!SSpeed Meter Initialized\r\n");


	while(1){

	}
}
