/*
 * WavAudioInterface.c
 *
 *  Created on: Apr 3, 2021
 *      Author: IMR HabPC
 */

#include "MainSupport.h"
#include "HAB_WAV_AUDIO_TASKS.H"
#include "AdcSupport.h"
#include "TimerSupport.h"
#include "SupportIO.h"
#include "dac.h"
#include "stdbool.h"
#include "stdlib.h"


// REDEFINITION OF FUNCTIONS DECLARED WEAK USED IN HAB WAV AUDIO
void * memoryAllocate(size_t NumberOfElements, size_t SizeOfElement)
{
	void *PointerToMemory;
	PointerToMemory = calloc(NumberOfElements, SizeOfElement);
	return(PointerToMemory);
}

void memoryFree(void *PointerToMemory)
{
	free(PointerToMemory);
}// END OF memoryFree

bool init_WavAudioTimer(uint32_t InteruptFrequency)
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
} // END OF init_WavAudioTimer

void disableAudioOutput(void)
{
	writeDAC_LeftChannel(0);
	AUDIO_AMP_OFF();
} // END OF disableAudioOutput


void enableAudioOutput(void)
{
	writeDAC_LeftChannel(0);
	AUDIO_AMP_ON();
} // END OF enableAudioOutput


void writeDAC_LeftChannel(uint16_t DAC_Value)
{
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, DAC_Value);
} // END OF writeDAC_LeftChannel


void writeDAC_RightChannel(uint16_t DAC_Value)
{
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, DAC_Value);
}  // END OF writeDAC_RightChannel


void startAudioTimer(void)
{
	HAL_TIM_Base_Start_IT(&htim3);
} // END OF startAudioTimer


void stopAudioTimer(void)
{
	HAL_TIM_Base_Stop_IT(&htim3);
} // END OF stopAudioTimer


void delayInMiliseonds(uint32_t DelayTime)
{
	delayMiliSecond(DelayTime);
} // END OF delayInMiliseonds
