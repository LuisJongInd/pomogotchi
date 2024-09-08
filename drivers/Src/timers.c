#include "stm32f429zi.h"

volatile uint32_t global_ticks;

void SystemTickInit(void) {
    // Configuring for ticking every 1ms with the default clock frequency
    // (16Mhz), using CMSIS defined function This should load the tick count
    // (16Mhz / 1kHz) in the load register of the SysTick Timer and enables the
    // IRQ.

    SysTick_Config(16000000 / 1000);
}

void SysTick_Handler(void) { global_ticks++; }

void delay(uint32_t ms) {
    uint32_t count = global_ticks + ms;
    while (global_ticks < count) {
        ;
    }
}
