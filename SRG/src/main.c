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
#include "SRG_ADC.h"
#include "LED.h"
#include "delay.h"

OnePulseSettings TIMOnePulse;
Angles angles;

#define FCMI 0x10
#define USART_INIT 0x20
#define LED_INIT 0x30
#define UDI 0x40
#define TIM2_INIT 0x50
#define TIM3_INIT 0x60
#define TIM4_INIT 0x70
#define EXTI_INIT 0x80
#define SM_INIT 0x90


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
	/**
	 * Initial values for period, pulse and on/off angles
	 */
	TIMOnePulse.TIM_Period = 65535;
	TIMOnePulse.TIM_Pulse = 16383;

	angles.onAngle = 10;
	angles.offAngle = 22.5;
	/**
	 * Initial values for period, pulse and on/off angles
	 */

	// "!S" Service message for APP
	// Setting fast clock mode
	setFastClockMode();

	// USART3 Initialization
	USART3_Init();
	print("!S%d\r\n", FCMI);
	print("!S%d\r\n", USART_INIT);

	// Debug LEDs initialization
	LED_Init();
	print("!S%d\r\n", LED_INIT);

	// Start Timer for User delay
	TIM12_Start();
	print("!S%d\r\n", UDI);

	// Timer 2 Initialization
	Timer2_Init();
	print("!S%d\r\n", TIM2_INIT);

	// Timer 3 Initialization
	Timer3_Init();
	print("!S%d\r\n", TIM3_INIT);

	// Timer 4 Initialization
	Timer4_Init();
	print("!S%d\r\n", TIM4_INIT);

	//EXTI Interrupt Initialization
	Input_EXTI_Init();
	print("!S%d\r\n", EXTI_INIT);

	//Speed Meter Initialization
	SpeedCounter_Init();
	print("!S%d\r\n", SM_INIT);

	while(1){

	}
}
