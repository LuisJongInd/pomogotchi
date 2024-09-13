#ifndef __TIMERS_H__
#define __TIMERS_H__

#include "stm32f429zi.h"

void SysTick_Init(void);
void delay(uint32_t ms);
void Timer_Init(void);
void Timer_Start(void);

#endif 
