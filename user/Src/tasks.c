#include "tasks.h"

Scheduler_TypeDef scheduler;
static void switch_task(void);

GPIO_DriverTypeDef scheduler_LED;
uint8_t focus_time = 25;
uint8_t short_rest_time = 15;
uint8_t long_rest_time = 5;

void Scheduler(void) {

    //                                  multiply * 60
    if (scheduler.minutes_to_elapse == global_seconds * 60) {
        switch_task();
    }
    if (global_seconds % 60 == 0) {
        uint16_t minutes_left =
            scheduler.minutes_to_elapse - (global_seconds / 60);
        task_MinuteElapsed(minutes_left);
    }
    if (scheduler.Availability == Available) {
        task_Idle();
    }
}

void Start_Scheduler(void) {
    Timer_Enable();
    task_Focus();
    task_MinuteElapsed(focus_time);
}

void task_Focus(void) {
    scheduler.State = State_Focus;
    global_seconds = 0;
    scheduler.minutes_to_elapse = focus_time;
    scheduler.cycles++;
    scheduler.Availability = NotAvailable;
    Image_clearStrings();
    Image_drawString((uint8_t *)" FOCUS\0");
    Image_displayImage();
    scheduler.Availability = Available;
}

void task_ShortRest(void) {
    scheduler.State = State_ShortRest;
    global_seconds = 0;
    scheduler.minutes_to_elapse = short_rest_time;
    scheduler.Availability = NotAvailable;
    Image_clearStrings();
    Image_drawString((uint8_t *)" SHORT\0");
    Image_drawString((uint8_t *)"  REST\0");
    Image_displayImage();
    scheduler.Availability = Available;
}

void task_LongRest(void) {
    scheduler.State = State_LongRest;
    global_seconds = 0;
    scheduler.minutes_to_elapse = long_rest_time;
    scheduler.cycles = 0;
    Image_clearStrings();
    scheduler.Availability = NotAvailable;
    Image_drawString((uint8_t *)" LONG\0");
    Image_drawString((uint8_t *)"  REST\0");
    Image_displayImage();
    scheduler.Availability = Available;
}

void task_MinuteElapsed(uint8_t minutes_left) {
    Image_clearMinutesLeft();
    Image_drawMinutesLeft(minutes_left);
    Image_displayImage();
}

void task_Idle(void) { __WFE(); }

static void switch_task(void) {

    if ((scheduler.State == State_Focus) && (scheduler.cycles <= 3)) {
        task_ShortRest();
    } else if ((scheduler.State == State_Focus)) {
        task_LongRest();
    } else {
        task_Focus();
    }
}
