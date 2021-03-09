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


// GLOBALS
//extern osSemaphoreId_t BinarySemPWM_AccelerometerHandle;


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
* STEP 2:
****************************************************************************************************** */
//void analogDigitaTask_Init(void)
//{
//
//
//}

