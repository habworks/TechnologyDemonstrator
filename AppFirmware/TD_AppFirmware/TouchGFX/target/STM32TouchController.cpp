/**
  ******************************************************************************
  * File Name          : STM32TouchController.cpp
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

/* USER CODE BEGIN STM32TouchController */

#include <STM32TouchController.hpp>

// HAB ADD THESE 3
#include <stm32746g_discovery_ts.h>
extern I2C_HandleTypeDef hi2c3; // HAB THIS IS I2C CONNECTED TO TOUCH GFX

void STM32TouchController::init()
{
	// HAB ADD
	BSP_TS_Init(480, 272); // HAB TODO FIX MAGIC NUMBERS
    /**
     * Initialize touch controller and driver
     *
     */
}

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    /**
     * By default sampleTouch returns false,
     * return true if a touch has been detected, otherwise false.
     *
     * Coordinates are passed to the caller by reference by x and y.
     *
     * This function is called by the TouchGFX framework.
     * By default sampleTouch is called every tick, this can be adjusted by HAL::setTouchSampleRate(int8_t);
     *
     */
	// HAB ADD - BELOW - RETURN FALSE WAS ALREADY THERE
//	if (tsDriver)
//	    {
//	        if (tsDriver->DetectTouch(TS_I2C_ADDRESS))
//	        {
//	            /* Get each touch coordinates */
//	            tsDriver->GetXY(TS_I2C_ADDRESS, (uint16_t*)&y, (uint16_t*)&x);
//	            return true;
//	        }
//	    }
//    return false;

	TS_StateTypeDef state = {0};
	BSP_TS_GetState(&state);
	if (state.touchDetected)
	{
		x = state.touchX[0];
		y = state.touchY[0];
		return(true);
	}
	return(false);
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
