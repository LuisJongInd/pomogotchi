#ifndef __TIMERS_H__
#define __TIMERS_H__

#include "stm32f429zi.h"

extern volatile uint32_t global_ticks;
extern volatile uint32_t global_seconds;

void SysTick_Init(void);
void delay(uint32_t ms);
void Timer_Init(void);
void Timer_Start(void);
void Scheduler(void);
void Timer_Enable(void);
void Timer_Disable(void);

#endif 
