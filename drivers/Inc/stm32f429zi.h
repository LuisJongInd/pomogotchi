#ifndef __STM32F429ZI_H__
#define __STM32F429ZI_H__


#include <stdint.h>
#include "stm32f429xx.h"

// Clocks values definitions
#define HSE_VALUE 8000000
#define HSI_VALUE 16000000

// Shortcut definitions
#define __vo volatile
#define __weak __attribute__((weak))

typedef enum{ERROR, OK}DriverStatus;
typedef enum {DISABLE, ENABLE} EnableDisable;
typedef enum {LOW, HIGH} PinLogicalLevel;



#include "gpio.h"
#include "spi.h"

#endif // !__STM32F429ZI_H__
