#ifndef __TASKS_H__
#define __TASKS_H__

#include "main.h"

typedef enum {State_Focus, State_ShortRest, State_LongRest} Scheduler_State;
typedef enum {Available, NotAvailable} Scheduler_Availability;

typedef struct{
  Scheduler_State State;
  Scheduler_Availability Availability;
  uint32_t minutes_to_elapse;
  uint8_t cycles;
} Scheduler_TypeDef;



void Scheduler(void);
void task_Focus(void);
void task_ShortRest(void);
void task_LongRest(void);
void task_MinuteElapsed(uint8_t minutes_left);
void task_Idle(void);
void tasks_Init(void);
void Start_Scheduler(void);

#endif
