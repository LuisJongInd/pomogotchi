#include "it.h"

void EXTI15_10_IRQHandler(void) {
    // Handling interruptions in Pin 13 (Built-in Button is PC13)
    GPIO_IRQ_Handling(13);
}

extern SPI_DriverTypeDef spi1;
void SPI1_IRQHandler(void) { SPI_IRQ_Handling(&spi1); }
