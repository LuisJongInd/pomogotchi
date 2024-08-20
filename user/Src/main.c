#include "main.h"
#include <string.h>

void GPIO_initialization(void);
void SPI_initialization(void);

SPI_DriverTypeDef spi1;
/*
 * This test will send data over SPI in blocking mode
 */

int main(void) {

    char msg[] = "Hello world";

    GPIO_initialization();
    SPI_initialization();
    SPI_SendData(&spi1, (uint8_t *)msg, strlen(msg));
    Test();

    while (1) {
        ;
    }

    return 0;
}

void GPIO_initialization(void) {

    GPIO_DriverTypeDef spi_NSS = {0};

    spi_NSS.pGPIOx = GPIOA;
    spi_NSS.InterruptMode = GPIO_It_Disable;
    spi_NSS.Config.Number = 4;
    spi_NSS.Config.Mode = GPIO_Mode_AlternateFunction;
    spi_NSS.Config.OutputType = GPIO_OpType_PushPull;
    spi_NSS.Config.Speed = GPIO_Speed_High;
    spi_NSS.Config.PullUpDown = GPIO_PuPd_None;
    spi_NSS.Config.AlternateFunction = 5;

    GPIO_Init(&spi_NSS);

    GPIO_DriverTypeDef spi_MOSI = {0};

    spi_MOSI.pGPIOx = GPIOA;
    spi_MOSI.InterruptMode = GPIO_It_Disable;
    spi_MOSI.Config.Number = 7;
    spi_MOSI.Config.Mode = GPIO_Mode_AlternateFunction;
    spi_MOSI.Config.OutputType = GPIO_OpType_PushPull;
    spi_MOSI.Config.Speed = GPIO_Speed_High;
    spi_MOSI.Config.PullUpDown = GPIO_PuPd_None;
    spi_MOSI.Config.AlternateFunction = 5;

    GPIO_Init(&spi_MOSI);

    GPIO_DriverTypeDef spi_SCK = {0};

    spi_SCK.pGPIOx = GPIOA;
    spi_SCK.InterruptMode = GPIO_It_Disable;
    spi_SCK.Config.Number = 5;
    spi_SCK.Config.Mode = GPIO_Mode_AlternateFunction;
    spi_SCK.Config.OutputType = GPIO_OpType_PushPull;
    spi_SCK.Config.Speed = GPIO_Speed_High;
    spi_SCK.Config.PullUpDown = GPIO_PuPd_None;
    spi_SCK.Config.AlternateFunction = 5;

    GPIO_Init(&spi_SCK);
}

void SPI_initialization(void) {

    spi1.pSPIx = SPI1;
    spi1.Config.Type = SPI_Type_FullDuplex;
    spi1.Config.Mode = SPI_Mode_0;
    spi1.Config.Hierarchy = SPI_Hierarchy_Master;
    spi1.Config.BaudRate = SPI_BaudRate_div16;
    spi1.Config.FrameFormat = SPI_FrameFormat_MSBFirst;
    spi1.Config.SSM = SPI_SSM_Disable;
    spi1.Config.DataFormat = SPI_DataFormat_8bit;

    SPI_Init(&spi1);
}

void Test(void) {
    for (uint8_t i = 0; i < 1; i++) {
        ;
    }
}
