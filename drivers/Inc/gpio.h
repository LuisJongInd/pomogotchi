#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f429zi.h"

typedef enum {GPIO_Mode_Input, GPIO_Mode_Output, GPIO_Mode_AlternateFunction, GPIO_MODE_Analog} GPIO_Config_Mode;
typedef enum {GPIO_OpType_PushPull, GPIO_OpTypE_OpenDrain} GPIO_Config_OpType;
typedef enum {GPIO_Speed_Low, GPIO_Speed_Medium, GPIO_Speed_High, GPIO_Speed_VeryHigh} GPIO_Config_Speed;
typedef enum {GPIO_PuPd_None, GPIO_PuPd_PullUp, GPIO_PuPd_PullDown} GPIO_Config_PuPd;
typedef enum {GPIO_It_Disable, GPIO_It_Fall, GPIO_It_Rise, GPIO_It_RiseFall}GPIO_It_Mode;

typedef struct{
  uint8_t Number;
  GPIO_Config_Mode Mode;
  GPIO_Config_OpType OutputType;
  GPIO_Config_Speed Speed;
  GPIO_Config_PuPd PullUpDown;
  uint8_t AlternateFunction;
}GPIO_ConfigTypeDef;


typedef struct{
  GPIO_TypeDef *pGPIOx;
  GPIO_ConfigTypeDef Config;
  GPIO_It_Mode InterruptMode;
}GPIO_DriverTypeDef;


// functions

DriverStatus GPIO_Init(GPIO_DriverTypeDef *pGPIODriver);


uint8_t GPIO_Pin_Read(GPIO_TypeDef *pGPIOx, uint8_t PinNumber);
void GPIO_Pin_Write(GPIO_TypeDef *pGPIOx, uint8_t PinNumber, PinLogicalLevel LorH);
void GPIO_Pin_Toggle(GPIO_TypeDef *pGPIOx, uint8_t PinNumber);

void GPIO_IRQ_Handling(uint8_t PinNumber);
void GPIO_Callback_IRQTrigger(uint8_t PinNumber);

void GPIO_IRQ_Control(uint8_t IRQNumber, EnableDisable EnOrDi);
void GPIO_IRQ_PriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

#endif // !__GPIO_H__
