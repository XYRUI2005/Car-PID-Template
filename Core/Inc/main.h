/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY1_Pin GPIO_PIN_13
#define KEY1_GPIO_Port GPIOC
#define KEY2_Pin GPIO_PIN_14
#define KEY2_GPIO_Port GPIOC
#define KEY3_Pin GPIO_PIN_15
#define KEY3_GPIO_Port GPIOC
#define R_LED_Pin GPIO_PIN_1
#define R_LED_GPIO_Port GPIOC
#define G_LED_Pin GPIO_PIN_2
#define G_LED_GPIO_Port GPIOC
#define B_LED_Pin GPIO_PIN_3
#define B_LED_GPIO_Port GPIOC
#define A2_Pin GPIO_PIN_0
#define A2_GPIO_Port GPIOA
#define B2_Pin GPIO_PIN_1
#define B2_GPIO_Port GPIOA
#define IN3_Pin GPIO_PIN_6
#define IN3_GPIO_Port GPIOA
#define IN4_Pin GPIO_PIN_7
#define IN4_GPIO_Port GPIOA
#define OLED_I2C_SCL_Pin GPIO_PIN_4
#define OLED_I2C_SCL_GPIO_Port GPIOC
#define OLED_I2C_SDA_Pin GPIO_PIN_5
#define OLED_I2C_SDA_GPIO_Port GPIOC
#define HW1_Pin GPIO_PIN_6
#define HW1_GPIO_Port GPIOC
#define HW2_Pin GPIO_PIN_7
#define HW2_GPIO_Port GPIOC
#define HW3_Pin GPIO_PIN_8
#define HW3_GPIO_Port GPIOC
#define HW4_Pin GPIO_PIN_9
#define HW4_GPIO_Port GPIOC
#define MPU6050_I2C_SCL_Pin GPIO_PIN_11
#define MPU6050_I2C_SCL_GPIO_Port GPIOA
#define MPU6050_I2C_SDA_Pin GPIO_PIN_12
#define MPU6050_I2C_SDA_GPIO_Port GPIOA
#define IN1_Pin GPIO_PIN_15
#define IN1_GPIO_Port GPIOA
#define HW5_Pin GPIO_PIN_10
#define HW5_GPIO_Port GPIOC
#define HW6_Pin GPIO_PIN_11
#define HW6_GPIO_Port GPIOC
#define HW7_Pin GPIO_PIN_12
#define HW7_GPIO_Port GPIOC
#define HW8_Pin GPIO_PIN_2
#define HW8_GPIO_Port GPIOD
#define IN2_Pin GPIO_PIN_3
#define IN2_GPIO_Port GPIOB
#define A1_Pin GPIO_PIN_6
#define A1_GPIO_Port GPIOB
#define B1_Pin GPIO_PIN_7
#define B1_GPIO_Port GPIOB
#define MPU6050_I2C_SCLB8_Pin GPIO_PIN_8
#define MPU6050_I2C_SCLB8_GPIO_Port GPIOB
#define MPU6050_I2C_SDAB9_Pin GPIO_PIN_9
#define MPU6050_I2C_SDAB9_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
