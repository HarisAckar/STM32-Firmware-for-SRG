/*
 * SRG_USART.h
 *
 *  Created on: Jun 14, 2018
 *      Author: Haris
 */

#ifndef SRG_USART_H_
#define SRG_USART_H_

// countof macro
#define countof(a) (sizeof(a) / sizeof(*(a)))

// TX and RX buffer defines
#define TXBufferSize (countof(TXBuffer) - 1)
#define RXBufferSize (countof(RXBuffer) - 1)

// Number of data to read and write (can be modified)
#define NODTR 64
#define NODTW 64

// Parse Message errors
#define IAV 0x01 //Invalid angle value
#define WAAT 0x02 //Wrong argument for angle type
#define WMT 0x03 //Wrong message type (expected '!A' message type)
#define MTL 0x04 // Message too long

#define MAX_TIMEOUT 100000

typedef enum { FALSE = 0, TRUE = !FALSE} Bool;

typedef struct{
	uint8_t TXBuffer[256];
	uint8_t RXBuffer[256];
	uint8_t TXCounter;
	uint8_t RXCounter;
}Buffers;

extern Buffers USART3_Buffers;

void USART3_Init(void);
void USART3_IRQHandler(void);
void print(char * format, ...);

#endif /* SRG_USART_H_ */
