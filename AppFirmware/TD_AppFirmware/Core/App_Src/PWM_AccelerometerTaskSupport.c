/** ****************************************************************************************************
 * @file 			PWM_AccelerometerTaskSupport.c
 * @brief			This is the Source file used to support tasks associated with the PWM and Accelerometer Screen
 * ****************************************************************************************************
 * @author			Hab S. Collector \n
 * Last Edited By:	Hab S. Collector \n
 *
 * @date			3/8/21 \n
 * @version       	See Main_Support.h: FIRMWARE_REV_MAJOR, FIRMWARE_REV_MINOR
 *
 * @param Development_Environment \n
 * Hardware:		STM32F746IGT6 \n
 * IDE:             STM32CubeIDE, VER 1.5.1 \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            This file provides a functions that support both the PWM and Accelerometer
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

#include "PWM_AccelerometerTaskSupport.h"
#include "Driver_LSI3DH.h"
#include "cmsis_os2.h"
#include "tim.h"


// GLOBALS
extern osSemaphoreId_t BinarySemPWM_AccelerometerHandle;


/*******************************************************************************************************
* @brief This function will perform the initialization routines necessary to support task: PWM_AccelerometerTask()
* All items that need to be processed ONE TIME before the endless while / for loop of the task is called.
* This function should be placed within task before the endless for loop:
*
* @date				3/8/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Avoids excessive edit of main.c when using STM32CubeMX
*
* @param void
* @return void
*
* STEP 1:
****************************************************************************************************** */
void PWM_AccelerometerTask_Init(void)
{
	uint8_t Ctl_Reg1;
	uint8_t Ctl_Reg4;
	uint8_t Cfg_Reg;
	// STEP 1: Setup Accelerometer
	// UNIT TO OPERATE IN NORMAL MODE: SEE TABLE 10: OPERATING MODE SELECTION OF DATASHEET
	// USE OF DEFAULT VALUES HERE ONLY SETTING THE ODR
	uint8_t WriteBuffer[2];
	WriteBuffer[0] = LIS3DH_REGISTER_CTRL_REG1;
	Ctl_Reg1 = 0x57; // Enable 100Hz Update, x, y, z  HEX 0X50 IS ODER VALUE SHOULD OR THIS
	WriteBuffer[1] = Ctl_Reg1;
	// OTHER VALUE NECESSARY FOR 10 BIT OPERATION IS CTRL_REG4[3] WHICH BY DEFAULT IS WHAT WE WANT 0.  0 = 10BIT RESOLUTION
	writeAccelerometerRegister(WriteBuffer, sizeof(WriteBuffer));
	// SET LSI3DH ADC FOR TEMPERATURE READING - IN CASE YOU USE IT
	WriteBuffer[0] = LIS3DH_REGISTER_CTRL_REG4;
	Ctl_Reg4 = 0x80; // BDU is set
	WriteBuffer[1] = Ctl_Reg4;
	writeAccelerometerRegister(WriteBuffer, sizeof(WriteBuffer));
	WriteBuffer[0] = LIS3DH_REGISTER_TEMP_CFG_REG;
	Cfg_Reg = 0xC0; // ADC ENABLE AND TEMP ENABLE
	WriteBuffer[1] = Cfg_Reg;
	writeAccelerometerRegister(WriteBuffer, sizeof(WriteBuffer));

	// STEP 2: TEST
	TEST_FAIL_IF_FALSE(isAccelerometerOnLine());
	uint8_t RegisterValue = 0x00;
	readAccelerometerRegister(LIS3DH_REGISTER_CTRL_REG1, &RegisterValue, 1);
	TEST_FAIL_IF_FALSE(RegisterValue == Ctl_Reg1);
	readAccelerometerRegister(LIS3DH_REGISTER_CTRL_REG4, &RegisterValue, 1);
	TEST_FAIL_IF_FALSE(RegisterValue == Ctl_Reg4);
	readAccelerometerRegister(LIS3DH_REGISTER_TEMP_CFG_REG, &RegisterValue, 1);
	TEST_FAIL_IF_FALSE(RegisterValue == Cfg_Reg);
} // END OF FUNCTION PWM_AccelerometerTask_Init



