#ifndef __EINKPAPER_H__
#define __EINKPAPER_H__

#include "stm32f429zi.h"

extern uint8_t einkDisplay_Width;
extern uint8_t einkDisplay_Height;
extern uint8_t *pImage;

// display has a 122 x 250 resolution

typedef struct {
  // GPIO related variables 
  GPIO_TypeDef *pGPIOx;
  uint8_t DC_PinNumber;
  uint8_t CS_PinNumber;
  uint8_t Busy_PinNumber;
  uint8_t Reset_PinNumber;
  // SPI related variables
} EinkPaper_TypeDef;



void eInkDisplay_Init(void);
void eInkDisplay_FillWhite(void);
void eInkDisplay_FillBlack(void);
void eInkDisplay_DisplayImage(uint8_t *pImage);

#endif // !__EINKPAPER_H__
