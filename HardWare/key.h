#ifndef __KEY_H__
#define __KEY_H__

#include "gpio.h"

#define KEY1   HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)
#define KEY2   HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)
#define KEY3   HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin)

#define KEY_NONE_PRESSED 0
#define KEY1_PRESSED 1
#define KEY2_PRESSED 2
#define KEY3_PRESSED 3

#define NONE_PRESSED 0
#define PRESSED 1

extern uint8_t KEY_Num,KEY_Flag;//按键号、按键状态标志位

void KEY_Scan(void);
uint8_t KEY_Read(void);

#endif
