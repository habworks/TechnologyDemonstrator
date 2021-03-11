/** ****************************************************************************************************
 * @file 			AnalogDigitalTaskSupport.c
 * @brief			This is the Source file used to support tasks associated with the Analog and Digital Screen
 * ****************************************************************************************************
 * @author			Hab S. Collector \n
 * Last Edited By:	Hab S. Collector \n
 *
 * @date			3/7/21 \n
 * @version       	See Main_Support.h: FIRMWARE_REV_MAJOR, FIRMWARE_REV_MINOR
 *
 * @param Development_Environment \n
 * Hardware:		STM32F746IGT6 \n
 * IDE:             STM32CubeIDE, VER 1.5.1 \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            This file provides a functions that support the Analog and Digital task analogDigitalTask()
 * 					The task functions are done here to minimize how much of the generated code has to be edited
 *
 *
 * 					This is an embedded application
 *		            It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *			          IMR Technology Demonstrator Schematic: IMR002S REV 1
 *				      IMR Technology Demonstrator Schematic: IMR002S REV 1
 *
 * @copyright		IMR Engineering
 * **************************************************************************************************** */

#include "AnalogDigitalTaskSupport.h"
#include "TD_SupportIO.h"
#include "string.h"
#include "cmsis_os2.h"

// GLOBALS
extern osSemaphoreId_t BinarySemAnalogDigitalHandle;
uint16_t ADC_DMA_Buffer[ADC_DMA_SIZE];
Type_AnalogDigitalSupportTask AnalogDigitalSupport;


/*******************************************************************************************************
* @brief This function will perform the initialization routines necessary to support task: analogDigitalTask()
* All items that need to be processed ONE TIME before the endless while / for loop of the task is called.
* This function should be placed within task before the endless for loop:
*
* @date				3/7/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Avoids excessive edit of main.c when using STM32CubeMX
*
* @param void
* @return void
*
* STEP 1: Clear the DMA memory buffer and start DMA for ADC
* STEP 2: Start the Timer associated with the DMA channel
****************************************************************************************************** */
void analogDigitalTask_Init(void)
{
	// STEP 1: Clear the DMA memory buffer and start DMA for ADC
	memset(ADC_DMA_Buffer, 0, ADC_DMA_SIZE * sizeof(uint16_t));
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_DMA_Buffer, ADC_DMA_SIZE);

	// STEP 2: Start the Timer associated with the DMA channel
	HAL_TIM_Base_Start_IT(&htim1);

} // END OF FUNCTION analogDigitaTask_Init



/*******************************************************************************************************
* @brief This function will perform the repeated routines necessary to support task: analogDigitalTask()
* All items that need to be processed ENDLESSLY after the init function of the task has been called.
* This function should be placed within task endless for loop:
*
* @date				3/7/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Avoids excessive edit of main.c when using STM32CubeMX
*
* @param void
* @return void
*
* STEP 1:
* STEP 2:
* STEP 3:
****************************************************************************************************** */
void analogDigitalTask_ForLoop(void)
{
	uint8_t HexSwitchValue;
	static uint8_t InFunctionTicks = 0;
	static uint8_t DataPoints = 0;

	if (TechDemoStatus.ActiveScreen ==  AD_SCREEN)
	{
		if (InFunctionTicks > 100)
		{
			DataPoints++;
			if (DataPoints > 100)
			{
				TechDemoStatus.AnalogDigitalScreen.RefreshGraph = TRUE;
				DataPoints = 0;
			}
			avgAndConvertAnalogChannels(&AnalogDigitalSupport);
			HexSwitchValue = getUserInterfaceHexSwitchValue();
			TechDemoStatus.AnalogDigitalScreen.HexSwitchValue = HexSwitchValue;
			convertHexSwitchValueToCharacter(HexSwitchValue, &TechDemoStatus.AnalogDigitalScreen.HexSwitchChar);
			osSemaphoreRelease(BinarySemAnalogDigitalHandle);
			InFunctionTicks = 0;
		}
		else
		{
			InFunctionTicks++;
			TechDemoStatus.AnalogDigitalScreen.RefreshGraph = FALSE;
		}
	}
} // END OF FUNCTION analogDigitalTask_ForLoop




/*******************************************************************************************************
* @brief Calculates the ADC reading for all channels and stores to BoardStatus
*
* @date				3/7/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note O
*
* @param AnalogDigital struct
* @return void
*
* STEP 1: Calculate Battery Voltage
* STEP 2: Calculate User Voltage
****************************************************************************************************** */
void avgAndConvertAnalogChannels(const Type_AnalogDigitalSupportTask *AnalogDigitalSupportTask)
{
	float ADC_AverageValue;
	uint16_t FullScale_ADC;

	ADC_FullScale(&hadc1, &FullScale_ADC);

	// STEP 1: Calculate Battery Voltage
	ADC_AverageValue = (float)(AnalogDigitalSupportTask->ADC_UserInterfaceSum / ADC_SAMPLES_PER_ACQUISION);
	TechDemoStatus.AnalogDigitalScreen.UserInterfaceVoltage = (float)(ADC_AverageValue * ADC_VREF / FullScale_ADC);

	// STEP 2: Calculate User Voltage
	ADC_AverageValue = (float)(AnalogDigitalSupportTask->ADC_BatReadSum / ADC_SAMPLES_PER_ACQUISION);
	TechDemoStatus.AnalogDigitalScreen.BatteryVoltage = (float)(ADC_AverageValue * ADC_VREF / FullScale_ADC);

} // END OF avgAndConvertAnalogChannels



