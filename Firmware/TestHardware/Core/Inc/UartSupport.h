/*
 * Uart.h
 *
 *  Created on: Jan 16, 2021
 *      Author: IMR HabPC
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "MainSupport.h"

// DEFINES
//#define USE_USB_VCP_AS_DEBUG_PORT

// FUNCTION PROTOTYPE
void Uart3_CommandHandler(void);
void SendDebugUartMsg(uint8_t *Msg);
void printErrorHandler(void);

#endif /* INC_UART_H_ */
