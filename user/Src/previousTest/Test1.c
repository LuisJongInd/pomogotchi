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

    GPIO_DriverTypeDef gpioLED = {0};

    gpioLED.pGPIOx = GPIOB;
    gpioLED.InterruptMode = GPIO_It_Disable;
    gpioLED.Config.Number = 7;
    gpioLED.Config.Mode = GPIO_Mode_Output;
    gpioLED.Config.OutputType = GPIO_OpType_PushPull;
    gpioLED.Config.Speed = GPIO_Speed_High;
    gpioLED.Config.PullUpDown = GPIO_PuPd_None;

    GPIO_Init(&gpioLED);

    GPIO_DriverTypeDef gpioButton = {0};

    gpioButton.pGPIOx = GPIOC;
    gpioButton.InterruptMode = GPIO_It_Fall;
    gpioButton.Config.Number = 13;
    gpioButton.Config.Mode = GPIO_Mode_Input;
    gpioButton.Config.PullUpDown = GPIO_PuPd_None;

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
