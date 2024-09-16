#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "einkPaper_2_13.h"

// eInkPaper widht = 122
// eInkPaper height = 250 
extern uint8_t Image_array[ ];
extern uint8_t alphaNumbers[];
extern uint8_t focus_monkey[];
extern uint8_t *current_tamagotchi;

extern uint8_t char_height;
extern uint8_t char_width;

void Image_displayImage(void);
void Image_drawString(uint8_t *c);
void Image_drawTamagotchi(void);
void Image_clearStrings(void);
void Image_clearMinutesLeft(void);
void Image_drawMinutesLeft(uint8_t minutesLeft);


#endif
