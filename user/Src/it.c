#include "it.h"

/*
 * Vector table entry that handles EXTI lines (10..15), used fot built-in button
 */

void EXTI15_10_IRQHandler(void) {
    // Software debouncer, avoid multiple IRQ triggering
    for (volatile uint32_t i = 0; i < 500000; i++) {
        ;
    }
    // GPIO API handler
    GPIO_IRQ_Handling(13);
}
