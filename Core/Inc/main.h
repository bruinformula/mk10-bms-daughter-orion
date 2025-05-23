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
#include "stm32l4xx_hal.h"

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
#define CS2_Pin GPIO_PIN_14
#define CS2_GPIO_Port GPIOC
#define CS3_Pin GPIO_PIN_15
#define CS3_GPIO_Port GPIOC
#define TS13_Pin GPIO_PIN_0
#define TS13_GPIO_Port GPIOA
#define TS14_Pin GPIO_PIN_1
#define TS14_GPIO_Port GPIOA
#define TS15_Pin GPIO_PIN_2
#define TS15_GPIO_Port GPIOA
#define TS16_Pin GPIO_PIN_3
#define TS16_GPIO_Port GPIOA
#define TS17_Pin GPIO_PIN_4
#define TS17_GPIO_Port GPIOA
#define TS18_Pin GPIO_PIN_5
#define TS18_GPIO_Port GPIOA
#define TS19_Pin GPIO_PIN_6
#define TS19_GPIO_Port GPIOA
#define TS20_Pin GPIO_PIN_7
#define TS20_GPIO_Port GPIOA
#define CS1_Pin GPIO_PIN_7
#define CS1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
