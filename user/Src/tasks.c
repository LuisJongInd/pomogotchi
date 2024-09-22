#include "tasks.h"

/* Global variables */

// Type structure variables
Scheduler_TypeDef scheduler;
GPIO_DriverTypeDef scheduler_button;

// Time of each Pomogotchi technique state
uint8_t focus_time = 25;
uint8_t short_rest_time = 5;
uint8_t long_rest_time = 15;
uint8_t amount_of_cycles = 3;

// Flag that indicates whether the device was On or Off (after button IRQ
// triggering)
volatile uint8_t poweredOff = 0;

/* Static functions */

static void switch_task(void);
static void GPIO_buttonInit(void);
static void task_Button(void);

/* Function implementations */

/*
 * The scheduler is invoked every second by the Timer 6 interruption. It handles
 * the following:
 *    * Switching the task whenever the time of the current state is met.
 *    * Whenever a minute elapses, display the current time left.
 *    * Invoke the idle state when there is no display updating operations.
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void Scheduler(void) {

    // Switch the task when the time of the current state has elapsed
    if (scheduler.minutes_to_elapse == global_seconds / 60) {
        // handler that switches the next task based on a state machine
        switch_task();
    }
    // Check if a minute has elapsed, not exclusive of the task switching in
    // order to display the time left after swithing to the next state
    if (global_seconds % 60 == 0) {
        uint16_t minutes_left =
            scheduler.minutes_to_elapse - (global_seconds / 60);
        task_MinuteElapsed(minutes_left);
    }
    if (scheduler.Availability == Available) {
        task_Idle();
    }
}

/*
 * Initialazes the built-in button that triggers a new task and switches On/Off
 * the device. Enables the Timer to trigger the strigger the scheduler and
 * invokes the first required tasks. Invoked from main function after hardware
 * intitializations.
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void Start_Scheduler(void) {
    GPIO_buttonInit();
    Timer_Enable();
    task_Focus();
    task_MinuteElapsed(focus_time);
}

/*
 * Handles the Focus state. Sets scheduler variables and the corresponding image
 * of it. This state is always invoked after any kind of rest (long or short).
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void task_Focus(void) {
    current_tamagotchi = focus_monkey;

    // Resetting global seconds variable to start over the count
    global_seconds = 0;

    // Setting the current state and the time to elapse
    scheduler.State = State_Focus;
    scheduler.minutes_to_elapse = focus_time;

    // Updates the amount of cycles, used to know whenever a long rest is next
    scheduler.cycles++;

    // Avoid to call the Idle task
    scheduler.Availability = NotAvailable;

    // Display operations
    Image_clearStrings();
    Image_clearMinutesLeft();
    Image_drawString((uint8_t *)" FOCUS\0");
    Image_displayImage();

    // Release the CPU
    scheduler.Availability = Available;
}

/*
 * Handles the Short Rest state. Invoked after every focus state except when the
 * amount of cycles is greater than amount_of_cycles.
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void task_ShortRest(void) {
    current_tamagotchi = beer_monkey;

    // Resetting global seconds variable to start over the count
    global_seconds = 0;

    // Setting the current state and the time to elapse
    scheduler.State = State_ShortRest;
    scheduler.minutes_to_elapse = short_rest_time;

    // Avoid to call the Idle task
    scheduler.Availability = NotAvailable;

    // Display operations
    Image_clearStrings();
    Image_clearMinutesLeft();
    Image_drawString((uint8_t *)" SHORT\0");
    Image_drawString((uint8_t *)"  REST\0");
    Image_displayImage();

    // Release the CPU
    scheduler.Availability = Available;
}

/*
 * Handles the Long Rest state. Invoked after focus state was invoked at least
 * amount_of_cycles times.
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void task_LongRest(void) {
    current_tamagotchi = sleeping_monkey;

    // Resetting global seconds variable to start over the count
    global_seconds = 0;

    // Setting the current state and the time to elapse
    scheduler.State = State_LongRest;
    scheduler.minutes_to_elapse = long_rest_time;

    // Reset the cycles variable to run again short rests
    scheduler.cycles = 0;

    // Avoid to call the Idle task
    scheduler.Availability = NotAvailable;

    // Display operations
    Image_clearStrings();
    Image_clearMinutesLeft();
    Image_drawString((uint8_t *)" LONG\0");
    Image_drawString((uint8_t *)"  REST\0");
    Image_displayImage();

    // Release the CPU
    scheduler.Availability = Available;
}
/*
 * Displays the minutes elapsed on the screen.
 *
 * Params:
 *    * minutes_left: 16 bit-wide integer that indicates the amount of minutes
 * left Returns:
 *    * None
 */
