/*
 * LED.c
 *
 *  Created on: Jun 15, 2018
 *      Author: haris
 */

#include <stdint.h>
#include "LED.h"
#include "stm32f4xx_gpio.h"

/***
 * Function for GPIO Initialization for LEDs
 */
void LED_Init(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}

/**************************************************
 * Primitive functions for debug LEDs
 **************************************************/
void ToggleGreen(void){
	GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
}

void ToggleOrange(void){
	GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
}

void ToggleRed(void){
	GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
}

void ToggleBlue(void){
	GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
}

void GreenON(void){
	GPIO_SetBits(GPIOD, GPIO_Pin_12);
}

void GreenOFF(void){
	GPIO_ResetBits(GPIOD, GPIO_Pin_12);
}

void OrangeON(void){
	GPIO_SetBits(GPIOD, GPIO_Pin_13);
}

void OrangeOFF(void){
	GPIO_ResetBits(GPIOD, GPIO_Pin_13);
}

void RedON(void){
	GPIO_SetBits(GPIOD, GPIO_Pin_14);
}

void RedOFF(void){
	GPIO_ResetBits(GPIOD, GPIO_Pin_14);
}

void BlueON(void){
	GPIO_SetBits(GPIOD, GPIO_Pin_15);
}

void BlueOFF(void){
	GPIO_ResetBits(GPIOD, GPIO_Pin_15);
}
/**************************************************
 * Primitive functions for debug LEDs
 **************************************************/

