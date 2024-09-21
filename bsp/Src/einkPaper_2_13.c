#include "einkPaper_2_13.h"

/* Global variables */
EinkPaper_TypeDef epaper_2_13;
SPI_DriverTypeDef spi1;

// Display dimensions
uint8_t einkDisplay_Width = 122;
uint8_t einkDisplay_Height = 250;

/* Static functions */
static void eInkDisplay_GPIO_Init(void);
static void eInkDisplay_SPI_Init(void);
static void eInkDisplay_Sequence_Init(void);
static void eInkDisplay_HW_Reset(void);
static void eInkDisplay_SendData(uint8_t data);
static void eInkDisplay_SendCommand(uint8_t command);
static void eInkDisplay_UpdateDisplay(void);

/*
 * Initialazes the Display
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void eInkDisplay_Init(void) {

    // GPIO initialization (SPI low level configuration and GPIO pins)
    eInkDisplay_GPIO_Init();

    // Configure the SPI 1 peripheral as communication interface
    eInkDisplay_SPI_Init();

    // Follows the initialization sequence of the display
    eInkDisplay_Sequence_Init();
}

/*
 * Clears the display with white pixels by sending (0 is black and 1 is white)
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void eInkDisplay_FillWhite(void) {

    // Command: Write to RAM (0x24)
    eInkDisplay_SendCommand(0x24);

    // Loop trough the height and widt of the display. Since a data transaction
    // sends one byte, the width is divided by 8
    for (uint8_t h = 0; h < einkDisplay_Height; h++) {
        for (uint8_t w = 0; w < einkDisplay_Width / 8 + 1; w++) {
            eInkDisplay_SendData(0xFF);
        }
    }
    // Update the display after writing in RAM
    eInkDisplay_UpdateDisplay();
}

/*
 * Clears the display with black pixels by sending (0 is black and 1 is white)
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void eInkDisplay_FillBlack(void) {

    // Command: Write to RAM (0x24)
    eInkDisplay_SendCommand(0x24);

    // Loop trough the height and widt of the display. Since a data transaction
    // sends one byte, the width is divided by 8
    for (uint8_t h = 0; h < einkDisplay_Height; h++) {
        for (uint8_t w = 0; w < einkDisplay_Width / 8 + 1; w++) {
            eInkDisplay_SendData(0x00);
        }
    }

    // Update the display after writing in RAM
    eInkDisplay_UpdateDisplay();
}

/*
 * Displays the image in the e-ink paper display
 *
 * Params:
 *    * pImage, a pointer to a 8 bit-wide integer that corresponeds to the top
 * half of the display, this contains the strings
 *    * character_bitmap, a pointer to a 8 bit-wide integer which points to the
 * current image that will be displayed in the display, this is asigned to the
 * bottom half of the display
 * Returns:
 *    * None
 */
void eInkDisplay_DisplayImage(uint8_t *pImage, uint8_t *character_bitmap) {

    // This corresponds to the top half of the display
    // Command: Write to RAM (0x24)
    eInkDisplay_SendCommand(0x24);
    for (uint8_t h = 0; h < (einkDisplay_Height) / 2; h++) {
        for (uint8_t w = 0; w < einkDisplay_Width / 8 + 1; w++) {
            // Send the corresponding byte of the pImage array, this is a 1-D
            // array, to get the desired byte: w is the width byte, every time w
            // increases, the char will follow to the following byte. h is the
            // line, every time h increases, it adds an offset of the amount of
            // bytes that fit in the x direction
            eInkDisplay_SendData(pImage[w + h * (einkDisplay_Width / 8 + 1)]);
        }
    }

    // This corresponds to the bottom half of the display
    // Command: Write to RAM (0x24)
    for (uint8_t h = 0; h < (einkDisplay_Height) / 2; h++) {
        for (uint8_t w = 0; w < einkDisplay_Width / 8 + 1; w++) {
            // Send the corresponding byte of the pImage array, this is a 1-D
            // array, to get the desired byte: w is the width byte, every time w
            // increases, the char will follow to the following byte. h is the
            // line, every time h increases, it adds an offset of the amount of
            // bytes that fit in the x direction
            eInkDisplay_SendData(
                character_bitmap[w + h * (einkDisplay_Width / 8 + 1)]);
        }
    }

    // Update the display after writing in RAM
    eInkDisplay_UpdateDisplay();
}


