/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MainSupport.h"
#include "TD_SDRAM.h"
//#include "app_touchgfx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for DefaultTouchGFx */
osThreadId_t DefaultTouchGFxHandle;
const osThreadAttr_t DefaultTouchGFx_attributes = {
  .name = "DefaultTouchGFx",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 8192 * 4
};
/* Definitions for GeneralHardware */
osThreadId_t GeneralHardwareHandle;
const osThreadAttr_t GeneralHardware_attributes = {
  .name = "GeneralHardware",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 512 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void defaultTouchGFxTask(void *argument);
void generalHardwareTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of DefaultTouchGFx */
  DefaultTouchGFxHandle = osThreadNew(defaultTouchGFxTask, NULL, &DefaultTouchGFx_attributes);

  /* creation of GeneralHardware */
  GeneralHardwareHandle = osThreadNew(generalHardwareTask, NULL, &GeneralHardware_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_defaultTouchGFxTask */
/**
  * @brief  Function implementing the DefaultTouchGFx thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_defaultTouchGFxTask */
void defaultTouchGFxTask(void *argument)
{
  /* USER CODE BEGIN defaultTouchGFxTask */
	// INIT TOUCHGFX - TOUCHGFX REQUIRES SDRAM INIT TO WORK
	SDRAM_InitSequence();
	MX_TouchGFX_Process();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END defaultTouchGFxTask */
}

/* USER CODE BEGIN Header_generalHardwareTask */
/**
* @brief Function implementing the GeneralHardware thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_generalHardwareTask */
void generalHardwareTask(void *argument)
{
  /* USER CODE BEGIN generalHardwareTask */
	main_Init();
  /* Infinite loop */
  for(;;)
  {
	main_WhileLoop();
    osDelay(1);
  }
  /* USER CODE END generalHardwareTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