/*******************************************************************************************************
* @brief This function will perform the repeated routines necessary to support task: PWM_AccelerometerTask()
* All items that need to be processed ENDLESSLY after the init function of the task has been called.
* This function should be placed within task endless for loop:
*
* @date				3/8/21 \n
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
void PWM_AccelerometerTask_ForLoop(void)
{
	static uint8_t InFunctionTicks = 0;

	if (TechDemoStatus.ActiveScreen == PA_SCREEN)
	{
		if (InFunctionTicks > 100)
		{
			uint8_t LowByte;
			uint8_t HighByte;
			Type_AccelerometerG GValue;
			Type_AccelerometerAngle AngleValue;
			static Type_LED_PWM_Percentage LastPWM_Value;

			// STEP 1: Get Accelerometer readings for all 3 axis
			// X AXSIS
			readAccelerometerRegister(LIS3DH_REGISTER_OUT_X_L, &LowByte, 1);
			readAccelerometerRegister(LIS3DH_REGISTER_OUT_X_H, &HighByte, 1);
			GValue.Acel_X = computeAcelerometerG(LowByte, HighByte, 10, 4);
			// Y AXSIS
			readAccelerometerRegister(LIS3DH_REGISTER_OUT_Y_L, &LowByte, 1);
			readAccelerometerRegister(LIS3DH_REGISTER_OUT_Y_H, &HighByte, 1);
			GValue.Acel_Y = computeAcelerometerG(LowByte, HighByte, 10, 4);
			// Z AXSIS
			readAccelerometerRegister(LIS3DH_REGISTER_OUT_Z_L, &LowByte, 1);
			readAccelerometerRegister(LIS3DH_REGISTER_OUT_Z_H, &HighByte, 1);
			GValue.Acel_Z = computeAcelerometerG(LowByte, HighByte, 10, 4);

			// STEP 2: Compute Tilt Angles
			computeThreeAxisAngularMeaure(&GValue, &AngleValue);
			TechDemoStatus.PWM_AccelerometerScreen.AccelerometerAngle = AngleValue;

			// STEP 3: Set PWM for Red LED
			if (LastPWM_Value.Red_LED != TechDemoStatus.PWM_AccelerometerScreen.PWM_Percent.Red_LED)
			{
				bool_t LED_UpdateStatus;
				LED_UpdateStatus = updateLED_PWM(&htim4, RED_LED_CHANNEL, TechDemoStatus.PWM_AccelerometerScreen.PWM_Percent.Red_LED);
				TEST_FAIL_IF_FALSE(LED_UpdateStatus);
				LastPWM_Value.Red_LED = TechDemoStatus.PWM_AccelerometerScreen.PWM_Percent.Red_LED;
			}

			// STEP 4: Set PWM for Red LED
			if (LastPWM_Value.Green_LED != TechDemoStatus.PWM_AccelerometerScreen.PWM_Percent.Green_LED)
			{
				bool_t LED_UpdateStatus;
				LED_UpdateStatus = updateLED_PWM(&htim4, GREEN_LED_CHANNEL, TechDemoStatus.PWM_AccelerometerScreen.PWM_Percent.Green_LED);
				TEST_FAIL_IF_FALSE(LED_UpdateStatus);
				LastPWM_Value.Green_LED = TechDemoStatus.PWM_AccelerometerScreen.PWM_Percent.Green_LED;
			}

			// STEP 5: Update display with new information
			osSemaphoreRelease(BinarySemPWM_AccelerometerHandle);
			InFunctionTicks = 0;
		}
		else
		{
			InFunctionTicks++;
		}
	}
} // END OF FUNCTION PWM_AccelerometerTask_ForLoop


/*******************************************************************************************************
* @brief Set the PWM positive duty cycle for the red and green User Interface LEDs.  If the PWM is 0 then
* turn off PWM
*
* @date				3/11/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Avoids excessive edit of main.c when using STM32CubeMX
*
* @param Handle to PWM Timer, LED Channel Number, Percentage 0 - 100 of LED on
* @return True or False on success
*
* STEP 1: Update PWM positive duty cycle if percent is 0 then just stop
****************************************************************************************************** */
bool_t updateLED_PWM(TIM_HandleTypeDef *Timer, uint32_t LED_Channel, uint8_t LED_PWM_Percent)
{
	// STEP 1: Update PWM positive duty cycle if percent is 0 then just stop
	if (LED_PWM_Percent != 0)
	{
		HAL_TIM_PWM_Stop(Timer, LED_Channel);
		TIM_OC_InitTypeDef ConfigOC = {0};
		ConfigOC.OCMode = TIM_OCMODE_PWM1;
		ConfigOC.Pulse = LED_PWM_Percent - 1; // Count from 0
		ConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		ConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
		if (HAL_TIM_PWM_ConfigChannel(Timer, &ConfigOC, LED_Channel) != HAL_OK)
		{
			return(FALSE);
		}
		HAL_TIM_PWM_Start(Timer, LED_Channel);
	}
	else
	{
		HAL_TIM_PWM_Stop(Timer, LED_Channel);
	}
	return(TRUE);
} // END OF updateLED_PWM
