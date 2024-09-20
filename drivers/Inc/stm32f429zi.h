#ifndef __STM32F429ZI_H__
#define __STM32F429ZI_H__

#include <stdint.h>
#include <stddef.h>
#include "stm32f429xx.h"

/* Exported variables */

// Clock frequencies
#define HSE_VALUE 8000000  // High Speed External Clock
#define HSI_VALUE 16000000 // High Speed Internal Clock

// Shortcut definitions
#define __vo volatile
#define __weak __attribute__((weak))

/* Exported TypeDefs */

/*
 * Status used for return statements
 */
typedef enum{
 ERROR,         // Error produced
 OK,            // Execution successfully
 BUSY,          // MCU is busy
}DriverStatus;

/*
 * Enables/Disables a configurable item
 */
typedef enum {
 DISABLE,       // Disable the configurable item
 ENABLE,        // Enable the configurable item
} EnableDisable;

/*
 * Indicates the logical state of a pin
 */
typedef enum {
 LOW,           // Pin logical state is LOW 
 HIGH,          // Pin logical state is HIGH
} PinLogicalLevel;

/*
 * Indicates when a flag is set or reset
 */
typedef enum {
 FLAG_LOW,      // Flag is Reset (LOW logical state)
 FLAG_HIGH,     // Flas is Set (HIGH logical state)
} FlagStatus;

/* Exported functions */
void System_Init(void);


#include "gpio.h"
#include "spi.h"
#include "timers.h"

#endif // !__STM32F429ZI_H__
