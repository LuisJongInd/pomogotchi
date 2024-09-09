#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "einkPaper_2_13.h"

// eInkPaper widht = 122
// eInkPaper height = 250 
extern uint8_t Image[ ((122 / 8) + 1) * 250 ];

void Image_displayImage(void);


#endif
