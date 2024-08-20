#include "main.h"

void GPIO_initialization(void);
void SPI_initialization(void);

/*
 * This test will only initialaze SPI1 and SPI3 porst.
 */

int main(void) {

    GPIO_initialization();
    SPI_initialization();
    Test();

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

void SPI_initialization(void) {

    SPI_DriverTypeDef spi1 = {0};

    spi1.pSPIx = SPI1;
    spi1.Config.Type = SPI_Type_FullDuplex;
    spi1.Config.Mode = SPI_Mode_1;
    spi1.Config.Hierarchy = SPI_Hierarchy_Master;
    spi1.Config.BaudRate = SPI_BaudRate_div8;
    spi1.Config.FrameFormat = SPI_FrameFormat_LSBFirst;
    spi1.Config.SSM = SPI_SSM_Disable;
    spi1.Config.DataFormat = SPI_DataFormat_16bit;

    SPI_Init(&spi1);

    SPI_DriverTypeDef spi3 = {0};
    spi3.pSPIx = SPI3;
    spi3.Config.Type = SPI_Type_HalfDuplex;
    spi3.Config.Mode = SPI_Mode_3;
    spi3.Config.Hierarchy = SPI_Hierarchy_Slave;
    spi3.Config.BaudRate = SPI_BaudRate_div8;
    spi3.Config.FrameFormat = SPI_FrameFormat_MSBFirst;
    spi3.Config.SSM = SPI_SSM_Enable;
    spi3.Config.DataFormat = SPI_DataFormat_16bit;

    SPI_Init(&spi3);
}

void Test(void) {
    for (uint8_t i = 0; i < 1; i++) {
        ;
    }
}
