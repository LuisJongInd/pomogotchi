#include "stm32f429zi.h"

/*
 * Initialazes the Timer 6 and SysTick interruption
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void System_Init(void) {
    SysTick_Init();
    Timer_Init();
}
