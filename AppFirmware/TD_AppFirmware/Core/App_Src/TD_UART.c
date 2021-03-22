/** ****************************************************************************************************
 * @file 			TD_UART.c
 * @brief			This is the Source file used to support USART related API functions
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
 * @note            This file provides functions that support all UART based APIs
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

#include "TD_UART.h"
#include "WifiTaskSupport.h"
extern uint8_t Uart2RxBuffer[3];

/*******************************************************************************************************
* @brief Transmit the contents of the DataBuffer, BufferSize bytes to the specified UART
*
* @date				3/21/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note
*
* @param Data buffer pointer, size of buffer, Uart Handle pointer
* @return true or false on try
*
* STEP 1: Transmit data buffer contents via UART channel
****************************************************************************************************** */
bool_t sendUartMsg(uint8_t *DataBuffer, uint8_t BufferSize, UART_HandleTypeDef *UartHandle)
{
	// STEP 1: Transmit data buffer contents via UART channel
	if (HAL_UART_Transmit_IT(UartHandle, DataBuffer, BufferSize) != HAL_OK)
		return(FALSE);
	else
		return(TRUE);
} // END OF sendUartMsg



/*******************************************************************************************************
* @brief This is the IRQ Callback function for the Uart.  It is defined as weak in HAL Lib.
*
* @date				3/21/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note This is the IRQ Callback function for the Uart.  It is defined as weak in HAL Lib.
* Each UART will have it own algo based on its associated tasks
* One byte of UART received data is passed
*
* @param Pointer to the UART with IRQ
* @return void
*
* STEP 1: UART specific
****************************************************************************************************** */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart2)
	{
		WIFI_UartRxHandler(Uart2RxBuffer, 1);
		// Enable IRQ again
		HAL_UART_Receive_IT(&huart2, Uart2RxBuffer, 1);
 	}

} // END OF HAL_UART_RxCpltCallback
