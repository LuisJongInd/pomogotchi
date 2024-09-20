#include "gpio.h"

/* Static functions */

static void GPIO_PeripheralClockControl(GPIO_TypeDef *pGPIOx,
                                        EnableDisable EnorDi);
static uint8_t GPIO_get_GPIOx_number(GPIO_TypeDef *pGPIOx);

/*
 * GPIO initilaization function. Used to configure the Pin as desired
 *
 * Params:
 *    * pGPIODriver, pointer to a GPIO_DriverTypeDef structure with the GPIO
 * configurations Returns:
 *    * DriverStatus, a flag that returns the succesfulness of the GPIO
 * initilaization
 */
DriverStatus GPIO_Init(GPIO_DriverTypeDef *pGPIODriver) {
    // Enable clock in order to configure registers

    GPIO_PeripheralClockControl(pGPIODriver->pGPIOx, ENABLE);

    /* Check for incorrect configuration values */
    if ((pGPIODriver->Config.Number > 15) ||
        (pGPIODriver->Config.AlternateFunction > 15))
        return ERROR;

    /* Pin mode configuration */

    // MODERy[1:0] configure each y pin (0..15):
    //    00: Input
    //    01: Output
    //    01: Alternate Function
    //    11: Analog mode

    if (pGPIODriver->InterruptMode == GPIO_It_Disable) {

        pGPIODriver->pGPIOx->MODER &=
            ~(0x3 << (2 * pGPIODriver->Config.Number));
        pGPIODriver->pGPIOx->MODER |= (0x3 & pGPIODriver->Config.Mode)
                                      << (2 * pGPIODriver->Config.Number);

    } else {

        /* Configuring EXTI Falling/Rising modes */

        // EXTI provides two registers to enable the edge to be
        // detected in the GPIO pin. EXTI_RTSR sets Rising edge
        // and EXTI_FTSR sets the Falling edge

        if (pGPIODriver->InterruptMode == GPIO_It_Fall) {
            EXTI->FTSR |= (1 << pGPIODriver->Config.Number);
            EXTI->RTSR &= ~(1 << pGPIODriver->Config.Number);
        } else if (pGPIODriver->InterruptMode == GPIO_It_Rise) {

            EXTI->FTSR &= ~(1 << pGPIODriver->Config.Number);
            EXTI->RTSR |= (1 << pGPIODriver->Config.Number);
        } else if (pGPIODriver->InterruptMode == GPIO_It_RiseFall) {

            EXTI->FTSR |= (1 << pGPIODriver->Config.Number);
            EXTI->RTSR |= (1 << pGPIODriver->Config.Number);
        }

        /* Configuring EXTI Interruption Mode */

        // EXTI lines can be configured as Interruptions or events. To
        // set them as interruptions, EXTI_IMR is used. This sets which
        // EXTIx will be enabled

        EXTI->IMR |= (1 << pGPIODriver->Config.Number);

        /* Configuring System Configuration Controller */

        // SYSCFG_EXTICRx are x(1..4) registers used to configure the
        // input of the EXTIy lines y(0..3). To select the desired input:
        //    0000: PAy pin
        //    0001: PBy pin
        //    0010: PCy pin
        //    0011: PDy pin
        //    0100: PEy pin
        //    0101: PFy pin
        //    0110: PGy pin
        //    0111: PHy pin
        //    1000: PIy pin
        uint8_t x = pGPIODriver->Config.Number / 4;
        uint8_t y = pGPIODriver->Config.Number % 4;
        uint8_t n = GPIO_get_GPIOx_number(pGPIODriver->pGPIOx);

        RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

        SYSCFG->EXTICR[x] |= (n << (y * 4));
        EXTI->PR |= (1 << pGPIODriver->Config.Number);
    }

    /* Output Type  */

    // OTYPERy[0] configure each y pin(0..15):
    //    0: Ouptut Push-pull
    //    1: Output open-drain

    pGPIODriver->pGPIOx->OTYPER &= ~(0x1 << (pGPIODriver->Config.Number));
    pGPIODriver->pGPIOx->OTYPER |=
        (pGPIODriver->Config.OutputType << (pGPIODriver->Config.Number));

    /* Speed configuration */

    // OSPEEDRy[1:0] configure each y pin (0..15):
    //    00: Low Speed
    //    01: Medium Speed
    //    10: High Speed
    //    11: Very High Speed

    pGPIODriver->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pGPIODriver->Config.Number));
    pGPIODriver->pGPIOx->OSPEEDR |= (0x3 & pGPIODriver->Config.Speed)
                                    << (2 * pGPIODriver->Config.Number);

    /* Pull-up/Push-down configuration */

    // PUPDy[1:0] configure each y pin (0..15):
    //    00: No pull-up, pull-down
    //    01: Pull-up
    //    10: pull-down

    pGPIODriver->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIODriver->Config.Number));
    pGPIODriver->pGPIOx->PUPDR |= (0x3 & pGPIODriver->Config.PullUpDown)
                                  << (2 * pGPIODriver->Config.Number);

    /* Alternate Function configuration */

    // AFR(H/L)y[3:0] configure each y pin(0..7) if L or y pin(8..15) if H:
    //    0000: AF0
    //    .
    //    .
    //    .
    //    1111: AF15

    if (pGPIODriver->Config.Mode == GPIO_Mode_AlternateFunction) {

        uint8_t LorH, y;
        // Low or High AFR:
        LorH = pGPIODriver->Config.Number / 8;
        y = pGPIODriver->Config.Number % 8;

        pGPIODriver->pGPIOx->AFR[LorH] &= ~(0x4 << (y * 4));
        pGPIODriver->pGPIOx->AFR[LorH] |=
            (pGPIODriver->Config.AlternateFunction << (y * 4));
    }

    return OK;
}

