/*
 * SRG_USART.c
 *
 *  Created on: Jun 14, 2018
 *      Author: Haris
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <misc.h>
#include "SRG_USART.h"
#include "SRG_Timers.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "LED.h"

Buffers USART3_Buffers;
Angles angles;

static void USART3_SendString(void);

/**
 * USART 3
 * BaudRate = 115200;
 * NoHW Flow Control
 * Stop bits 1
 * USB-Serial Cable-F (Olimex): GND = BLUE; RX = GREEN; TX = RED;
 */
void USART3_Init(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //Enable GPIOD peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //Enable USART3 peripheral clock

	/* GPIO for USART3 */
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; // PD8 - USART3_TX, PD9 - USART3_RX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // this pins are configured as alternate function for USART3
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // IO speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // Output type as Push-Pull mode
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // Activates pullup resistor on the IO pins
	GPIO_Init(GPIOD, &GPIO_InitStruct); // Setting up GPIO registers
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3); // PD8 is now connected to alternating function for USART3_TX
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3); // PD9 is now connected to alternating function for USART3_RX

	/* Enable USART3 Interrupt */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	/* USART3 Initialization */
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200; // Baudrate
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; //8 bits frame size
	USART_InitStruct.USART_StopBits = USART_StopBits_1; // 1 stop bit
	USART_InitStruct.USART_Parity = USART_Parity_No; // No parity
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // No flow control
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // Enable Transmit and Receive

	// Configure USART3
	USART_Init(USART3, &USART_InitStruct); // Setting up USART3 registers

	// Enable Receive interrupt
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	//DMA Settings
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) (&(USART3->DR));
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStruct.DMA_Channel = DMA_Channel_4;
    DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStruct.DMA_Memory0BaseAddr =(uint32_t)(&USART3_Buffers.TXBuffer) ;

    DMA_Init(DMA1_Stream3,&DMA_InitStruct);
    USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);

	// Enable the USART3
	USART_Cmd(USART3, ENABLE);

	// Initialize values before start
	USART3_Buffers.RXCounter = 0;
	USART3_Buffers.TXCounter = 0;
}

/**
 * Helper function
 * Used in ParseMessage
 */
static uint8_t getAngle(void){
	uint8_t i = 0;
	char str[10];
	for(i = 3; i < USART3_Buffers.RXCounter; i++){
		str[i-3] = USART3_Buffers.RXBuffer[i];
	}
	return atoi(str);
}

/***
 * Function for Parsing incoming message
 */
static void ParseMessage(void){
	if(USART3_Buffers.RXCounter < 10){
		if(USART3_Buffers.RXBuffer[0] == '!' && USART3_Buffers.RXBuffer[1] == 'A'){
			if(USART3_Buffers.RXBuffer[2] == 'N'){//ON Angle
				uint8_t tmp = getAngle();
				if(tmp > 45 && tmp < 0){
					print("!S%d\r\n", IAV);
				}
				else{
					angles.onAngle = tmp;
				}
			}
			else if(USART3_Buffers.RXBuffer[2] == 'F'){//OFF Angle
				uint8_t tmp = getAngle();
				if(tmp > 45 && tmp < 0){
					print("!S%d\r\n", IAV);
				}
				else{
					angles.offAngle = tmp;
				}
			}
			else {
				print("!S%d\r\n", WAAT);
			}
		}
		else{
			print("!S%d\r\n", WMT);
		}
	}
	else {
		print("!S%d\r\n", MTL);
	}
	USART3_Buffers.RXCounter = 0;
}

/***
 * Interrupt handler for USART3
 */
void USART3_IRQHandler(void){
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		USART3_Buffers.RXBuffer[USART3_Buffers.RXCounter++] = USART_ReceiveData(USART3);
		if(USART3_Buffers.RXBuffer[USART3_Buffers.RXCounter - 1] == '\r'){ //Received end of Message
			ParseMessage();
		}
		if(USART3_Buffers.RXCounter == NODTR){ //if buffer is full stop interrupts
			USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
		}
	}
}

/***
 * Function for transmitting data over USART3
 */
void print(char * format, ...){
	va_list args;
	va_start (args, format);
	vsnprintf (USART3_Buffers.TXBuffer, 255, format, args);
	va_end (args);
	USART3_SendString();
}

static void USART3_SendString(void){
	uint32_t i = 0;
	uint32_t timeOut = MAX_TIMEOUT;

	while (USART3_Buffers.TXBuffer[i] != 0){
		i++;
	}

	DMA1_Stream3->NDTR = i;
	DMA_Cmd(DMA1_Stream3, ENABLE);
	while(DMA_GetFlagStatus(DMA1_Stream3, DMA_IT_TCIF3) != SET && --timeOut > 0);
	DMA_ClearFlag(DMA1_Stream3, DMA_IT_TCIF3);
}
