#include "main.h"
#include <string.h>

void GPIO_initialization(void);

/*
 */

int main(void) {
    System_Init();
    eInkDisplay_Init();
    task_Focus();

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
