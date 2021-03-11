/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal.h"

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
#define WIFI_EN_Pin GPIO_PIN_0
#define WIFI_EN_GPIO_Port GPIOA
#define TOUCH_IRQ_Pin GPIO_PIN_6
#define TOUCH_IRQ_GPIO_Port GPIOH
#define TOUCH_NRST_Pin GPIO_PIN_9
#define TOUCH_NRST_GPIO_Port GPIOH
#define PWM_GLED_Pin GPIO_PIN_12
#define PWM_GLED_GPIO_Port GPIOD
#define PWM_RLED_Pin GPIO_PIN_13
#define PWM_RLED_GPIO_Port GPIOD
#define STATUS_LED_Pin GPIO_PIN_2
#define STATUS_LED_GPIO_Port GPIOG
#define OTG_FS_VUSB_NEN_Pin GPIO_PIN_8
#define OTG_FS_VUSB_NEN_GPIO_Port GPIOA
#define DISPLAY_EN_Pin GPIO_PIN_1
#define DISPLAY_EN_GPIO_Port GPIOI
#define PWM_DISPLAY_Pin GPIO_PIN_15
#define PWM_DISPLAY_GPIO_Port GPIOA
#define HEX_8_Pin GPIO_PIN_3
#define HEX_8_GPIO_Port GPIOD
#define HEX_2_Pin GPIO_PIN_4
#define HEX_2_GPIO_Port GPIOD
#define HEX_4_Pin GPIO_PIN_5
#define HEX_4_GPIO_Port GPIOD
#define HEX_1_Pin GPIO_PIN_7
#define HEX_1_GPIO_Port GPIOD
#define ACEL_NEN_Pin GPIO_PIN_9
#define ACEL_NEN_GPIO_Port GPIOG
#define PWR_5V_EN_Pin GPIO_PIN_13
#define PWR_5V_EN_GPIO_Port GPIOG
#define AUDIO_EN_Pin GPIO_PIN_7
#define AUDIO_EN_GPIO_Port GPIOI
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
