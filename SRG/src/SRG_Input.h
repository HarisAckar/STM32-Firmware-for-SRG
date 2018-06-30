/*
 * SRG_Input.h
 *
 *  Created on: Jun 21, 2018
 *      Author: haris
 */

#ifndef SRG_INPUT_H_
#define SRG_INPUT_H_

#define TIM5_FREQ 84000000 //need to be checked

void Input_EXTI_Init(void);
void SpeedCounter_Init(void);
void EXTI4_IRQHandler(void);

#endif /* SRG_INPUT_H_ */
