#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "einkPaper_2_13.h"

/* Extern variables */

// display array, used to write text
extern uint8_t Image_array[ ];

// Tamagotchi image, used to display new image
extern uint8_t focus_monkey[];

// Pointer to the Tamagotchi image that will be displayed
extern uint8_t *current_tamagotchi;

// Array that contains the letters an numbers
extern uint8_t alphaNumbers[];

// Variables that defines the char height and width
extern uint8_t char_height;
extern uint8_t char_width;


/* Exported functions */

// Draw functions
void Image_drawString(uint8_t *c);
void Image_drawMinutesLeft(uint16_t minutesLeft);

// Clear functions
void Image_clearStrings(void);
void Image_clearMinutesLeft(void);

// Display function
void Image_displayImage(void);


#endif
