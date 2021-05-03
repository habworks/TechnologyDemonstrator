/** ****************************************************************************************************
 * @file 			WifiTaskSupport.c
 * @brief			This is the Source file used to support tasks associated with the WIFI Screen
 * ****************************************************************************************************
 * @author			Hab S. Collector \n
 * Last Edited By:	Hab S. Collector \n
 *
 * @date			3/21/21 \n
 * @version       	See Main_Support.h: FIRMWARE_REV_MAJOR, FIRMWARE_REV_MINOR
 *
 * @param Development_Environment \n
 * Hardware:		STM32F746IGT6 \n
 * IDE:             STM32CubeIDE, VER 1.5.1 \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            This file provides a functions that support the WIFI task WIFI_AP_Task()
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

#include "WifiTaskSupport.h"
#include "TD_SupportIO.h"
#include "PWM_AccelerometerTaskSupport.h"
#include "usart.h"
#include "tim.h"

// GLOBAL VARS
uint8_t Uart2RxBuffer[3];

/*******************************************************************************************************
* @brief This function will perform the initialization routines necessary to support task: WIFI_AP_Task()
* All items that need to be processed ONE TIME before the endless while / for loop of the task is called.
* This function should be placed within task before the endless for loop:
*
* @date				3/21/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Avoids excessive edit of main.c when using STM32CubeMX
*
* @param void
* @return void
*
* STEP 1: Enable the WIFI
****************************************************************************************************** */
void WIFI_AP_Task_Init(void)
{
	// STEP 1: Enable the WIFI and start UART 2
	WIFI_ON();  //TODO Hab need enable wifi when the screen is on / disable when screen off
	HAL_UART_Receive_IT(&huart2, Uart2RxBuffer, 1);

} // END OF WIFI_AP_Task_Init



/*******************************************************************************************************
* @brief This function will perform the repeated routines necessary to support task: WIFI_AP_Task()
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
* STEP 1: On valid packet received from remote connection set LED (Red / Green) On off based on packet info
* STEP 2: Check for GUI click up / down  and transmit information to remote connection
****************************************************************************************************** */
void WIFI_AP_Task_ForLoop(void)
{
	// STEP 1: On valid packet received set LED (Red / Green) On off based on packet info
	if (TechDemoStatus.ActiveScreen == WIFI_SCREEN)
	{
		// STEP 1: On valid packet received from remote connection set LED (Red / Green) On off based on packet info
		if (TechDemoStatus.WifiScreen.PacketReceive)
		{
			switch(TechDemoStatus.WifiScreen.UartMsgDataRx[DATA_SELECT_INDEX])
			{
			case RED_LED:
			{
				if (TechDemoStatus.WifiScreen.UartMsgDataRx[DATA_VALUE_INDEX] == LED_ON)
					updateLED_PWM(&htim4, RED_LED_CHANNEL, 100);
				else
					updateLED_PWM(&htim4, RED_LED_CHANNEL, 0);
				break;
			}

			case GREEN_LED:
			{
				if (TechDemoStatus.WifiScreen.UartMsgDataRx[DATA_VALUE_INDEX] == LED_ON)
					updateLED_PWM(&htim4, GREEN_LED_CHANNEL, 100);
				else
					updateLED_PWM(&htim4, GREEN_LED_CHANNEL, 0);
				break;
			}

			default:
				break;
			}
			TechDemoStatus.WifiScreen.PacketReceive = FALSE;
		}

		// STEP 2: Check for GUI click up / down  and transmit information to remote connection
		static uint8_t Last_UI_Update1 = 0;
		static uint8_t Last_UI_Update2 = 0;
		TechDemoStatus.WifiScreen.UastMsgDataTx[HEADER_INDEX] = HEADER_BYTE;
		// UI Update 1
		if (TechDemoStatus.WifiScreen.UpdateCount1 != Last_UI_Update1)
		{

			TechDemoStatus.WifiScreen.UastMsgDataTx[DATA_SELECT_INDEX] = UI_UPDATE_1;
			TechDemoStatus.WifiScreen.UastMsgDataTx[DATA_VALUE_INDEX] = TechDemoStatus.WifiScreen.UpdateCount1;
			calculateSimpleCheckSum(TechDemoStatus.WifiScreen.UastMsgDataTx, SIMPLE_PROTOCOL_LENGTH);
			sendUartMsg(TechDemoStatus.WifiScreen.UastMsgDataTx, SIMPLE_PROTOCOL_LENGTH, &huart2);
			Last_UI_Update1 = TechDemoStatus.WifiScreen.UpdateCount1;
		}
		// UI Update 2
		if (TechDemoStatus.WifiScreen.UpdateCount2 != Last_UI_Update2)
		{
			TechDemoStatus.WifiScreen.UastMsgDataTx[DATA_SELECT_INDEX] = UI_UPDATE_2;
			TechDemoStatus.WifiScreen.UastMsgDataTx[DATA_VALUE_INDEX] = TechDemoStatus.WifiScreen.UpdateCount2;
			calculateSimpleCheckSum(TechDemoStatus.WifiScreen.UastMsgDataTx, SIMPLE_PROTOCOL_LENGTH);
			sendUartMsg(TechDemoStatus.WifiScreen.UastMsgDataTx, SIMPLE_PROTOCOL_LENGTH, &huart2);
			Last_UI_Update2 = TechDemoStatus.WifiScreen.UpdateCount2;
		}
	}
} // END OF WIFI_AP_Task_ForLoop