/*******************************************************************************************************
* @brief Returns the full scale ADC conversion value - based on the ADC init
*
* @date				3/7/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Only 6, 8, 10 and 12 bit possible
*
* @param Handle to ADC
* @return Full scale conversion value
*
* STEP 1: Return ADC full scale
****************************************************************************************************** */
bool_t ADC_FullScale(ADC_HandleTypeDef *ADC_Handle, uint16_t *FullScale)
{
	// STEP 1: Return ADC full scale
	switch (ADC_Handle->Init.Resolution)
	{
	case ADC_RESOLUTION_12B:
		*FullScale = ADC_12B_FULL_SCALE;
		break;

	case ADC_RESOLUTION_10B:
		*FullScale = ADC_10B_FULL_SCALE;
		break;

	case ADC_RESOLUTION_8B:
		*FullScale = ADC_8B_FULL_SCALE;
		break;

	case ADC_RESOLUTION_6B:
		*FullScale = ADC_6B_FULL_SCALE;
		break;

	default:
		return(FALSE);
		break;
	}
	return(TRUE);
} // END OF ADC_FullScale



/*******************************************************************************************************
* @brief Returns the reading of the Hex switch.  Hex switch is comprised of 4 bits with a value 0 - 0x0F
*
* @date				3/7/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Hex switch PN: 220ADC16
* 		Low indicates that bit weight is present
*
* @param void
* @return a value of 0 - 15 for the Hex Switch setting
*
* STEP 1: Read and calculate hex switch value
****************************************************************************************************** */
uint8_t getUserInterfaceHexSwitchValue(void)
{
	// STEP 1: Read and calculate hex switch value
	uint8_t HexSum = 0;
	if (HEX_1_STATUS() == GPIO_PIN_RESET)
		HexSum++;
	if (HEX_2_STATUS() == GPIO_PIN_RESET)
		HexSum += 2;
	if (HEX_4_STATUS() == GPIO_PIN_RESET)
		HexSum += 4;
	if (HEX_8_STATUS() == GPIO_PIN_RESET)
		HexSum += 8;
	return(HexSum);
} // END OF getUserInterfaceHexSwitchValue


/*******************************************************************************************************
* @brief Converts the Hex Switch Value 0-0x0F to a character 0-9, A-F
*
* @date				3/7/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Hex switch PN: 220ADC16
*
* @param Hex Switch Value, Pointer to Hex Switch Char
* @return True on successful conversion
*
* STEP 1: Convert numeric value 0x00 - 0x0F to Char 0-9, A-F
****************************************************************************************************** */
bool_t convertHexSwitchValueToCharacter(uint8_t HexSwitchValue, char *HexSwitchChar)
{
	// STEP 1: Convert numeric value 0x00 - 0x0F to Char 0-9, A-F
	if (HexSwitchValue > 0x0F)
		return(FALSE);
	if (HexSwitchValue <= 0x09)
	{
		*HexSwitchChar = ASCII_DIGIT_CHAR_OFFSET + HexSwitchValue;
	}
	else
	{
		uint8_t OffsetValue = HexSwitchValue - 0x0A;
		*HexSwitchChar = ASCII_CAP_LETTER_CHAR_OFFSET + OffsetValue;
	}
	return(TRUE);
} // END OF convertHexSwitchValueToCharacter



/*******************************************************************************************************
* @brief This is the IRQ Callback function for the ADC.  It is defined as weak in HAL Lib
*
* @date				3/7/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Avoids excessive edit of main.c when using STM32CubeMX
* This is the IRQ Callback function for the ADC.  It is defined as weak in HAL Lib.
* The ADC_DMA_Buffer contains the values of the conversions.  The conversions are listed
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
*
* @param void
* @return void
*
* STEP 1:
* STEP 2:
* STEP 3:
****************************************************************************************************** */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* ADC_Handle)
{
	if (ADC_Handle == &hadc1)
	{
		uint32_t ADC_UserInterfaceSum = 0;
		uint32_t ADC_BatReadSum = 0;
		uint32_t ADC_TempSensorSum = 0;
		uint16_t *DMA_Buffer = ADC_DMA_Buffer;

		for (uint8_t Index = 0; Index < (ADC_DMA_SIZE/NUMBER_OF_ADC_CHANNELS); Index++)
		{
			ADC_UserInterfaceSum += *DMA_Buffer;
			DMA_Buffer++;
			ADC_BatReadSum += *DMA_Buffer;
			DMA_Buffer++;
			ADC_TempSensorSum += *DMA_Buffer;
			DMA_Buffer++;
		}
		AnalogDigitalSupport.ADC_UserInterfaceSum = ADC_UserInterfaceSum;
		AnalogDigitalSupport.ADC_BatReadSum = ADC_BatReadSum;
		AnalogDigitalSupport.ADC_TempSensorSum = ADC_TempSensorSum;
	}

	// Calculate temp sensor from chip - maybe you can add and offset to this and read room temperature - otherwise not accurate
//    float V25 = 0.76;
//    float Avg_Slope = 0.0025;
//    float AvgFilterTempSensor = (float)(ADC_TempSensor / ADC_SAMPLES_PER_ACQUISION);
//    float VSENSE = (float)(3.3 * AvgFilterTempSensor / 4095);
//    static float TempSensor;
//	TempSensor = ((V25 - VSENSE) / Avg_Slope) + 25;
//	__NOP();
} // END OF HAL_ADC_ConvCpltCallback (REDEFINITION)
