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
#include "TD_SupportIO.h"
#include "app_touchgfx.h"
#include "AnalogDigitalTaskSupport.h"
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
/* Definitions for AnalogDigital */
osThreadId_t AnalogDigitalHandle;
const osThreadAttr_t AnalogDigital_attributes = {
  .name = "AnalogDigital",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for PWM_Acceleromet */
osThreadId_t PWM_AccelerometHandle;
const osThreadAttr_t PWM_Acceleromet_attributes = {
  .name = "PWM_Acceleromet",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for BinarySemAnalogDigital */
osSemaphoreId_t BinarySemAnalogDigitalHandle;
const osSemaphoreAttr_t BinarySemAnalogDigital_attributes = {
  .name = "BinarySemAnalogDigital"
};
/* Definitions for BinarySemPWM_Accelerometer */
osSemaphoreId_t BinarySemPWM_AccelerometerHandle;
const osSemaphoreAttr_t BinarySemPWM_Accelerometer_attributes = {
  .name = "BinarySemPWM_Accelerometer"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void defaultTouchGFxTask(void *argument);
void generalHardwareTask(void *argument);
void analogDigitalTask(void *argument);
void PWM_AccelerometerTask(void *argument);

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

  /* Create the semaphores(s) */
  /* creation of BinarySemAnalogDigital */
  BinarySemAnalogDigitalHandle = osSemaphoreNew(1, 1, &BinarySemAnalogDigital_attributes);

  /* creation of BinarySemPWM_Accelerometer */
  BinarySemPWM_AccelerometerHandle = osSemaphoreNew(1, 1, &BinarySemPWM_Accelerometer_attributes);

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

  /* creation of AnalogDigital */
  AnalogDigitalHandle = osThreadNew(analogDigitalTask, NULL, &AnalogDigital_attributes);

  /* creation of PWM_Acceleromet */
  PWM_AccelerometHandle = osThreadNew(PWM_AccelerometerTask, NULL, &PWM_Acceleromet_attributes);

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
	TOUCH_RST_DISABLE();
	HAL_Delay(200);
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

/* USER CODE BEGIN Header_analogDigitalTask */
/**
* @brief Function implementing the AnalogDigital thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_analogDigitalTask */
void analogDigitalTask(void *argument)
{
  /* USER CODE BEGIN analogDigitalTask */
	analogDigitaTask_Init();
  /* Infinite loop */
  for(;;)
  {
	analogDigitalTask_ForLoop();
    osDelay(1);
  }
  /* USER CODE END analogDigitalTask */
}

/* USER CODE BEGIN Header_PWM_AccelerometerTask */
/**
* @brief Function implementing the PWM_Acceleromet thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PWM_AccelerometerTask */
void PWM_AccelerometerTask(void *argument)
{
  /* USER CODE BEGIN PWM_AccelerometerTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END PWM_AccelerometerTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
