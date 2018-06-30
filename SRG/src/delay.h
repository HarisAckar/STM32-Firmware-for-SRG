/*
 * delay.h
 *
 *  Created on: Jun 16, 2018
 *      Author: haris
 */

#ifndef DELAY_H_
#define DELAY_H_

void TIM12_Start(void);
void delay_ms(uint32_t time_ms);
void delay_us(uint32_t time_us);
void TIM8_BRK_TIM12_IRQHandler(void);

#endif /* DELAY_H_ */
