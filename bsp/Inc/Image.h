#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "einkPaper_2_13.h"

// eInkPaper widht = 122
// eInkPaper height = 250 
extern uint8_t Image[ ];
extern uint8_t alphaNumbers[];

extern uint8_t char_height;
extern uint8_t char_width;

void Image_displayImage(void);
void Image_drawString(uint8_t *c);


#endif
