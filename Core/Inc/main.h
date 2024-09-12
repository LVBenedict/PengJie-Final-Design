/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#define Motor_AIN2_Pin GPIO_PIN_5
#define Motor_AIN2_GPIO_Port GPIOA
#define Motor_BIN1_Pin GPIO_PIN_6
#define Motor_BIN1_GPIO_Port GPIOA
#define Motor_BIN2_Pin GPIO_PIN_7
#define Motor_BIN2_GPIO_Port GPIOA
#define GRAY_SERIAL_CLK_Pin GPIO_PIN_10
#define GRAY_SERIAL_CLK_GPIO_Port GPIOB
#define GRAY_SERIAL_DAT_Pin GPIO_PIN_11
#define GRAY_SERIAL_DAT_GPIO_Port GPIOB
#define Trig_Left_Pin GPIO_PIN_12
#define Trig_Left_GPIO_Port GPIOB
#define Echo_Left_Pin GPIO_PIN_13
#define Echo_Left_GPIO_Port GPIOB
#define Button4_Pin GPIO_PIN_14
#define Button4_GPIO_Port GPIOB
#define Button3_Pin GPIO_PIN_15
#define Button3_GPIO_Port GPIOB
#define Button1_Pin GPIO_PIN_8
#define Button1_GPIO_Port GPIOA
#define Trig_Right_Pin GPIO_PIN_11
#define Trig_Right_GPIO_Port GPIOA
#define Echo_Right_Pin GPIO_PIN_12
#define Echo_Right_GPIO_Port GPIOA
#define Motor_AIN1_Pin GPIO_PIN_4
#define Motor_AIN1_GPIO_Port GPIOB
#define Button2_Pin GPIO_PIN_5
#define Button2_GPIO_Port GPIOB
#define Trig_Middle_Pin GPIO_PIN_6
#define Trig_Middle_GPIO_Port GPIOB
#define Echo_Middle_Pin GPIO_PIN_7
#define Echo_Middle_GPIO_Port GPIOB
#define OLED_SCLK_Pin GPIO_PIN_8
#define OLED_SCLK_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_9
#define OLED_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
