/*
 * SRG_Timers.h
 *
 *  Created on: Jun 14, 2018
 *      Author: Haris
 */

#ifndef SRG_TIMERS_H_
#define SRG_TIMERS_H_

#define TIM234_FREQ 84000000 //need to be checked

typedef struct {
	uint32_t TIM_Period;
	uint32_t TIM_Prescaler;
	uint32_t TIM_Pulse;
	uint32_t Counter;
}OnePulseSettings;

typedef struct {
	uint8_t onAngle;
	uint8_t offAngle;
}Angles;

extern OnePulseSettings TIMOnePulse;

extern Angles angles;

void Timer2_Init(void);
void Timer3_Init(void);
void Timer4_Init(void);
void updateTimers(void);

#endif /* SRG_TIMERS_H_ */
