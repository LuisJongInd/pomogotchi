#include "main.h"

void GPIO_initialization(void);
void buttonPressed(void);

/* This example initialazes a turns on a built-in pin in order
 * to test GPIO_Init function and GPIO_Pin_Write function. This
 * will write to PB7, which is the built-in blue LED in the
 * STM32F429zi board.
 */

int main(void) {

    GPIO_initialization();

    Test();
    GPIO_Pin_Write(GPIOB, 7, HIGH);

    while (1) {

        while (!GPIO_Pin_Read(GPIOC, 13)) {
            ;
        }
        buttonPressed();
    }
    return 0;
}

void GPIO_initialization(void) {

    GPIO_ConfigTypeDef gpioLED = {0};

    gpioLED.pGPIOx = GPIOB;
    gpioLED.PinConfig.Number = 7;
    gpioLED.PinConfig.Mode = GPIO_MODE_OUTPUT;
    gpioLED.PinConfig.OutputType = GPIO_OPTYPE_PUSHPULL;
    gpioLED.PinConfig.Speed = GPIO_SPEED_HIGH;
    gpioLED.PinConfig.PullUpDown = GPIO_PUPD_NONE;

    GPIO_Init(&gpioLED);

    GPIO_ConfigTypeDef gpioButton = {0};

    gpioButton.pGPIOx = GPIOC;
    gpioButton.PinConfig.Number = 13;
    gpioButton.PinConfig.Mode = GPIO_MODE_INPUT;

    GPIO_Init(&gpioButton);
}

void Test(void) {
    for (uint8_t i = 0; i < 1; i++) {
        ;
    }
}

void buttonPressed(void) {
    while (1)
        ;
}
