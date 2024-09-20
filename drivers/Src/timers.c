#include "stm32f429zi.h"

/* Global variables */

// Updated in the SysTick handler which occurs every 1 ms. Used for delay
// function
volatile uint32_t global_ticks;
// Update in the TIM6 interruption which occurs every 1 s. Used in the Scheduler
volatile uint32_t global_seconds;

/*
 * SysTick initializaiton function
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void SysTick_Init(void) {

    // Configuring for ticking every 1ms with the default clock frequency
    // (16Mhz), Loading the tick count
    // (16Mhz / 1kHz) in the load register of the SysTick Timer and enabling the
    // IRQ.

    SysTick->LOAD = 16000000 / 1000;
    SysTick->VAL = 0x00;

    // Interruption settings. The SysTick is enabled in the delay function only
    // when it is used
    NVIC_SetPriority(SysTick_IRQn, 15);
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; // Processor as Systick Source
    SysTick->CTRL |=
        SysTick_CTRL_TICKINT_Msk; // Interruption triggered when reaching 0
}

/*
 * Handles the SysTick interruption, updates the global ticks count
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void SysTick_Handler(void) { global_ticks++; }

/*
 * Halts the CPU during desired milliseconds
 *
 * Params:
 *    * ms, an 32 bit-wide integer that represents the amount of delays to
 * elapse Returns:
 *    * None
 */
void delay(uint32_t ms) {

    // resets the amount of global ticks
    global_ticks = 0;

    // Enable the SysTick accessing the CTRL register
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    // halt the CPU until the global ticks meet the desired ticks to elapse
    uint32_t count = global_ticks + ms;
    while (global_ticks < count) {
        ;
    }

    // Disable the SysTick interruption
    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk);
}

/*
 * Initialazes the Timer 6 as the scheduler clock source. This is configured to
 * trigger every second.
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void Timer_Init(void) {
    // Enabling Timer clock
    RCC->APB1ENR = (RCC_APB1ENR_TIM6EN);

    // TIM6 is connected to APB1 bus. By default, the pre-escalers between the
    // main clock and the APB1 timer clock are set as one, this means that the
    // APB1 runs at 16 MHz. 1 s interruption can be achieved by the following:
    //    * Timer 6 PSC (prescaler) = 250
    //    * Timer 6 frequency = 16 Mhz / 250 = 64kHz (64k ticks per second)
    //    * Timer ARR (Auto Reload Register) = 64k (to reach 1 second period)
    TIM6->PSC = 250 - 1;
    TIM6->ARR = 64000 - 1;

    Timer_Start();
}

/*
 *  Sets the interruption configuration of the timer 6 and enables the interruption
 *
 *  Params:
 *      * None
 *  Returns:
 *      * None
 */
void Timer_Start(void) {
    // Enable and configure interruption
    TIM6->DIER = (TIM_DIER_UIE);

    NVIC_SetPriority(TIM6_DAC_IRQn, 15);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

/*
 * Enables the timer interruption when the ARR count reaches 0
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void Timer_Enable(void) {
    // enabling the counter
    TIM6->CR1 = (TIM_CR1_CEN);
}


/*
 * Disables the timer interruption
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void Timer_Disable(void) {
    // Enabling the counter
    TIM6->CR1 &= ~(TIM_CR1_CEN);
}


/*
 * Handles the Timer 6 interruption. Entry of the NVIC vector table
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void TIM6_DAC_IRQHandler(void) {
    
    // Check if the interruption is enabled
    if (TIM6->DIER & TIM_DIER_UIE) {
        // Check the Update Interrupt Flag 
        if (TIM6->SR & TIM_SR_UIF) {
            // Clear the flag
            TIM6->SR &= ~(TIM_SR_UIF);
        }
    }
    global_seconds++;
    Scheduler();
}

/*
 * Weak implementation of the scheduler, overriden in user layer
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
__weak void Scheduler(void) { ; }
