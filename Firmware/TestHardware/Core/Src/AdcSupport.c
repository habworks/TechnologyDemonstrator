/*
 * AdcSupport.c
 *
 *  Created on: Jan 17, 2021
 *      Author: IMR HabPC
 */

#include "AdcSupport.h"
#include "SupportIO.h"
#include "adc.h"

extern uint16_t ADC_DMA_Buffer[ADC_DMA_SIZE];

// This is the IRQ Callback function for the ADC.  It is defined as weak in HAL Lib.
/* The ADC_DMA_Buffer contains the values of the conversions.  The conversions are listed
 * in ranked order.  For example if they are 3 conversions to be made then the buffer holds
 *   ADC_DMA_Buffer[0] Conversation 1 of ADC Chx
 *   ADC_DMA_Buffer[1] Conversation 1 of ADC Chy
 *   ADC_DMA_Buffer[2] Conversation 1 of ADC Chz
 *   ADC_DMA_Buffer[3] Conversation 2 of ADC Chx
 *   ADC_DMA_Buffer[4] Conversation 2 of ADC Chy
 *   ADC_DMA_Buffer[5] Conversation 2 of ADC Chz
 * NOTE ON TIMER:
 * The ADC DMA is triggered by Timer1
 * Timer 1 is setup at the frequency for ADC conversations to begin (in this case 1ms)
 * Timer 1 is set up as Output Compare No Output
 * NOTE ON ADC:
 * The ADC channels to be used must be selected and must be ranked
 * Within the CubeMX ADC parameter settings you must:
 * Set "Number Of Conversions" = to number of ADC channels - this then creates a rank 1 to x for x number of channels
 * You must link each Rank to a channel
 * Set "External Trigger Conversion Source" = Timer 1 Trigger Out event
 * Set "External Trigger Conversion Edge" = Trigger Detection on rising edge
 * For the timer to trigger the ADC conversion event you must set:
 * "Scan Conversion Mode" = Enable
 * "Continuous Conversion Mode" = Disable
 * "Discontinuous Conversion Mode" = Disable
 * "DMA Continuous Request" = Enable
 * In DMA Settings Add the ADC - select it and set mode to circular
 * See... https://www.youtube.com/watch?v=0jKtgP4OYvU - see how to add temperature channel
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

	uint32_t ADC_UiReadSum = 0;
	uint32_t ADC_BatReadSum = 0;
	uint32_t ADC_TempSensor = 0;
	uint16_t *DMA_Buffer = ADC_DMA_Buffer;

	for (uint8_t Index = 0; Index < (ADC_DMA_SIZE/NUMBER_OF_ADC_CHANNELS); Index++)
	{
		ADC_UiReadSum += *DMA_Buffer;
		DMA_Buffer++;
		ADC_BatReadSum += *DMA_Buffer;
		DMA_Buffer++;
		ADC_TempSensor += *DMA_Buffer;
		DMA_Buffer++;
	}
	BoardStatus.ADC_UiReadSum = ADC_UiReadSum;
	BoardStatus.ADC_BatReadSum = ADC_BatReadSum;

	// Calculate temp sensor from chip - maybe you can add and offset to this and read room temperature - otherwise not accurate
    float V25 = 0.76;
    float Avg_Slope = 0.0025;
    float AvgFilterTempSensor = (float)(ADC_TempSensor / ADC_SAMPLES_PER_ACQUISION);
    float VSENSE = (float)(3.3 * AvgFilterTempSensor / 4095);
    static float TempSensor;
	TempSensor = ((V25 - VSENSE) / Avg_Slope) + 25;
	__NOP();

}

uint16_t ADC_FullScale(void)
{
	switch (hadc1.Init.Resolution)
	{
	case ADC_RESOLUTION_12B:
		return(4095);
		break;

	case ADC_RESOLUTION_10B:
		return(1023);
		break;

	case ADC_RESOLUTION_8B:
		return(255);
		break;

	case ADC_RESOLUTION_6B:
		return(63);
		break;

	default:
		while(1);
	}
}
