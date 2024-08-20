#include "spi.h"

static void SPI_PeripheralClockControl(SPI_TypeDef *pSPIx,
                                       EnableDisable EnorDi);

DriverStatus SPI_Init(SPI_DriverTypeDef *pSPIDriver) {
    // Enable clocks in ordr to configure the SPI
    SPI_PeripheralClockControl(pSPIDriver->pSPIx, ENABLE);

    /* Configure SPI Type  */

    // BIDIMODE[0] sets the wire configuration of SPI:
    //    0: 2-line unidirectional data mode selected
    //    1: 1-line bidirectional data mode selected

    if (pSPIDriver->Config.Type == SPI_Type_FullDuplex) {
        // BIDIMODE[0] = 0
        pSPIDriver->pSPIx->CR1 &= ~(SPI_CR1_BIDIMODE);

    } else if (pSPIDriver->Config.Type == SPI_Type_HalfDuplex) {
        // BIDIMODE[0] = 1
        pSPIDriver->pSPIx->CR1 |= (SPI_CR1_BIDIMODE);
    } else if (pSPIDriver->Config.Type == SPI_Type_RxOnly) {
        // BIDIMODE[0] = 0
        pSPIDriver->pSPIx->CR1 &= ~(SPI_CR1_BIDIMODE);
        // RXONLY[0] is used with BIDIMODE to select the direction of transfer
        // in 2-line mode.
        //  0: Full Duplex (transmit and receive)
        //  1: Output disabled (Receive-only mode)
        pSPIDriver->pSPIx->CR1 |= ~(SPI_CR1_RXONLY);
    }

    /* Configure SPI mode */

    // CPOL[0] Clock polarity configuration
    //    0: Clock to 0 when idle
    //    1: Clock to 1 when idle
    // CPHA[0] Clock phase configuration
    //    0: Capture on first edge
    //    1: Capture on second edge
    if (pSPIDriver->Config.Mode == SPI_Mode_0) {
        // Mode 0: CPOL = 0, CPHA = 0
        pSPIDriver->pSPIx->CR1 &= ~(SPI_CR1_CPOL);
        pSPIDriver->pSPIx->CR1 &= ~(SPI_CR1_CPHA);

    } else if (pSPIDriver->Config.Mode == SPI_Mode_1) {
        // Mode 0: CPOL = 0, CPHA = 1
        pSPIDriver->pSPIx->CR1 &= ~(SPI_CR1_CPOL);
        pSPIDriver->pSPIx->CR1 |= (SPI_CR1_CPHA);

    } else if (pSPIDriver->Config.Mode == SPI_Mode_2) {
        // Mode 0: CPOL = 1, CPHA = 0
        pSPIDriver->pSPIx->CR1 |= (SPI_CR1_CPOL);
        pSPIDriver->pSPIx->CR1 &= ~(SPI_CR1_CPHA);

    } else if (pSPIDriver->Config.Mode == SPI_Mode_3) {
        // Mode 0: CPOL = 1, CPHA = 1
        pSPIDriver->pSPIx->CR1 |= (SPI_CR1_CPOL);
        pSPIDriver->pSPIx->CR1 |= (SPI_CR1_CPHA);
    }

    /* Master/Slave selection */

    // MSTR[0] Selects between Master or Slave
    //    0: Slave Configuration
    //    1: Master Confiuration

    pSPIDriver->pSPIx->CR1 |=
        (pSPIDriver->Config.Hierarchy << SPI_CR1_MSTR_Pos);

    /* Select Baud Rate divider */

    // BR[2:0] selects the BaudRate divider
    pSPIDriver->pSPIx->CR1 |= (pSPIDriver->Config.BaudRate << SPI_CR1_BR_Pos);

    /* Frame Format selection */

    // LSBFIRST[0]
    //    0: MSB transmitted first
    //    1: LSB tranmistted first
    pSPIDriver->pSPIx->CR1 |=
        (pSPIDriver->Config.FrameFormat << SPI_CR1_LSBFIRST_Pos);

    /* SSM management management */

    pSPIDriver->pSPIx->CR1 |=
        (pSPIDriver->Config.FrameFormat << SPI_CR1_LSBFIRST_Pos);

    /* Data Frame Format Selection */

    // DFF[0]
    //  0: 8-bit data frame format
    //  1: 16-bit data frame format
    pSPIDriver->pSPIx->CR1 |=
        (pSPIDriver->Config.DataFormat << SPI_CR1_DFF_Pos);

    return OK;
}

static void SPI_PeripheralClockControl(SPI_TypeDef *pSPIx,
                                       EnableDisable EnorDi) {

    // SPI peripherals are on different buses:
    //  * APB2:
    //      - SPI1
    //      - SPI4
    //      - SPI5
    //      - SPI6
    //  * ABP1:
    //      - SPI2
    //      - SPI3

    if (EnorDi == ENABLE) {
        switch ((unsigned long int)pSPIx) {
        case SPI1_BASE:
            RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
            break;
        case SPI2_BASE:
            RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
            break;
        case SPI3_BASE:
            RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
            break;
        case SPI4_BASE:
            RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
            break;
        case SPI5_BASE:
            RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
            break;
        case SPI6_BASE:
            RCC->APB2ENR |= RCC_APB2ENR_SPI6EN;
            break;
        }
    }
}
