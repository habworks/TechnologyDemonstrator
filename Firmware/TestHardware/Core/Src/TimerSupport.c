/*
 * Timer.c
 *
 *  Created on: Jan 15, 2021
 *      Author: IMR HabPC
 */

#include <HAB_WAV_AUDIO_TASKS.H>
#include "TimerSupport.h"


/* This function is the part of the Sys Tick timer.  It is defined weak else where.
 * FOR SYSTEM TIMER TO WORK YOU MUST INCLUDE ITS BASIC DUTIES AND INCLUDE:
 *    uwTick += uwTickFreq;
 * HAL_Delay() will need this
 */
void HAL_IncTick(void)
{
	uwTick += uwTickFreq;
	BoardStatus.GreenLedMsCounter++;
	BoardStatus.RedLedMsCounter++;
	BoardStatus.StatusLedMsCounter++;
	BoardStatus.MiliSecondCounter++;
}

void delayMiliSecond(uint32_t DelayTime)
{
	BoardStatus.MiliSecondCounter = 0;
	while (BoardStatus.MiliSecondCounter < DelayTime);
}


bool_t init_AudioTimer(uint32_t InteruptFrequency)
{
	uint16_t TicksToIrq;
	uint32_t TimerFrequency;
	double TimperPeriod;

	TimerFrequency = (uint32_t)(FULL_TIMER_CLOCK_RATE / (htim3.Init.Prescaler + 1));
	TimperPeriod = (double)(1.0 / TimerFrequency);
	TicksToIrq = (uint16_t)(INTEGER_ROUNDING + (double)((1.0 / InteruptFrequency)) / TimperPeriod);
	HAL_TIM_Base_Stop_IT(&htim3);
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 10-1;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = TicksToIrq-1;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		return(FALSE);
	}
	else
		return(TRUE);
}


// Generic IRQ call back handler for all timers
// Defined weak so it is redefine here
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM3)
		audioTimerIrqHandler();
}

