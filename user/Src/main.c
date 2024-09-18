/**
 * @file main.c
 * @author LuisJongInd
 * @brief main file, sets previous configurations and runs the scheduler
 */

#include "main.h"
#include <string.h>

void GPIO_initialization(void);

int main(void) {
    // Timer and SysTick related configurations (driver level)
    System_Init();
    // Display initialization (bsp level)
    eInkDisplay_Init();
    // Run the scheduler
    Start_Scheduler();

    while (1) {
        ;
    }

    return 0;
}
