#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f429zi.h"

typedef enum  { SPI_Type_FullDuplex,  SPI_Type_HalfDuplex, SPI_Type_RxOnly} SPI_Config_Type; 
typedef enum  { SPI_Mode_0, SPI_Mode_1, SPI_Mode_2, SPI_Mode_3 } SPI_Config_Mode;
typedef enum { SPI_Hierarchy_Slave, SPI_Hierarchy_Master }SPI_Config_Hierarchy;
typedef enum { SPI_BaudRate_div2, SPI_BaudRate_div4, SPI_BaudRate_div8, SPI_BaudRate_div16, SPI_BaudRate_div32, SPI_BaudRate_div64, SPI_BaudRate_div128, SPI_BaudRate_div256 } SPI_Config_BaudRate; 
typedef enum { SPI_FrameFormat_MSBFirst,  SPI_FrameFormat_LSBFirst } SPI_Config_FrameFormat;
typedef enum { SPI_SSM_Disable, SPI_SSM_Enable } SPI_Config_SSM;
typedef enum { SPI_DataFormat_8bit, SPI_DataFormat_16bit } SPI_Config_DataFormat;

typedef enum { SPI_It_Disable}SPI_It_Mode;

typedef struct{
   SPI_Config_Type Type;
  SPI_Config_Mode Mode;
  SPI_Config_Hierarchy Hierarchy;
  SPI_Config_BaudRate BaudRate;
  SPI_Config_FrameFormat FrameFormat;
  SPI_Config_SSM SSM;
  SPI_Config_DataFormat DataFormat;
}SPI_ConfigTypeDef;

typedef struct{
  SPI_TypeDef *pSPIx;
  SPI_ConfigTypeDef Config;
  SPI_It_Mode InterrupMode;
}SPI_DriverTypeDef;

DriverStatus SPI_Init(SPI_DriverTypeDef *pSPIDriver);

void SPI_SendData(SPI_DriverTypeDef *pSPIDriver, uint8_t *pTxBuffer, uint32_t Len);


#endif // !__SPI_H__