void task_MinuteElapsed(uint16_t minutes_left) {
    // Clear the previous minutes displayed
    Image_clearMinutesLeft();
    // Draw on the image array the minutes left sent by the scheduler
    Image_drawMinutesLeft(minutes_left);
    Image_displayImage();
}

/*
 * Invokes the WFE (Wait For Event) instruction, which is a CPU instruction.
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void task_Idle(void) {
    // Using macro defined in cmsis_gcc header file.
    __WFE();
}

/*
 * Handles the built-in button interruption. Checks wether the MCU was powered
 * off or not.
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
static void task_Button(void) {
    if (poweredOff) {
        // Set the variable as not powered off (whenever is pressed again the
        // button, it will turn off the device)
        poweredOff = 0;

        // Re-initialazes all variables, start with focus state again
        current_tamagotchi = focus_monkey;
        scheduler.cycles = 0;

        // Display operations
        Timer_Enable();
        task_Focus();
        task_MinuteElapsed(focus_time);
    } else {
        // Set the variable as powered off (whenever the button is pressed
        // again, the device will turn on and follow the re-initialization
        // sequence)
        poweredOff = 1;

        // Disable the timer to avoid triggering the scheduler
        Timer_Disable();

        // emtpy the image
        current_tamagotchi = empty_tamagotchi;

        // Display operations
        Image_clearStrings();
        Image_clearMinutesLeft();
        Image_drawString((uint8_t *)" BYE\0");
        Image_drawString((uint8_t *)" BYE\0");
        Image_displayImage();
        eInkDisplay_FillWhite();

        // Put the CPU to sleep with Wait For Event instruction
        __WFE();
    }
}

/*
 * Defines how the task will switch based on the current conditions
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
static void switch_task(void) {

    if ((scheduler.State == State_Focus) &&
        (scheduler.cycles <= amount_of_cycles)) {
        task_ShortRest();
    } else if ((scheduler.State == State_Focus)) {
        task_LongRest();
    } else {
        task_Focus();
    }
}

/*
 * Buil-in initialization as interruption mode. Set the trigger interruption on
 * the falling edge
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
static void GPIO_buttonInit(void) {
    // built-in button configuration
    scheduler_button.pGPIOx = GPIOC;
    scheduler_button.InterruptMode = GPIO_It_Fall;
    scheduler_button.Config.Number = 13;
    scheduler_button.Config.Mode = GPIO_Mode_Input;
    scheduler_button.Config.PullUpDown = GPIO_PuPd_None;
    GPIO_Init(&scheduler_button);

    // Interruption confuguration
    GPIO_IRQ_Control(EXTI15_10_IRQn, ENABLE);
    GPIO_IRQ_PriorityConfig(EXTI15_10_IRQn, 25);
}

/*
 * Calls the task button whenever the built-in function is triggered. Invoked by
 * the GPIO API.
 *
 * Params:
 *    * Pin number: Corresponds to the Pin number that triggered the
 * interruption
 */
void GPIO_Callback_IRQTrigger(uint8_t PinNumber) {
    if (PinNumber == 13) {
        task_Button();
    }
}