/*
 * Reads the current state of the desired pin
 *
 * Params:
 *    * pGPIOx, pointer to GPIO_TypeDef which is the desired GPIO port to
 * configure
 *    * PinNumber, 8 bit-wide integer which is the desired Pin of the GPIOx port
 * to read
 * Returns:
 *    * logicalState, an 8 bit-wide integer with the logical value of the
 * desired pin
 */
uint8_t GPIO_Pin_Read(GPIO_TypeDef *pGPIOx, uint8_t PinNumber) {

    /* Reading form single pin in Input Data Registers */
    // IDRy is a read only register which reads the input of the y(0..15) pin
    // of the GPIOx port.

    uint8_t logicalState = ((pGPIOx->IDR >> PinNumber) & 0x1);
    return logicalState;
}

/*
 * Writes to a desired pin in the pGPIOx port
 *
 * Params:
 *    * pGPIOx, pointer to GPIO_TypeDef which is the desired GPIO port to
 * configure
 *    * PinNumber, 8 bit-wide integer which is the desired Pin of the pGPIOx
 * port to write
 *    * LorH, a PinLogicalLevel variable which indicates the logical state to
 * write in the desired pin Returns:
 *    * None
 */
void GPIO_Pin_Write(GPIO_TypeDef *pGPIOx, uint8_t PinNumber,
                    PinLogicalLevel LorH) {
    /* Writing to single pin in Output Data Register */
    // ODRy writes to the y(0..15) pin.
    if (LorH == HIGH) {
        pGPIOx->ODR |= (uint16_t)(0x1 << PinNumber);
    } else {
        pGPIOx->ODR &= (uint16_t) ~(0x1 << PinNumber);
    }
}

/*
 * Toggles the current value of the desired pin
 *
 * Params:
 *    * pGPIOx, pointer to GPIO_TypeDef which is the desired GPIO port to
 * configure
 *    * PinNumber, 8 bit-wide integer which is the desired Pin of the pGPIOx
 * port to toggle
 * Returns:
 *    * None
 */
void GPIO_Pin_Toggle(GPIO_TypeDef *pGPIOx, uint8_t PinNumber) {
    pGPIOx->ODR ^= (uint16_t)(0x1 << PinNumber);
}

