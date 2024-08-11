#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f429zi.h"

typedef enum {GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG} GPIO_PIN_MODE;
typedef enum {GPIO_OPTYPE_PUSHPULL, GPIO_OPTYPE_OPENDRAIN} GPIO_PIN_OPTYPE;
typedef enum {GPIO_SPEED_LOW, GPIO_SPEED_MEDIUM, GPIO_SPEED_HIGH, GPIO_SPEED_VERYHIGH} GPIO_PIN_SPEED;
typedef enum {GPIO_PUPD_NONE, GPIO_PUPD_PULLUP, GPIO_PUPD_PULLDOWN} GPIO_PIN_PUPD;
typedef enum {GPIO_IT_DISABLE, GPIO_IT_FALL, GPIO_IT_RISE, GPIO_IT_RISEFALL}GPIO_IT_MODE;

typedef struct{
  uint8_t Number;
  GPIO_PIN_MODE Mode;
  GPIO_PIN_OPTYPE OutputType;
  GPIO_PIN_SPEED Speed;
  GPIO_PIN_PUPD PullUpDown;
  uint8_t AlternateFunction;
}GPIO_PinConfig;


typedef struct{
  GPIO_TypeDef *pGPIOx;
  GPIO_PinConfig PinConfig;
  GPIO_IT_MODE InterruptMode;
}GPIO_ConfigTypeDef;


// functions

DriverStatus GPIO_Init(GPIO_ConfigTypeDef *pgpioconfig);


uint8_t GPIO_Pin_Read(GPIO_TypeDef *pGPIOx, uint8_t PinNumber);
void GPIO_Pin_Write(GPIO_TypeDef *pGPIOx, uint8_t PinNumber, PinLogicalLevel LorH);


void GPIO_IRQ_Control(uint8_t IRQNumber, EnableDisable EnOrDi);
void GPIO_IRQ_PriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

#endif // !__GPIO_H__
