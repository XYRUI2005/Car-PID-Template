#ifndef __LED_H__
#define __LED_H__

#include "stm32f1xx.h"
#include "gpio.h"
#include "main.h"

#define RedLED_Toggle() HAL_GPIO_TogglePin(R_LED_GPIO_Port,R_LED_Pin)
#define GreenLED_Toggle() HAL_GPIO_TogglePin(G_LED_GPIO_Port,G_LED_Pin)
#define YellowLED_Toggle() HAL_GPIO_TogglePin(B_LED_GPIO_Port,B_LED_Pin)

#define RedLED_ON() HAL_GPIO_WritePin(R_LED_GPIO_Port,R_LED_Pin,GPIO_PIN_RESET)
#define GreenLED_ON() HAL_GPIO_WritePin(G_LED_GPIO_Port,G_LED_Pin,GPIO_PIN_RESET)
#define YellowLED_ON() HAL_GPIO_WritePin(B_LED_GPIO_Port,B_LED_Pin,GPIO_PIN_RESET)

#define RedLED_OFF() HAL_GPIO_WritePin(R_LED_GPIO_Port,R_LED_Pin,GPIO_PIN_SET)
#define GreenLED_OFF() HAL_GPIO_WritePin(G_LED_GPIO_Port,G_LED_Pin,GPIO_PIN_SET)
#define YellowLED_OFF() HAL_GPIO_WritePin(B_LED_GPIO_Port,B_LED_Pin,GPIO_PIN_SET)

#endif
