#include "Image.h"
#include <stdio.h>

// Variables to keep track of the current character position, pos_x corresponds
// to the x position in the x directoin of the display whereas pos_y corresponds
// to the line number. Every time there is a char drawing operations, these will
// be updated.
uint8_t pos_x = 0;
uint8_t pos_y = 0;
uint8_t *current_tamagotchi = focus_monkey;

static void Image_drawChar(uint8_t *c);

void Image_displayImage(void) {
    eInkDisplay_DisplayImage(Image_array, current_tamagotchi);
}

static void Image_drawChar(uint8_t *c) {
    uint8_t letterNumber = *c - (uint8_t)'0';

    // Image (array representation of the display) is 16 bytes width (122 pixels
    // / 8 bits), each char occupies 2 bytes:
    uint8_t display_width = (einkDisplay_Width / 8) + 1;
    uint8_t chars_in_x = display_width / char_width - 1;

    // If the char is an space, there's no need of modifying the Image array
    if (*c != ' ') {
        for (uint8_t y = 0; y < char_height; y++) {
            for (uint8_t x = 0; x < char_width; x++) {
                // Every pixel in y direction is at y (line of the char) *
                // display_width, we need to add an offset that corresponds to
                // the position in y (the current y position of the char to
                // write), same case in x direction. The corresponding byte of
                // the char is at y * char_widt (since each line in y direction
                // has 2 bytes of the x direction) plus an offset corresponding
                // to the ascii number in the array
                Image_array[(y * display_width) +
                            (display_width * char_height * pos_y) + (x) +
                            (pos_x * 2)] =
                    alphaNumbers[x + (y * char_width) +
                                 (letterNumber * char_height * char_width)];
            }
        }
    }
    // Updating the current line numbers
    pos_x++;
    pos_x %= chars_in_x;
    if ((pos_x) == 0) {
        pos_y++;
        pos_y %= display_width;
    }
}

void Image_clearStrings(void) {

    uint8_t display_width = (einkDisplay_Width / 8) + 1;
    uint8_t chars_in_x = display_width / char_width - 1;

    uint8_t amountOfChars = (pos_y * chars_in_x) + (pos_x);

    pos_x = 0;
    pos_y = 0;
    uint8_t c = ':';
    for (uint8_t i = 0; i < amountOfChars; i++) {
        Image_drawChar(&c);
    }
    pos_x = 0;
    pos_y = 0;
}

void Image_drawTamagotchi(void) { ; }

void Image_drawString(uint8_t *c) {
    while (*c != '\0') {
        Image_drawChar(c++);
    }
    pos_y++;
    pos_x = 0;
}

void Image_drawMinutesLeft(uint16_t minutesLeft) {
    pos_y = 4;
    char buf[8];
    snprintf(buf, 8, "%d MIN\n", minutesLeft);
    uint8_t i = 0;
    while (buf[i] != '\n') {
        Image_drawChar((uint8_t *)&buf[i++]);
    }
    pos_x = 0;
}

void Image_clearMinutesLeft(void) {

    pos_x = 0;
    pos_y = 4;
    uint8_t c = ':';
    for (uint8_t i = 0; i < 6; i++) {
        Image_drawChar(&c);
    }
    pos_x = 0;
    pos_y = 0;
}
