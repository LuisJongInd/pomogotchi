#include "main.h"

void GPIO_initialization(void);

/*
 *  This example will turn on a LED whenever the button is pressed. The button
 * will be configured in Interruption mode, so toggling the LED will be done
 * inside the callback function
 */

int main(void) {

    GPIO_initialization();

    while (1) {
        ;
    }

    return 0;
}

void GPIO_initialization(void) {

    GPIO_ConfigTypeDef gpioLED = {0};

    gpioLED.pGPIOx = GPIOB;
    gpioLED.InterruptMode = GPIO_IT_DISABLE;
    gpioLED.PinConfig.Number = 7;
    gpioLED.PinConfig.Mode = GPIO_MODE_OUTPUT;
    gpioLED.PinConfig.OutputType = GPIO_OPTYPE_PUSHPULL;
    gpioLED.PinConfig.Speed = GPIO_SPEED_HIGH;
    gpioLED.PinConfig.PullUpDown = GPIO_PUPD_NONE;

    GPIO_Init(&gpioLED);

    GPIO_ConfigTypeDef gpioButton = {0};

    gpioButton.pGPIOx = GPIOC;
    gpioButton.InterruptMode = GPIO_IT_FALL;
    gpioButton.PinConfig.Number = 13;
    gpioButton.PinConfig.Mode = GPIO_MODE_INPUT;
    gpioButton.PinConfig.PullUpDown = GPIO_PUPD_NONE;

    GPIO_Init(&gpioButton);

    GPIO_IRQ_Control(EXTI15_10_IRQn, ENABLE);
    GPIO_IRQ_PriorityConfig(EXTI15_10_IRQn, 15);
    GPIO_Pin_Write(GPIOB, 7, LOW);
}

void GPIO_Callback_IRQTrigger(uint8_t PinNumber) {
    if (PinNumber == 13) {
        GPIO_Pin_Toggle(GPIOB, 7);
    }
}

void Test(void) {
    for (uint8_t i = 0; i < 1; i++) {
        ;
    }
}
