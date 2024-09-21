#include "Image.h"
#include <stdio.h>

// Variables to keep track of the current character position, pos_x corresponds
// to the x position in the x directoin of the display whereas pos_y corresponds
// to the line number. Every time there is a char drawing operations, these will
// be updated.
static uint8_t pos_x = 0;
static uint8_t pos_y = 0;

// Pointer to the current tamagotchi to display
uint8_t *current_tamagotchi = focus_monkey;

/* Static functions */
static void Image_drawChar(uint8_t *c);

/*
 * Displays the current array in the e-ink paper
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void Image_displayImage(void) {
    // Invoking the bsp e-ink function
    eInkDisplay_DisplayImage(Image_array, current_tamagotchi);
}

/*
 * Draws a single char in the curent position (pos_x, pos_y)
 *
 * Params:
 *    * c, a pointer to a 8 bit-wide integer which is the desired char to draw
 * Returns:
 *    * None
 */
static void Image_drawChar(uint8_t *c) {
    // Finding the character possition in alphaNumbers array (starts with '0')
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
    // If the amount of maximum chars in x exceeds, write in a new line
    pos_x %= chars_in_x;
    if ((pos_x) == 0) {
        pos_y++;
        pos_y %= display_width;
    }
}

/*
 * Eliminates all the written chars in the display
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void Image_clearStrings(void) {

    // Get the maximum amounf of chars that fit in x direction
    uint8_t display_width = (einkDisplay_Width / 8) + 1;
    uint8_t chars_in_x = display_width / char_width - 1;

    // Find the amount of already written chars to clear
    uint8_t amountOfChars = (pos_y * chars_in_x) + (pos_x);

    // Start writting in the origin of the display
    pos_x = 0;
    pos_y = 0;

    // Use a char in the alphaNumbers array that is empty
    uint8_t c = ':';

    // Use drawchar function to draw an empty char instead
    for (uint8_t i = 0; i < amountOfChars; i++) {
        Image_drawChar(&c);
    }

    // Set the current position in the origin to start writting in it
    pos_x = 0;
    pos_y = 0;
}

/*
 * Draw a string in the display
 *
 * Params:
 *    * c, a pointer to a 8 bit-wide integer which is a pointer to a string
 * Returns:
 *    * None
 */
void Image_drawString(uint8_t *c) {
    // Draw the current char until a null terminator is encountered
    while (*c != '\0') {
        Image_drawChar(c++);
    }

    // Update the position, move to a new line
    pos_y++;
    pos_x = 0;
}

/*
 * Draws the amount of minutes left in the display
 *
 * Params:
 *    * minutesLeft, a 16 bit-wide integer wich contains the current amount of
 * minutes left Returns:
 *    * None
 */
void Image_drawMinutesLeft(uint16_t minutesLeft) {

    // Draw in the fourth line
    pos_y = 4;

    // Create a buffer to contain a string
    char buf[8];

    // Create a string with the current amount of minutesLeft
    snprintf(buf, 8, "%d MIN\n", minutesLeft);

    // Draw a char until an escape line is met
    uint8_t i = 0;
    while (buf[i] != '\n') {
        Image_drawChar((uint8_t *)&buf[i++]);
    }
    // Update the position in x
    pos_x = 0;
}

/*
 * Clears the line where the minutes left are written
 *
 * Params:
 *    * None
 * Returns:
 *    * None
 */
void Image_clearMinutesLeft(void) {

    // Sets the cursos at the beginning of the minutes left line
    pos_x = 0;
    pos_y = 4;

    // Use a character that has no representation in the alphaNumbers array
    uint8_t c = ':';
    for (uint8_t i = 0; i < 6; i++) {
        Image_drawChar(&c);
    }
    // Sets the position at the origin
    pos_x = 0;
    pos_y = 0;
}
