#include "spi.h"

/* Static functions */
static void SPI_PeripheralClockControl(SPI_TypeDef *pSPIx,
                                       EnableDisable EnorDi);
static FlagStatus SPI_GetFlag(SPI_TypeDef *pSPIx, uint32_t flag);
static void SPI_IRQHandleTXe(SPI_DriverTypeDef *pSPIDriver);
static void SPI_CloseTransmission(SPI_DriverTypeDef *pSPIDriver);

/*
 * SPI initilaization function. Used to configure the SPI port
 *
 * Params:
 *    * pSPIDriver, pointer to a SPI_DriverTypeDef structure with the SPI
 * configurations
 * Returns:
 *    * DriverStatus, a flag that returns the succesfulness of the SPI
 * initilaization
 */
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
        // RXONLY[0] is used with BIDIMODE to select the direction of
        // transfer in 2-line mode.
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

    if (pSPIDriver->Config.SSM == SPI_SSM_Disable) {
        // Disable Software management
        pSPIDriver->pSPIx->CR1 &= ~(SPI_CR1_SSM);
        // Enable Slave Select
        pSPIDriver->pSPIx->CR2 |= (SPI_CR2_SSOE);

    } else if (pSPIDriver->Config.SSM == SPI_SSM_Enable) {
        // Enable Software management
        pSPIDriver->pSPIx->CR1 |= (SPI_CR1_SSM);
        // enable SSU to avoid MODF error
        pSPIDriver->pSPIx->CR1 |= (SPI_CR1_SSI);
        // Disable Slave Select
        pSPIDriver->pSPIx->CR2 &= ~(SPI_CR2_SSOE);
    }
    /* Data Frame Format Selection */

    // DFF[0]
    //  0: 8-bit data frame format
    //  1: 16-bit data frame format
    pSPIDriver->pSPIx->CR1 |=
        (pSPIDriver->Config.DataFormat << SPI_CR1_DFF_Pos);

    return OK;
}

/*
 * Sending data via SPIx in blocking mode
 *
 * Params:
 *    * pSPIDriver, a pointer to SPI_DriverTypeDef that contains the SPIx that
 * will send data and its configuration
 *    * pTxBuffer, a pointer to a 8 bit-wide integer that holds the data to send
 *    * Len, a 32 bit-wide integer with the Len of the data to send
 * Returns:
 *    * None
 */
void SPI_SendData(SPI_DriverTypeDef *pSPIDriver, uint8_t *pTxBuffer,
                  uint32_t Len) {
    /* Send Data over SPIx, blocking mode */

    // Enable SPI peripheral
    pSPIDriver->pSPIx->CR1 |= (SPI_CR1_SPE);

    // Block until Len = 0

    while (Len) {
        // Wait until TXe flags is HIGH
        while (SPI_GetFlag(pSPIDriver->pSPIx, SPI_SR_TXE) != FLAG_HIGH) {
            ;
        }

        if (pSPIDriver->Config.DataFormat == SPI_DataFormat_16bit) {

            // Save 2 bytes into Data Register
            pSPIDriver->pSPIx->DR = *((uint16_t *)pTxBuffer);
            // Decrease Lenght by 2
            Len--;
            Len--;
            (uint16_t *)pTxBuffer++;

        } else {

            // Save 1 byte into Data Register
            pSPIDriver->pSPIx->DR = *(pTxBuffer);
            // Decrease Lenght by 1
            Len--;
            pTxBuffer++;
        }
    }

    // Wait until Busy Flag is LOW
    while (SPI_GetFlag(pSPIDriver->pSPIx, SPI_SR_BSY) != FLAG_LOW)
        ;

    // Disable SPI
    pSPIDriver->pSPIx->CR1 &= ~(SPI_CR1_SPE);
}

/*
 * Sending data via SPIx in interruption mode (non blocking mode)
 *
 * Params:
 *    * pSPIDriver, a pointer to SPI_DriverTypeDef that contains the SPIx that
 * will send data and its configuration
 *    * pTxBuffer, a pointer to a 8 bit-wide integer that holds the data to send
 *    * Len, a 32 bit-wide integer with the Len of the data to send
 * Returns:
 *    * None
 */
DriverStatus SPI_SendDataIT(SPI_DriverTypeDef *pSPIDriver, uint8_t *pTxBuffer,
                            uint32_t Len) {
    /* Send Data over SPIx, IT mode */

    // Verify Tx is not busy already
    if (pSPIDriver->TxState == SPI_TxState_Busy) {
        return BUSY;
    }

    // Save Len and pointer variables inside the SPI Driver Struct
    pSPIDriver->TxState = SPI_TxState_Busy;
    pSPIDriver->TxLen = Len;
    pSPIDriver->pTxBuffer = pTxBuffer;

    // Configure SPI interruption
    // TXEIE[0] Enables the Tx Buffer interrupt enable
    //    0: TXE masked
    //    1: TXE not masked, used ot generated TXE interrupts

    pSPIDriver->pSPIx->CR2 |= SPI_CR2_TXEIE;

    // Enable SPI peripheral
    pSPIDriver->pSPIx->CR1 |= (SPI_CR1_SPE);

    return OK;
}

/*
 * Handles the SPI interruption, invokes a handler function that correspond to
 * the type of interruption
 *
 * Params:
 *    * pSPIDriver, a pointer to the SPI_DriverTypeDef which contains the SPI
 * information Returns:
 *    * None
 *
 */