/*
 * Initialazes the GPIO pins (SPI low level initializaion and GPIO pins of the e-ink paper display)
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void eInkDisplay_GPIO_Init(void) {

    /* Gpio pin initialization */

    // Configuring SPI peripheral

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

    GPIO_DriverTypeDef spi_CS = {0};

    spi_CS.pGPIOx = GPIOB;
    spi_CS.Config.Number = 0;
    spi_CS.Config.Mode = GPIO_Mode_Output;
    spi_CS.Config.OutputType = GPIO_OpType_PushPull;
    spi_CS.Config.Speed = GPIO_Speed_VeryHigh;
    spi_CS.Config.PullUpDown = GPIO_PuPd_None;

    GPIO_Init(&spi_CS);

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

    // Reset Pin is used to make  a HW reset
    GPIO_DriverTypeDef Reset_Pin;
    Reset_Pin.pGPIOx = GPIOB;
    Reset_Pin.Config.Number = 8;
    Reset_Pin.Config.Mode = GPIO_Mode_Output;
    Reset_Pin.Config.OutputType = GPIO_OpType_PushPull;
    Reset_Pin.Config.Speed = GPIO_Speed_VeryHigh;
    Reset_Pin.Config.PullUpDown = GPIO_PuPd_None;
    GPIO_Init(&Reset_Pin);

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
    epaper_2_13.CS_PinNumber = spi_CS.Config.Number;
    epaper_2_13.Busy_PinNumber = Busy_Pin.Config.Number;
    epaper_2_13.Reset_PinNumber = Reset_Pin.Config.Number;

    GPIO_Pin_Write(epaper_2_13.pGPIOx, epaper_2_13.CS_PinNumber, HIGH);
}


/*
 * Initialazes SPI1 as communication interface
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void eInkDisplay_SPI_Init(void) {
    /* SPI Pin Initialization */

    // SPI1 is used as the communication interface between with the display,
    // Selecting the 4-wire configuration available.

    spi1.pSPIx = SPI1;
    spi1.Config.Type = SPI_Type_FullDuplex;
    spi1.Config.Mode = SPI_Mode_0;
    spi1.Config.Hierarchy = SPI_Hierarchy_Master;
    spi1.Config.BaudRate = SPI_BaudRate_div2;
    spi1.Config.FrameFormat = SPI_FrameFormat_MSBFirst;
    spi1.Config.SSM = SPI_SSM_Enable;
    spi1.Config.DataFormat = SPI_DataFormat_8bit;

    SPI_Init(&spi1);
}

/*
 * Resets the e-ink display using Reset Pin
 * 
 * Params:
 *    * None
 * Returns:
 *    * None
 */
static void eInkDisplay_HW_Reset(void) {
    // To make a HW reset, Reset pin must go low
    GPIO_Pin_Write(epaper_2_13.pGPIOx, epaper_2_13.Reset_PinNumber, HIGH);
    delay(2);
    GPIO_Pin_Write(epaper_2_13.pGPIOx, epaper_2_13.Reset_PinNumber, LOW);
    delay(2);
    GPIO_Pin_Write(epaper_2_13.pGPIOx, epaper_2_13.Reset_PinNumber, HIGH);
    delay(2);
}

/*
 * Follows the initialization procedure of the e-ink paper display
 * 
 * Params:
 *    * None
 * Returns:
 *    * None
 */
