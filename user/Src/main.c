#include "main.h"
#include <string.h>

void GPIO_initialization(void);

char msg[] = "Hello world";
/*
 * This test will send data over SPI in blocking mode
 */

int main(void) {
    SystemTickInit();
    eInkDisplay_Init();
    eInkDisplay_FillBlack();
    eInkDisplay_FillWhite();
    Test();

    while (1) {
        ;
    }

    return 0;
}

void Test(void) {
    for (uint8_t i = 0; i < 1; i++) {
        ;
    }
}
