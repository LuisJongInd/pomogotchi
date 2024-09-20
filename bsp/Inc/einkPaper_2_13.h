#ifndef __EINKPAPER_H__
#define __EINKPAPER_H__

#include "stm32f429zi.h"

/* Extern variables */

// Display sizes
extern uint8_t einkDisplay_Width;
extern uint8_t einkDisplay_Height;

// Pointer to the current Tamgotchi image
extern uint8_t *pImage;

/* Exported TypeDefs  */

/*
 * Gathers the pin numbering of the used GPIO pins by the e-ink paper. All of these pins should be on the same GPIOx port
 */
typedef struct {
  GPIO_TypeDef *pGPIOx;         // GPIOx common to all GPIO pins used in the display
  uint8_t DC_PinNumber;         // Data Selection pin number (outupt), defines if the byte sent is data or a command
  uint8_t CS_PinNumber;         // Chip Selection pin number (output), notifies the display to expect new incomming data
  uint8_t Busy_PinNumber;       // Busy Pin number (input), to inform that the display is busy, no operation should be done
  uint8_t Reset_PinNumber;      // Reset Pin number (output), resets the display
} EinkPaper_TypeDef;


/* Exported functions */

// Initialization function
void eInkDisplay_Init(void);

// Clear functions
void eInkDisplay_FillWhite(void);
void eInkDisplay_FillBlack(void);

// Display function
void eInkDisplay_DisplayImage(uint8_t *pImage, uint8_t *character_bitmap);

#endif // !__EINKPAPER_H__
