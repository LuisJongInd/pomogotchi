#include "tasks.h"

Scheduler_TypeDef scheduler;
static void switch_task(void);
static void GPIO_buttonInit(void);
static void task_Button(void);

GPIO_DriverTypeDef scheduler_button;
uint8_t focus_time = 25;
uint8_t short_rest_time = 10;
uint8_t long_rest_time = 15;
volatile uint8_t poweredOff = 0;

void Scheduler(void) {

    if (scheduler.minutes_to_elapse == global_seconds / 60) {
        switch_task();
    }
    if (global_seconds % 60 == 0) {
        uint16_t minutes_left = scheduler.minutes_to_elapse - (global_seconds / 60);
        task_MinuteElapsed(minutes_left);
    }
    if (scheduler.Availability == Available) {
        task_Idle();
    }
}

void Start_Scheduler(void) {
    GPIO_buttonInit();
    Timer_Enable();
    task_Focus();
    task_MinuteElapsed(focus_time);
}

void task_Focus(void) {
    current_tamagotchi = focus_monkey;
    scheduler.State = State_Focus;
    global_seconds = 0;
    scheduler.minutes_to_elapse = focus_time;
    scheduler.cycles++;
    scheduler.Availability = NotAvailable;
    Image_clearStrings();
    Image_clearMinutesLeft();
    Image_drawString((uint8_t *)" FOCUS\0");
    Image_displayImage();
    scheduler.Availability = Available;
}

void task_ShortRest(void) {

    current_tamagotchi = beer_monkey;
    scheduler.State = State_ShortRest;
    global_seconds = 0;
    scheduler.minutes_to_elapse = short_rest_time;
    scheduler.Availability = NotAvailable;
    Image_clearStrings();
    Image_clearMinutesLeft();
    Image_drawString((uint8_t *)" SHORT\0");
    Image_drawString((uint8_t *)"  REST\0");
    Image_displayImage();
    scheduler.Availability = Available;
}

void task_LongRest(void) {
    current_tamagotchi = sleeping_monkey;
    scheduler.State = State_LongRest;
    global_seconds = 0;
    scheduler.minutes_to_elapse = long_rest_time;
    scheduler.cycles = 0;
    Image_clearStrings();
    Image_clearMinutesLeft();
    scheduler.Availability = NotAvailable;
    Image_drawString((uint8_t *)" LONG\0");
    Image_drawString((uint8_t *)"  REST\0");
    Image_displayImage();
    scheduler.Availability = Available;
}

void task_MinuteElapsed(uint16_t minutes_left) {
    Image_clearMinutesLeft();
    Image_drawMinutesLeft(minutes_left);
    Image_displayImage();
}

void task_Idle(void) { __WFE(); }

static void task_Button(void) {
    if (poweredOff) {
        poweredOff = 0;
        current_tamagotchi = focus_monkey;
        scheduler.cycles = 0;
        Timer_Enable();
        task_Focus();
        task_MinuteElapsed(focus_time);
    } else {
        poweredOff = 1;
        Timer_Disable();
        current_tamagotchi = empty_tamagotchi;
        Image_clearStrings();
        Image_clearMinutesLeft();
        Image_drawString((uint8_t *)" BYE\0");
        Image_drawString((uint8_t *)" BYE\0");
        Image_displayImage();
        eInkDisplay_FillWhite();
        __WFE();
    }
}

static void switch_task(void) {

    if ((scheduler.State == State_Focus) && (scheduler.cycles <= 2)) {
        task_ShortRest();
    } else if ((scheduler.State == State_Focus)) {
        task_LongRest();
    } else {
        task_Focus();
    }
}

static void GPIO_buttonInit(void) {
    scheduler_button.pGPIOx = GPIOC;
    scheduler_button.InterruptMode = GPIO_It_Rise;
    scheduler_button.Config.Number = 13;
    scheduler_button.Config.Mode = GPIO_Mode_Input;
    scheduler_button.Config.PullUpDown = GPIO_PuPd_None;

    GPIO_Init(&scheduler_button);
    GPIO_IRQ_Control(EXTI15_10_IRQn, ENABLE);
    GPIO_IRQ_PriorityConfig(EXTI15_10_IRQn, 25);
}

void GPIO_Callback_IRQTrigger(uint8_t PinNumber) {
    if (PinNumber == 13) {
        task_Button();
    }
}
