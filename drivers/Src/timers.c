#include "stm32f429zi.h"

volatile uint32_t global_ticks;

void SystemTickInit(void) {
    // Configuring for ticking every 1ms with the default clock frequency
    // (16Mhz), Loading the tick count
    // (16Mhz / 1kHz) in the load register of the SysTick Timer and enabling the
    // IRQ.

    SysTick->LOAD = 16000000 / 1000;
    SysTick->VAL = 0x00;
    NVIC_SetPriority(SysTick_IRQn, 15);
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; // Processor as Systick Source
    SysTick->CTRL |=
        SysTick_CTRL_TICKINT_Msk; // Interruption triggered when reaching 0
}

void SysTick_Handler(void) { global_ticks++; }

void delay(uint32_t ms) {
    global_ticks = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // Enabling SysTick
    uint32_t count = global_ticks + ms;
    while (global_ticks < count) {
        ;
    }
    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk); // Disabling SysTick
}
