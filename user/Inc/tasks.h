#ifndef __TASKS_H__
#define __TASKS_H__

#include "main.h"

/* Extern variables defined in other files */

// Array representation of the images for every state
extern uint8_t focus_monkey[];
extern uint8_t beer_monkey[];
extern uint8_t sleeping_monkey[];
extern uint8_t empty_tamagotchi[];
// Pointer to the array image that will be displayed
extern uint8_t *current_tamagotchi;

/*
 * Defines the three available states in the Pomodoro technique.
 */
typedef enum {
  State_Focus,      // Focus State      (25 minutes)
  State_ShortRest,  // Short Rest State (5 minutes)
  State_LongRest,   // Long Rest State  (15 minutes)
} Scheduler_State;

/*
 * Indicates if the CPU is able to go to sleep.
 */
typedef enum {
  Available,       // Scheduler available, CPU able to sleep
  NotAvailable     // Scheduler not available, CPU not able to sleep

} Scheduler_Availability;

/*
 * Contains the current State and the Availability of the scheduler. 
 */
typedef struct{
  Scheduler_State State;              // Sets the current state of the pomodoro technique, values can be of Scheduler_State
  Scheduler_Availability Availability;// Defines the current state of the pomodoro technique, values can be of Scheduler_Availability.
  uint32_t minutes_to_elapse;         // Integer variable that holds the amount of time of the current state.
  uint8_t cycles;                     // Gets track of the current amount of cycles, used to decide betweeen states when scheduled.
} Scheduler_TypeDef;


/* Exported functions */

// Task functions
void task_Focus(void);
void task_ShortRest(void);
void task_LongRest(void);
void task_MinuteElapsed(uint16_t minutes_left);
void task_Idle(void);
void tasks_Init(void);

// Scheduler functions
void Start_Scheduler(void);
void Scheduler(void);


#endif