void SPI_IRQ_Handling(SPI_DriverTypeDef *pSPIDriver) {
    /* Handling any SPI interruption arised */

    // Check if the interruption is due to TXE by checking
    // if Tx Empty is enabled in CR2 and if TX Empty flag is set in SR
    if ((pSPIDriver->pSPIx->CR2 >> SPI_CR2_TXEIE_Pos) &&
        (pSPIDriver->pSPIx->SR >> SPI_SR_TXE_Pos)) {
        // TX buffer ready to sent new data, invoke the callback
        SPI_IRQHandleTXe(pSPIDriver);
    }
}

/*
 * Enables/Disables the interruption on the NVIC table
 *
 * Params:
 *    * IRQNumber, a IRQn_Type variable that corresponds to the SPI interruption
 * number
 *    * EnorDi, a EnableDisable variable that defines if the interruption will
 * be enabled or disabled
 */
void SPI_IRQ_Control(IRQn_Type IRQNumber, EnableDisable EnOrDi) {
    /* Enabling/Disabling SPI interruptions*/

    // To enable interruptions we use NVIC_ISERx register. This
    // is a set of 8 registers that configure up to 91 interruptions

    // CMSIS provides functions for enabling/disabling interruptions
    if (EnOrDi == ENABLE) {
        // param of this function is a IRQn_Type which is an enum defined
        // in stm32f429xx header file
        __NVIC_EnableIRQ(IRQNumber);
    } else {
        __NVIC_DisableIRQ(IRQNumber);
    }
}

/*
 * Sets the interruption level of the SPI
 *
 * Params:
 *    * IRQNumber, an IRQn_Type variable with the corresponding interruption
 * number on the vector table
 *    * IRQPriority, a 32 bit-wide integer with the new level of priority
 */
void SPI_IRQ_PriorityConfig(IRQn_Type IRQNumber, uint32_t IRQPriority) {

    /* Configuring SPI interruptions priority */

    // To configure the priority, the NVIC_IPRx registers are used. Those
    // are 60 registers used to configure the priority values of each
    // interruptions available

    // Using CMSIS defined function
    __NVIC_SetPriority(IRQNumber, IRQPriority);
}

/*
 * Enables/disables the clock of the SPIx
 *
 * Params:
 *    * pSPIx, a pointer to a SPI_TypeDef with the SPIx to be enabled
 *    * EnorDi, a EnableDisable variable that defines whether the peripheral
 * will be enabled or disabled
 * Returns:
 *    * None
 */
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

/*
 * Returns the status of a desired flag of the SPIx
 *
 * Params:
 *    * pSPIx, a pointer to the SPI_TypeDef which corresponds to the SPIx
 *    * flag, a 32 bit-wide pointer that corresponds with the position of the
 * flag Returns:
 *    * FlagStatus, the status of the desired flag on the SPIx
 */
static FlagStatus SPI_GetFlag(SPI_TypeDef *pSPIx, uint32_t flag) {
    // The SPI_SR contains the arised flags of the SPIx peripheral
    if (pSPIx->SR & flag) {
        return FLAG_HIGH;
    }
    return FLAG_LOW;
}

/*
 * Sends a new byte every time the Tx buffer is ready to send a new byte
 *
 * Params:
 *    * pSPIDriver, a pointer to a SPI_DriverTypeDef which contains the SPIx
 * that will send data
 * Returns:
 *    * None
 */
static void SPI_IRQHandleTXe(SPI_DriverTypeDef *pSPIDriver) {
    /* Send data withouth blocking */

    if (pSPIDriver->Config.DataFormat == SPI_DataFormat_16bit) {

        // Save 2 bytes into Data Register
        pSPIDriver->pSPIx->DR = *((uint16_t *)pSPIDriver->pTxBuffer);
        // Decrease Lenght by 2
        pSPIDriver->TxLen--;
        pSPIDriver->TxLen--;
        (uint16_t *)pSPIDriver->pTxBuffer++;

    } else {

        // Save 1 byte into Data Register
        pSPIDriver->pSPIx->DR = *(pSPIDriver->pTxBuffer);
        // Decrease Lenght by 1
        pSPIDriver->TxLen--;
        pSPIDriver->pTxBuffer++;
    }

    if (pSPIDriver->TxLen == 0) {
        SPI_CloseTransmission(pSPIDriver);
        SPI_CallbackTxCompleted(pSPIDriver);
    }
}

/*
 * Ends the transmission on the SPIx driver when there is no more data to send
 *
 * Params:
 *    * pSPIDriver, a pointer to a SPI_DriverTypeDef which contains the SPIx
 * that will close the transmission
 * Returns:
 *    * None
 */
static void SPI_CloseTransmission(SPI_DriverTypeDef *pSPIDriver) {
    // Disabling the interruption

    pSPIDriver->pSPIx->CR2 &= ~(SPI_CR2_TXEIE);

    // Re-initialaze holded variables
    pSPIDriver->pTxBuffer = NULL;
    pSPIDriver->TxLen = 0;

    // Setting state as ready to be available again
    pSPIDriver->TxState = SPI_TxState_Ready;

    // Disable SPI
    pSPIDriver->pSPIx->CR1 &= ~(SPI_CR1_SPE);
}

/*
 * Indicates that the SPIx has completed the data transmission, weak
 * implementation
 *
 * Params:
 *    * pSPIDriver, a pointer to a SPI_DriverTypeDef which contains the SPIx
 * that ended the transmission
 * Returns:
 *    * None
 */
__weak void SPI_CallbackTxCompleted(SPI_DriverTypeDef *pSPIDriver) {
    // nullify the unused variable
    (void)pSPIDriver;
}
