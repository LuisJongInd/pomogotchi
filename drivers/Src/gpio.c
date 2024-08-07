#include "gpio.h"

static void GPIO_PeripheralClockControl(GPIO_TypeDef *GPIOx,
                                        EnableDisable EnorDi);

DriverStatus GPIO_Init(GPIO_ConfigTypeDef *pGPIOConfig) {
    // Enable clock in order to configure registers

    GPIO_PeripheralClockControl(pGPIOConfig->pGPIOx, ENABLE);

    /* Check for incorrect configuration values */
    if ((pGPIOConfig->PinConfig.Number > 15) ||
        (pGPIOConfig->PinConfig.AlternateFunction > 15))
        return ERROR;

    /* Pin mode configuration */

    // MODERy[1:0] configure each y pin (0..15):
    //    00: Input
    //    01: Output
    //    01: Alternate Function
    //    11: Analog mode

    if (pGPIOConfig->InterruptMode == GPIO_IT_DISABLE) {

        pGPIOConfig->pGPIOx->MODER &=
            ~(0x3 << (2 * pGPIOConfig->PinConfig.Number));
        pGPIOConfig->pGPIOx->MODER |= (0x3 & pGPIOConfig->PinConfig.Mode)
                                      << (2 * pGPIOConfig->PinConfig.Number);

    } else {
        // TODO (Interrupt Mode)
    }

    /* Output Type  */

    // OTYPERy[0] configure each y pin(0..15):
    //    0: Ouptut Push-pull
    //    1: Output open-drain

    pGPIOConfig->pGPIOx->OTYPER &= ~(0x1 << (pGPIOConfig->PinConfig.Number));
    pGPIOConfig->pGPIOx->OTYPER |=
        (pGPIOConfig->PinConfig.OutputType << (pGPIOConfig->PinConfig.Number));

    /* Speed configuration */

    // OSPEEDRy[1:0] configure each y pin (0..15):
    //    00: Low Speed
    //    01: Medium Speed
    //    10: High Speed
    //    11: Very High Speed

    pGPIOConfig->pGPIOx->OSPEEDR &=
        ~(0x3 << (2 * pGPIOConfig->PinConfig.Number));
    pGPIOConfig->pGPIOx->OSPEEDR |= (0x3 & pGPIOConfig->PinConfig.Speed)
                                    << (2 * pGPIOConfig->PinConfig.Number);

    /* Pull-up/Push-down configuration */

    // PUPDy[1:0] configure each y pin (0..15):
    //    00: No pull-up, pull-down
    //    01: Pull-up
    //    10: pull-down

    pGPIOConfig->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIOConfig->PinConfig.Number));
    pGPIOConfig->pGPIOx->PUPDR |= (0x3 & pGPIOConfig->PinConfig.PullUpDown)
                                  << (2 * pGPIOConfig->PinConfig.Number);

    /* Alternate Function configuration */

    // AFR(H/L)y[3:0] configure each y pin(0..7) if L or y pin(8..15) if H:
    //    0000: AF0
    //    .
    //    .
    //    .
    //    1111: AF15

    if (pGPIOConfig->PinConfig.Mode == GPIO_MODE_AF) {

        uint8_t LorH, y;
        // Low or High AFR:
        LorH = pGPIOConfig->PinConfig.Number / 8;
        y = pGPIOConfig->PinConfig.Number % 8;

        pGPIOConfig->pGPIOx->AFR[LorH] &= ~(0x4 << y);
        pGPIOConfig->pGPIOx->AFR[LorH] |=
            (pGPIOConfig->PinConfig.AlternateFunction << y);
    }

    return OK;
}

// TODO
static void GPIO_PeripheralClockControl(GPIO_TypeDef *GPIOx,
                                        EnableDisable EnorDi) {
    switch ((unsigned long int)GPIOx) {
    case GPIOA_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN_Msk;
        break;
    case GPIOB_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN_Msk;
        break;
    case GPIOC_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN_Msk;
        break;
    case GPIOD_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN_Msk;
        break;
    case GPIOE_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN_Msk;
        break;
    case GPIOF_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN_Msk;
        break;
    case GPIOG_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN_Msk;
        break;
    case GPIOH_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN_Msk;
        break;
    case GPIOI_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN_Msk;
        break;
    case GPIOJ_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN_Msk;
        break;
    case GPIOK_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOKEN_Msk;
        break;
    default:
        break;
    }
}
