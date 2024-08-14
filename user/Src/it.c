#include "it.h"

void EXTI15_10_IRQHandler(void) {
    // Handling interruptions in Pin 13 (Built-in Button is PC13)
    GPIO_IRQ_Handling(13);
}