/*******************************************************************************************************
* @brief This function accepts incoming Rx data from UART 2 and parses the data into a valid message
* A simple protocol is used
*   A header (first byte) of value HEADER_BYTE
*   Second Byte is Data Type
*   Third Byte is Data Value
*   Fourth Byte is a simple uint8_t Checksum
*   Fixed length of 4 bytes
* UART 2 is connected to the Serial Port of the ESP8266 module.  The ESP8266 module transmits via this
* port what the remote WIFI connection is setting the RED or GREEN LED to (on / off)
*
* @date				3/21/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Avoids excessive edit of main.c when using STM32CubeMX
*
* @param void
* @return void
*
* STEP 1: Parse incoming Rx Data for Header Byte Information advance search if found
* STEP 2: Parse incoming Rx Data for valid type Information advance search if found, if not reset
* STEP 3: Parse incoming Rx Data for type value and advance search
****************************************************************************************************** */
void WIFI_UartRxHandler(const uint8_t *RxDataBuffer, uint8_t BufferSize)
{
	static uint8_t State = 0;

	while (BufferSize > 0)
	{
		switch(State)
		{
		// STEP 1: Parse incoming Rx Data for Header Byte Information advance search if found
		case HEADER_INDEX:
		{
			if (*RxDataBuffer == HEADER_BYTE)
			{
				TechDemoStatus.WifiScreen.UartMsgDataRx[State] = HEADER_BYTE;
				State++;
			}
			break;
		}

		// STEP 2: Parse incoming Rx Data for valid type Information advance search if found, if not reset
		case DATA_SELECT_INDEX:
		{
			if ((*RxDataBuffer == RED_LED) || (*RxDataBuffer == GREEN_LED))
			{
				TechDemoStatus.WifiScreen.UartMsgDataRx[State] = *RxDataBuffer;
				State++;
			}
			else
			{
				State = HEADER_INDEX;
			}
			break;
		}

		// STEP 3: Parse incoming Rx Data for type value and advance search
		case DATA_VALUE_INDEX:
		{
			TechDemoStatus.WifiScreen.UartMsgDataRx[State] = *RxDataBuffer;
			State++;
			break;
		}

		case SIMPLE_CHECKSUM_INDEX:
		{
			calculateSimpleCheckSum(TechDemoStatus.WifiScreen.UartMsgDataRx, SIMPLE_PROTOCOL_LENGTH);
			if (TechDemoStatus.WifiScreen.UartMsgDataRx[SIMPLE_CHECKSUM_INDEX] == *RxDataBuffer)
			{
				// TODO Hab fix later this would be better done with a msg queue - just get it working for now
				TechDemoStatus.WifiScreen.PacketReceive = TRUE;
			}
			State = HEADER_INDEX;
			break;
		}

		default:
			State = HEADER_INDEX;
			break;
		} // END STATE SWITCH
		BufferSize--;
	} // END WHILE
} // END OF WIFI_UartRxHandler



/*******************************************************************************************************
* @brief Simple checksum to verify data integrity of the transmitted packet.  A simple uint8_t addition
* of all bytes within the Buffer is calcualte with no overflow
*
* @date				3/21/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Supports the simple protocol in use - non overflow addition of all bytes within buffer
*
* @param Buffer pointer, Size of the buffer
* @return By reference the computed sum is placed within the last element of the buffer
*
* STEP 1: Calculate simple non-overflow unit8_t data sum
* STEP 2: Return the sum by reference within data buffer pointer
****************************************************************************************************** */
void calculateSimpleCheckSum(uint8_t *Buffer, uint8_t Size)
{
  uint8_t SimpleSum = 0;

  // STEP 1: Calculate simple non-overflow unit8_t data sum
  for (uint8_t Index = 0; Index < Size - 1; Index++)
  {
    SimpleSum = SimpleSum + Buffer[Index];
  }

  // STEP 2: Return the sum by reference within data buffer pointer
  Buffer[Size - 1] = SimpleSum;
} // END OF calculateSimpleCheckSum
