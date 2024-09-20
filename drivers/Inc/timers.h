#ifndef __TIMERS_H__
#define __TIMERS_H__

#include "stm32f429zi.h"

/* Extern variables */

// Global variables to keep track of the time elapsed

// Global ticks, triggered by the SysTick Interruption and used for delay function
extern volatile uint32_t global_ticks;
// Global seconds, triggered by the Timer 6 and used for the scheduler
extern volatile uint32_t global_seconds;

/* Exported functions */

// Initialization fucntions
void SysTick_Init(void);
void Timer_Init(void);

// delay function (in milliseconds)
void delay(uint32_t ms);

// Scheduler function, triggered by the timer
void Scheduler(void);

// Timer control
void Timer_Enable(void);
void Timer_Disable(void);
void Timer_Start(void);

#endif 
