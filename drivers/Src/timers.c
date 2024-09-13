#include "stm32f429zi.h"

volatile uint32_t global_ticks;
volatile uint32_t global_seconds;

static void TIM6_GPIO_Init(void);

void SysTick_Init(void) {
    // Configuring for ticking every 1ms with the default clock frequency
    // (16Mhz), Loading the tick count
    // (16Mhz / 1kHz) in the load register of the SysTick Timer and enabling the
    // IRQ.

    SysTick->LOAD = 16000000 / 1000;
    SysTick->VAL = 0x00;
    NVIC_SetPriority(SysTick_IRQn, 15);
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; // Processor as Systick Source
    SysTick->CTRL |=
        SysTick_CTRL_TICKINT_Msk; // Interruption triggered when reaching 0
}

void SysTick_Handler(void) { global_ticks++; }

void delay(uint32_t ms) {
    global_ticks = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // Enabling SysTick
    uint32_t count = global_ticks + ms;
    while (global_ticks < count) {
        ;
    }
    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk); // Disabling SysTick
}

void Timer_Init(void) {
    // Enabling Timer clock
    RCC->APB1ENR = (RCC_APB1ENR_TIM6EN);

    TIM6->PSC = 250 - 1;   // Preescaler 250
    TIM6->ARR = 64000 - 1; // Auto Reaload Register (should not exceed 16bits)

    TIM6_GPIO_Init();
    Timer_Start();
}

void Timer_Start(void) {
    // Enable and configure interruption
    TIM6->DIER = (TIM_DIER_UIE);

    NVIC_SetPriority(TIM6_DAC_IRQn, 16);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    // Enabling the Counter
    TIM6->CR1 = (TIM_CR1_CEN);
}

void TIM6_DAC_IRQHandler(void) {
    if (TIM6->DIER & 0x01) {
        if (TIM6->SR & 0x01) {
            TIM6->SR &= ~(0X01 << 0);
        }
    }
    GPIO_Pin_Toggle(GPIOB, 12);
    global_seconds++;
}

static void TIM6_GPIO_Init(void) {
    // For testing porpuses
    GPIO_DriverTypeDef tim6_gpio;
    tim6_gpio.pGPIOx = GPIOB;
    tim6_gpio.Config.Number = 12;
    tim6_gpio.Config.Mode = GPIO_Mode_Output;
    tim6_gpio.Config.OutputType = GPIO_OpType_PushPull;
    tim6_gpio.Config.Speed = GPIO_Speed_VeryHigh;
    tim6_gpio.Config.PullUpDown = GPIO_PuPd_None;
    GPIO_Init(&tim6_gpio);
}
