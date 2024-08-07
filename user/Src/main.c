#include "main.h"

GPIO_ConfigTypeDef gpioTest;

void GPIO_initialization(void);

int main(void) {

    GPIO_initialization();

    Test();

    while (1) {
        ;
    }
    return 0;
}

void GPIO_initialization(void) {

    gpioTest.pGPIOx = GPIOC;
    gpioTest.PinConfig.Number = 10;
    gpioTest.PinConfig.Mode = GPIO_MODE_OUTPUT;
    gpioTest.PinConfig.OutputType = GPIO_OPTYPE_PUSHPULL;
    gpioTest.PinConfig.Speed = GPIO_SPEED_HIGH;
    gpioTest.PinConfig.PullUpDown = GPIO_PUPD_NONE;

    GPIO_Init(&gpioTest);
}

void Test(void) {
    for (uint8_t i = 0; i < 1; i++) {
        ;
    }
}
