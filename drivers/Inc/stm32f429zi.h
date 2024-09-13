#ifndef __STM32F429ZI_H__
#define __STM32F429ZI_H__


#include <stdint.h>
#include <stddef.h>
#include "stm32f429xx.h"

// Clocks values definitions
#define HSE_VALUE 8000000
#define HSI_VALUE 16000000

// Shortcut definitions
#define __vo volatile
#define __weak __attribute__((weak))

typedef enum{ERROR, OK, BUSY }DriverStatus;
typedef enum {DISABLE, ENABLE} EnableDisable;
typedef enum {LOW, HIGH} PinLogicalLevel;
typedef enum {FLAG_LOW, FLAG_HIGH} FlagStatus;

void System_Init(void);


#include "gpio.h"
#include "spi.h"
#include "timers.h"

#endif // !__STM32F429ZI_H__
