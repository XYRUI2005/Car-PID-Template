#ifndef __LINE_H__
#define __LINE_H__

#include "main.h"

#define HW1   HAL_GPIO_ReadPin(HW1_GPIO_Port,HW1_Pin) //读取红外对管连接的GPIO电平
#define HW2   HAL_GPIO_ReadPin(HW2_GPIO_Port,HW2_Pin)
#define HW3   HAL_GPIO_ReadPin(HW3_GPIO_Port,HW3_Pin)
#define HW4   HAL_GPIO_ReadPin(HW4_GPIO_Port,HW4_Pin)
#define HW5   HAL_GPIO_ReadPin(HW5_GPIO_Port,HW5_Pin) 
#define HW6   HAL_GPIO_ReadPin(HW6_GPIO_Port,HW6_Pin)
#define HW7   HAL_GPIO_ReadPin(HW7_GPIO_Port,HW7_Pin)
#define HW8   HAL_GPIO_ReadPin(HW8_GPIO_Port,HW8_Pin)

extern uint8_t Line_Data;
extern int32_t Line_Num;

void LINE_Scan(void);

#endif
