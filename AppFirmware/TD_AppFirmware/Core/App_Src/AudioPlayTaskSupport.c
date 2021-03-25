/** ****************************************************************************************************
 * @file 			AudioPlayTaskSupport.c
 * @brief			This is the Source file used to support tasks associated with the Music play screen and audio
 * ****************************************************************************************************
 * @author			Hab S. Collector \n
 * Last Edited By:	Hab S. Collector \n
 *
 * @date			3/23/21 \n
 * @version       	See Main_Support.h: FIRMWARE_REV_MAJOR, FIRMWARE_REV_MINOR
 *
 * @param Development_Environment \n
 * Hardware:		STM32F746IGT6 \n
 * IDE:             STM32CubeIDE, VER 1.5.1 \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            This file provides a functions that support the Music and Audio task audioPlayTask()
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

#include "AudioPlayTaskSupport.h"
#include "cmsis_os2.h"
#include "tim.h"
// MIDDLEWEAR
#include "fatfs.h"
//#include "usbd_cdc_if.h"


#include "TD_SupportIO.h"
#include "TD_UART.h"
#include "stdio.h"
#include "string.h"
// GLOBALS
extern osMessageQueueId_t AudioQueueHandle;

void audioPlayTask_Init(void)
{

} // END OF




void audioPlayTask_ForLoop(void)
{

	osStatus_t QueueStatus;
	Type_AudioMsg AudioMsg;
	static uint8_t DummyCount = 0;
	static bool_t Test = TRUE;

	QueueStatus = osMessageQueueGet(AudioQueueHandle, &AudioMsg, NULL, 0U);
	if (QueueStatus == osOK)
	{
//		DummyCount++;
//		if (Test)
//		{
//			HAL_TIM_Base_Start_IT(&htim3);
//			Test = FALSE;
//		}
//		else
//		{
//			HAL_TIM_Base_Stop_IT(&htim3);
//			Test = TRUE;
//		}
		testMicro_SD();
	}

} // END OF audioPlayTask_ForLoop



void callbackHandlerTimer3(void)
{

		STATUS_LED_TOGGLE();
}


extern UART_HandleTypeDef huart3;
void testMicro_SD(void)
{

	char Line1[] = "Hab This is line 1 - OK\r\n";
	char Line2[] = "Hab This is line 2 - OK\r\n";
	char Line3[] = "Hab This is line 3 - OK\r\n";
	uint8_t DebugUarOutputtMsg[50];
	UINT BytesWritten;

	// READ FROM DISK
	sprintf((char *)DebugUarOutputtMsg, "Test uSD Read:\r\n");
	sendUartMsg((uint8_t *)DebugUarOutputtMsg, strlen((char *)DebugUarOutputtMsg), &huart3);
	uint8_t line[100]; /* Line buffer */
	FRESULT fr;     /* FatFs return code */

	/* Gives a work area to the default drive */
	fr = f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
	if (fr != FR_OK)
	{
		sprintf((char *)DebugUarOutputtMsg, "ERROR: Drive failed to mount\r\n");
		sendUartMsg((uint8_t *)DebugUarOutputtMsg, strlen((char *)DebugUarOutputtMsg), &huart3);
		Error_Handler();
	}
	/* Open a text file */
	fr = f_open(&SDFile, "0:SrcFile.txt", FA_READ);
	if (fr != FR_OK)
	{
		sprintf((char *)DebugUarOutputtMsg, "ERROR: Read File open\r\n");
		sendUartMsg((uint8_t *)DebugUarOutputtMsg, strlen((char *)DebugUarOutputtMsg), &huart3);
		f_mount(0,"0:",0);
		return;
		Error_Handler();
	}
	/* Read every line and display it */
	while (f_gets((char *)line, sizeof line, &SDFile))
	{
		sendUartMsg((uint8_t *)line, strlen((char *)line), &huart3);
		sprintf((char *)DebugUarOutputtMsg, "\r");
		sendUartMsg((uint8_t *)DebugUarOutputtMsg, strlen((char *)DebugUarOutputtMsg), &huart3);
	}

	/* Close the file */
	f_close(&SDFile);

	// WRITE TO FILE
	sprintf((char *)DebugUarOutputtMsg, "\r\nRead PASS OK\r\n\nTesting uSD Write File: \r\n");
	sendUartMsg((uint8_t *)DebugUarOutputtMsg, strlen((char *)DebugUarOutputtMsg), &huart3);
	fr = f_open(&SDFile, "0:DstFileA.txt", FA_CREATE_ALWAYS | FA_WRITE);
	if (fr != FR_OK)
	{
		sprintf((char *)DebugUarOutputtMsg, "ERROR: File Write Test: File failed to create\r\n");
		sendUartMsg((uint8_t *)DebugUarOutputtMsg, strlen((char *)DebugUarOutputtMsg), &huart3);
		Error_Handler();
	}
	// Write 3 lines only test the first
	f_write(&SDFile, Line1, sizeof(Line1), &BytesWritten);
	if ((BytesWritten == 0) || (BytesWritten < sizeof(Line1)))
	{
		sprintf((char *)DebugUarOutputtMsg, "ERROR: File Write Test: Data failed to write\r\n");
		sendUartMsg((uint8_t *)DebugUarOutputtMsg, strlen((char *)DebugUarOutputtMsg), &huart3);
		Error_Handler();
	}
	f_write(&SDFile, Line2, sizeof(Line1), &BytesWritten);
	f_write(&SDFile, Line3, sizeof(Line1), &BytesWritten);
	f_close(&SDFile);
	sprintf((char *)DebugUarOutputtMsg, "PASS OK\r\n");
	sendUartMsg((uint8_t *)DebugUarOutputtMsg, strlen((char *)DebugUarOutputtMsg), &huart3);
}
