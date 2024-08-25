#include "einkPaper_2_13.h"

EinkPaper_TypeDef epaper_2_13;
SPI_DriverTypeDef spi1;

void eInkDisplay_GPIO_Init(void);
void eInkDisplay_SPI_Init(void);

void eInkDisplay_Init(void) {
    // HardWare Initialization
    eInkDisplay_GPIO_Init();
    eInkDisplay_SPI_Init();
}

void eInkDisplay_GPIO_Init(void) {
    /* Gpio pin initialization */

    // Data/Command Pin (GPIO Output Pin):
    //  This is used in order to select between sending a command or sending
    //  data.

    GPIO_DriverTypeDef DS_Pin = {0};
    DS_Pin.pGPIOx = GPIOB;
    DS_Pin.Config.Number = 2;
    DS_Pin.Config.Mode = GPIO_Mode_Output;
    DS_Pin.Config.OutputType = GPIO_OpType_PushPull;
    DS_Pin.Config.Speed = GPIO_Speed_VeryHigh;
    DS_Pin.Config.PullUpDown = GPIO_PuPd_None;
    GPIO_Init(&DS_Pin);

    // Busy pin is used to indicate whenever the display's CPU is busy, it
    // should not be interurpted during this
    GPIO_DriverTypeDef Busy_Pin;
    Busy_Pin.pGPIOx = GPIOB;
    Busy_Pin.Config.Number = 5;
    Busy_Pin.Config.Mode = GPIO_Mode_Input;
    Busy_Pin.Config.PullUpDown = GPIO_PuPd_None;
    GPIO_Init(&Busy_Pin);

    // Initialaze EinkPaper_TypeDef struct with information related with
    // initialization

    epaper_2_13.pGPIOx = GPIOB;
    epaper_2_13.DC_PinNumber = DS_Pin.Config.Number;
    epaper_2_13.Busy_PinNumber = Busy_Pin.Config.Number;


    GPIO_DriverTypeDef spi_MOSI = {0};

    spi_MOSI.pGPIOx = GPIOA;
    spi_MOSI.Config.Number = 7;
    spi_MOSI.Config.Mode = GPIO_Mode_AlternateFunction;
    spi_MOSI.Config.OutputType = GPIO_OpType_PushPull;
    spi_MOSI.Config.Speed = GPIO_Speed_High;
    spi_MOSI.Config.PullUpDown = GPIO_PuPd_None;
    spi_MOSI.Config.AlternateFunction = 5;

    GPIO_Init(&spi_MOSI);

    GPIO_DriverTypeDef spi_SCK = {0};

    spi_SCK.pGPIOx = GPIOA;
    spi_SCK.Config.Number = 5;
    spi_SCK.Config.Mode = GPIO_Mode_AlternateFunction;
    spi_SCK.Config.OutputType = GPIO_OpType_PushPull;
    spi_SCK.Config.Speed = GPIO_Speed_High;
    spi_SCK.Config.PullUpDown = GPIO_PuPd_None;
    spi_SCK.Config.AlternateFunction = 5;

    GPIO_Init(&spi_SCK);

}

// TODO: Consider making an file that contain all handler in the bsp level
void EXTI9_5_IRQHandler(void) { GPIO_IRQ_Handling(epaper_2_13.Busy_PinNumber); }

void eInkDisplay_SPI_Init(void) {
    /* SPI Pin Initialization */

    // SPI1 is used as the communication interface between with the display,
    // Selecting the 4-wire configuration available.

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
