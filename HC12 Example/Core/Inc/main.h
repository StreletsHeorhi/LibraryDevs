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
#define SYNC_Pin GPIO_PIN_13
#define SYNC_GPIO_Port GPIOC
#define B_Pin GPIO_PIN_14
#define B_GPIO_Port GPIOC
#define R_Pin GPIO_PIN_15
#define R_GPIO_Port GPIOC
#define ID0_Pin GPIO_PIN_0
#define ID0_GPIO_Port GPIOB
#define ID1_Pin GPIO_PIN_1
#define ID1_GPIO_Port GPIOB
#define ID2_Pin GPIO_PIN_2
#define ID2_GPIO_Port GPIOB
#define ID3_Pin GPIO_PIN_10
#define ID3_GPIO_Port GPIOB
#define ID4_Pin GPIO_PIN_11
#define ID4_GPIO_Port GPIOB
#define HC12_Mode_Pin GPIO_PIN_4
#define HC12_Mode_GPIO_Port GPIOB
#define USART1_DE_Pin GPIO_PIN_5
#define USART1_DE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
