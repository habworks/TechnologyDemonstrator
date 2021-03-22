/** ****************************************************************************************************
 * @file 			TD_UART.h
 * @brief			This is the Header file used to support TD_UART.c
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
 * @note            See source file for notes
 *
 * 					This is an embedded application
 *		            It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *			          IMR Technology Demonstrator Schematic: IMR002S REV 1
 *				      IMR Technology Demonstrator Schematic: IMR002S REV 1
 *
 * @copyright		IMR Engineering
 * **************************************************************************************************** */
#ifndef APP_INC_TD_UART_H_
#define APP_INC_TD_UART_H_
#ifdef __cplusplus
extern"C" {
#endif

#include "MainSupport.h"
#include "usart.h"


// FUNCTION PROTOTYPES
bool_t sendUartMsg(uint8_t *DataBuffer, uint8_t BufferSize, UART_HandleTypeDef *UartHandle);


#ifdef __cplusplus
}
#endif
#endif /* APP_INC_TD_UART_H_ */
