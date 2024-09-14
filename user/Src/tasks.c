#include "tasks.h"

Scheduler_TypeDef scheduler;
static void switch_task(void);

void Scheduler(void) {
    //                                  multiply * 60
    if (scheduler.minutes_to_elapse == global_seconds) {
        switch_task();
    } else if (global_seconds % 60 == 0) {

        uint8_t minutes_left =
            scheduler.minutes_to_elapse - (global_seconds * 60);
        task_MinuteElapsed(minutes_left);
    }
    // task_Idle();
}

void task_Focus(void) {
    scheduler.State = State_Focus;
    global_seconds = 0;
    scheduler.minutes_to_elapse = 25;
    scheduler.cycles++;
    Image_clearStrings();
    Image_drawString((uint8_t *)" FOCUS\0");
    Image_displayImage();
}

void task_ShortRest(void) {
    scheduler.State = State_ShortRest;
    global_seconds = 0;
    scheduler.minutes_to_elapse = 5;
    Image_clearStrings();
    Image_drawString((uint8_t *)" SHORT\0");
    Image_drawString((uint8_t *)"  REST\0");
    Image_displayImage();
}

void task_LongRest(void) {
    scheduler.State = State_LongRest;
    global_seconds = 0;
    scheduler.minutes_to_elapse = 15;
    scheduler.cycles = 0;
    Image_clearStrings();
    Image_drawString((uint8_t *)" LONG\0");
    Image_drawString((uint8_t *)"  REST\0");
    Image_displayImage();
}

void task_MinuteElapsed(uint8_t minutes_left) {}

void task_Idle(void) { __WFI(); }

static void switch_task(void) {

    if ((scheduler.State == State_Focus) && (scheduler.cycles <= 3)) {
        task_ShortRest();
    } else if ((scheduler.State == State_Focus)) {
        task_LongRest();
    } else {
        task_Focus();
    }
}
