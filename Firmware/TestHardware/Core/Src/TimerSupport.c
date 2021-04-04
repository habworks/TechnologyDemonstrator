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





// Generic IRQ call back handler for all timers
// Defined weak so it is redefine here
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM3)
		audioTimerIrqHandler();
}

