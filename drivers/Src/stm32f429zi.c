#include "stm32f429zi.h"

void System_Init(void) {
    SysTick_Init();
    Timer_Init();
}
