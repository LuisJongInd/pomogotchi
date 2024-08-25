#ifndef __EINKPAPER_H__
#define __EINKPAPER_H__

#include "stm32f429zi.h"


typedef struct {
  // GPIO related variables 
  GPIO_TypeDef *pGPIOx;
    uint8_t DC_PinNumber;
  uint8_t Busy_PinNumber;
  // SPI related variables
} EinkPaper_TypeDef;



void eInkDisplay_Init(void);

#endif // !__EINKPAPER_H__