static void eInkDisplay_Sequence_Init(void) {
    /* Display initialization */

    // Wait for 10 ms after energy supply
    delay(10);

    // HW reset
    eInkDisplay_HW_Reset();

    // Halt until e-ink display is not busy
    while (GPIO_Pin_Read(epaper_2_13.pGPIOx, epaper_2_13.Busy_PinNumber)) {
        ;
    }

    // Command: SW Reset (0x12)
    eInkDisplay_SendCommand(0x12);

    // Halt until e-ink display is not busy
    while (GPIO_Pin_Read(epaper_2_13.pGPIOx, epaper_2_13.Busy_PinNumber)) {
        ;
    }
    // Wait 10 ms
    // TODO: Delay function
    delay(10);
    // Command: gate driver output (0x01)
    eInkDisplay_SendCommand(0x01);
    // Data:
    //    [8:0]: MUX Gate lines - 1
    //    [2:0]: Gate Scanning sequence:
    //        [2]: Select first gate to scan:
    //          0: Gate 0 is the first to scan
    //          1: Gate 1 is the first to scan
    //        [1]: Scanning order:
    //          0: Sequential (0 .. gate outputs)
    //          1: Split in order (0, gate outputs/2, 1, gate outputs/2 +1, ...)
    //        [0]: Direction scanning:
    //          0: From button to top
    //          1: Frrom top to button (reversed)

    // Selecting 250 - 1 gate outputs
    eInkDisplay_SendData(einkDisplay_Height - 1); // first byte [7:0]
    eInkDisplay_SendData(0x00);                   // last bit [8]
    // Select 0 as the first gate to scan in sequential order,
    // from button to top
    eInkDisplay_SendData(0x00);

    // Set RAM size (0x11, 0x44, 0x45)

    // Command: Data entry mode (0x11)
    eInkDisplay_SendCommand(0x11);

    // Data:
    //    [2:0]: Define data entry sequence
    //          [1:0]: Increment/decrement setting:
    //              00: y decrement, x decrement
    //              01: y decrement, x increment
    //              10: y increment, x decrement
    //              11: y increment, x increment
    //          [2]: Set which direction to increment the address:
    //              0: increment in x direction
    //              1: increment in y direction
    // Select x and y incrementing direction, updating counter in x direction
    eInkDisplay_SendData(0x03);

    // Command: Set RAM x widht (0x44)
    eInkDisplay_SendCommand(0x44);

    // Data:
    //    [5:0]: x RAM start position
    //    [5:0]: x RAM end position
    // Select x start as 0 and end as source outputs - 1
    eInkDisplay_SendData(0x00);
    eInkDisplay_SendData((einkDisplay_Width - 1) /
                         8); // by every RAM unit (8 bits)

    // Command: Set RAM x widht (0x45)
    eInkDisplay_SendCommand(0x45);

    // Data:
    //    [8:0]: y RAM start position
    //    [8:0]: y RAM end position
    // Select y start as 0 and end as gate outputs - 1

    eInkDisplay_SendData(0x00);                     // [7:0]
    eInkDisplay_SendData(0x00);                     // [8]
    eInkDisplay_SendData((einkDisplay_Height - 1)); // [7:0]
    eInkDisplay_SendData(0x00);                     // [8]

    // Command: Set RAM x Counter (0x4E)
    eInkDisplay_SendCommand(0x4E);
    eInkDisplay_SendData(0x00);

    // Command: Set RAM y Counter (0x4F)
    eInkDisplay_SendCommand(0x4F);
    eInkDisplay_SendData(0x00);
    eInkDisplay_SendData(0x00);

    // Command: Set panel border (0x3C)
    eInkDisplay_SendCommand(0x3C);
    eInkDisplay_SendData(0x05);

    eInkDisplay_SendCommand(0x21);
    eInkDisplay_SendData(0x00);
    eInkDisplay_SendData(0x80);
    //
    // Command: Sense temperature by int/ext TS (0x18)
    eInkDisplay_SendCommand(0x18);
    eInkDisplay_SendData(0x80); // int temperature sensor

    // Wait busy
    while (GPIO_Pin_Read(epaper_2_13.pGPIOx, epaper_2_13.Busy_PinNumber)) {
        ;
    }
}

/*
 * Sends a byte in data mode
 *
 * Params:
 *    * data, a 8 bit-wide integer that will be sent in data mode 
 * Returns:
 *    * None
 */
static void eInkDisplay_SendData(uint8_t data) {
    // Send Data, D/C should be HIGH
    GPIO_Pin_Write(epaper_2_13.pGPIOx, epaper_2_13.DC_PinNumber, HIGH);
    GPIO_Pin_Write(epaper_2_13.pGPIOx, epaper_2_13.CS_PinNumber, LOW);
    SPI_SendData(&spi1, &data, 1);
    GPIO_Pin_Write(epaper_2_13.pGPIOx, epaper_2_13.CS_PinNumber, HIGH);
}

/*
 * Sends a byte in command mode
 *
 * Params:
 *    * data, a 8 bit-wide integer that will be sent in command mode 
 * Returns:
 *    * None
 */
static void eInkDisplay_SendCommand(uint8_t command) {
    // Send Command, D/C should be LOW
    GPIO_Pin_Write(epaper_2_13.pGPIOx, epaper_2_13.DC_PinNumber, LOW);
    GPIO_Pin_Write(epaper_2_13.pGPIOx, epaper_2_13.CS_PinNumber, LOW);
    SPI_SendData(&spi1, &command, 1);
    GPIO_Pin_Write(epaper_2_13.pGPIOx, epaper_2_13.CS_PinNumber, HIGH);
}

/*
 * Updates the image on the display after the RAM content is modified
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
static void eInkDisplay_UpdateDisplay(void) {

    // Command: Display update control (0x22)
    eInkDisplay_SendCommand(0x22);
    eInkDisplay_SendData(0xF7);

    eInkDisplay_SendCommand(0x20); // update display control
    
    // Wait until busy
    while (GPIO_Pin_Read(epaper_2_13.pGPIOx, epaper_2_13.Busy_PinNumber)) {
        ;
    }
}