/*
 * Enables/Disables the IRQ functionality of the EXTI line
 *
 * Params:
 *    * IRQNumber, an integer defined in the CMSIS device file with the
 * corresponding EXTI line IRQ number
 *    * EnOrDi, an EnableDisable variable to decide whether the peripheral
 * interruption will be enabled or disabled Returns:
 *    * None
 */
void GPIO_IRQ_Control(IRQn_Type IRQNumber, EnableDisable EnOrDi) {

    /* Enabling/Disabling GPIO interruptions*/

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
 *  Configures the priority level of the peripheal
 *
 *  Params:
 *      * IRQNumber, an IRQn_Type that corresponds to the IRQ to configure
 * (defined in CMSIS layer)
 *      * IRQPriority, an uint32_t with the new priority
 *  Returns:
 *      * None
 */
void GPIO_IRQ_PriorityConfig(IRQn_Type IRQNumber, uint32_t IRQPriority) {

    /* Configuring GPIO interruptions priority */

    // To configure the priority, the NVIC_IPRx registers are used. Those are
    // 60 registers used to configure the priority values of each interruptions
    // available

    // Using CMSIS defined function
    __NVIC_SetPriority(IRQNumber, IRQPriority);
}

/*
 * Handles the GPIO interruption. Invoked by the EXTI handler
 *
 * Params:
 *    * PinNumber: PinNumber, an 8 bit-wide integer that corresponds to the
 * PinNumber that arosed the interruption
 * Returns:
 *    * None
 */
void GPIO_IRQ_Handling(uint8_t PinNumber) {
    /* Handling GPIO IRQ interruptions */

    // Whenever an interruption arises, the EXTI_PRx register sets a bit to
    // indicate there is a pending request on the EXTIx line. In order to clean
    // this (and avoid the NVIC continously triggering the EXTIx line), a '1'
    // must be written

    if (EXTI->PR & (1 << PinNumber)) {
        EXTI->PR |= (1 << PinNumber);
        GPIO_Callback_IRQTrigger(PinNumber);
    }
}

/*
 * Enables pGPIOx port clock
 *
 * Params:
 *    * pGPIOx, a pointer to a GPIO_TypeDef which corresponds to the desired
 * GPIOx to enable its clock
 *    * EnorDi, a EnableDisable variable to enable/disable the clock
 * Returns:
 *    * None
 */
static void GPIO_PeripheralClockControl(GPIO_TypeDef *pGPIOx,
                                        EnableDisable EnorDi) {
    // RCC peripheral enables/disables the clock. GPIOx ports are in AHB1 bus
    RCC->AHB1ENR |= (1 << GPIO_get_GPIOx_number(pGPIOx));
}

/*
 * Returns the number of the GPIOx, used in bit shift operations where the
 * x(A..K) corresponds to a position
 *
 * Params:
 *    * pGPIOx, GPIO_TypeDef that corresponds to the GPIOx
 * Returns:
 *    * number, an 8 bit-wide integer with the corresponding number to the x
 * GPIO port
 */
static uint8_t GPIO_get_GPIOx_number(GPIO_TypeDef *pGPIOx) {
    uint8_t number = 0;
    switch ((unsigned long int)pGPIOx) {
    case GPIOA_BASE:
        number = 0;
        break;
    case GPIOB_BASE:
        number = 1;
        break;
    case GPIOC_BASE:
        number = 2;
        break;
    case GPIOD_BASE:
        number = 3;
        break;
    case GPIOE_BASE:
        number = 4;
        break;
    case GPIOF_BASE:
        number = 5;
        break;
    case GPIOG_BASE:
        number = 6;
        break;
    case GPIOH_BASE:
        number = 7;
        break;
    case GPIOI_BASE:
        number = 8;
        break;
    case GPIOJ_BASE:
        number = 9;
        break;
    case GPIOK_BASE:
        number = 10;
        break;
    }
    return number;
}

/*
 * Weak callback implementation of the GPIO interruption trigger
 *
 * Params:
 *    * PinNumber, 8 bit-wide integer that triggered the intertuption
 */
__weak void GPIO_Callback_IRQTrigger(uint8_t PinNumber) {
    // should be implemented in user level code
    // cast to void to avoid unused variable
    (void)PinNumber;
}
