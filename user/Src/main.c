#include "main.h"
#include <string.h>

void GPIO_initialization(void);

char msg[] = "Hello world";
/*
 * This test will send data over SPI in blocking mode
 */

int main(void) {
    System_Init();
    eInkDisplay_Init();

    Image_drawString((uint8_t *)" SHORT\0");
    Image_drawString((uint8_t *)"  REST\0");
    Image_displayImage();

    delay(1000);
    eInkDisplay_FillWhite();

    delay(1000);
    Image_clearStrings();
    Image_drawString((uint8_t *)" LONG\0");
    Image_drawString((uint8_t *)"  REST\0");
    Image_displayImage();
    eInkDisplay_FillWhite();

    Test();

    while (1) {
        ;
    }

    return 0;
}

void Test(void) {
    for (uint8_t i = 0; i < 1; i++) {
        ;
    }
}
