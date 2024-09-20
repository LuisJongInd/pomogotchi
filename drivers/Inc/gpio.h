#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f429zi.h"

/* Exported TypeDefs */

/*
 * Defines the different modes of the GPIO pin
 */
typedef enum {
  GPIO_Mode_Input,             // Input Pin, reads the logical state of the pin
  GPIO_Mode_Output,            // Output Pin, sets the logical state of the pin
  GPIO_Mode_AlternateFunction, // Alternate Function, maps the pin to another functionality of the MCU
  GPIO_MODE_Analog,            // Analog Mode, generate analog signals
} GPIO_Config_Mode;

/*
 * Sets the configuration of the output
 */
typedef enum {
 GPIO_OpType_PushPull,          // Push-Pull configuration
 GPIO_OpTypE_OpenDrain,         // Open Drain configuration
} GPIO_Config_OpType;

/*
 * Configures the speed of the output of the GPIO
 */
typedef enum {
 GPIO_Speed_Low,                // Low speed
 GPIO_Speed_Medium,             // Medium speed
 GPIO_Speed_High,               // High speed
 GPIO_Speed_VeryHigh,           // Very High speed
} GPIO_Config_Speed;

/*
 * Defines wich the configurations the Pull-Up/Pull-Down resistors
 */
typedef enum {
  GPIO_PuPd_None,               // No resistor used
  GPIO_PuPd_PullUp,             // Pull Down resistor used
  GPIO_PuPd_PullDown,           // Pull Up and Pull down resistor used
} GPIO_Config_PuPd;


/*
 * Configures the interruption modes of the GPIO pin
 */
typedef enum {
  GPIO_It_Disable,              // Not interruption used
  GPIO_It_Fall,                 // Interruption triggered in Falling edge 
  GPIO_It_Rise,                 // Interruption triggered in Rising edge
  GPIO_It_RiseFall              // Interruption triggered in both edges
}GPIO_It_Mode;

/*
 * Configures the GPIO pin
 */
typedef struct{
  uint8_t Number;               // Defines the number of the GPIO pin (0..15)
  GPIO_Config_Mode Mode;        // Selects the mode of the pin, can be any value of GPIO_Config_Mode
  GPIO_Config_OpType OutputType;// Configures the type of Output can be any value of GPIO_Config_OpType
  GPIO_Config_Speed Speed;      // Selects the speed of the Pin, used in output mode. Can be any value of GPIO_Config_Speed
  GPIO_Config_PuPd PullUpDown;  // Defines which resistor is used. Can be any value of GPIO_Config_PuPd
  uint8_t AlternateFunction;    // Sets the alternate funciton of the pin (0..15)
}GPIO_ConfigTypeDef;

/*
 * GPIO driver handle
 */
typedef struct{
  GPIO_TypeDef *pGPIOx;         // Pointer to pGPIOx, where x(0..16). This type is defined in CMSIS device
  GPIO_ConfigTypeDef Config;    // Configuration struct used to configure the pin attributes
  GPIO_It_Mode InterruptMode;   // Defines the interruption configuration, can be of GPIO_It_Mode
}GPIO_DriverTypeDef;


/* Exported functions */

// Initialization function
DriverStatus GPIO_Init(GPIO_DriverTypeDef *pGPIODriver);

// Read/Write functions
uint8_t GPIO_Pin_Read(GPIO_TypeDef *pGPIOx, uint8_t PinNumber);
void GPIO_Pin_Write(GPIO_TypeDef *pGPIOx, uint8_t PinNumber, PinLogicalLevel LorH);
void GPIO_Pin_Toggle(GPIO_TypeDef *pGPIOx, uint8_t PinNumber);

// IRQ Handling and Configuration functions
void GPIO_IRQ_Handling(uint8_t PinNumber);
void GPIO_Callback_IRQTrigger(uint8_t PinNumber);
void GPIO_IRQ_Control(IRQn_Type IRQNumber, EnableDisable EnOrDi);
void GPIO_IRQ_PriorityConfig(IRQn_Type IRQNumber, uint32_t IRQPriority);

#endif // !__GPIO_H__
