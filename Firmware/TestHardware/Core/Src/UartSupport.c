/*
 * Uart.c
 *
 *  Created on: Jan 16, 2021
 *      Author: IMR HabPC
 */

#include "UartSupport.h"
#include "TimerSupport.h"

#include "usart.h"

#include "string.h"
#include "stdio.h"

#include "usbd_cdc_if.h"

extern uint8_t DebugUartInputMsg[10];

void Uart3_CommandHandler(void)
{

	BoardStatus.UartCmdChar = DebugUartInputMsg[0];

}

#ifndef USE_USB_VCP_AS_DEBUG_PORT
void SendDebugUartMsg(uint8_t *Msg)
{
	if (HAL_UART_Transmit_IT(&huart3, (uint8_t *)Msg, strlen((char *)Msg)) != HAL_OK)
	{
		// Check for error
	}
	delayMiliSecond(PRINT_TIME_DELAY);
}
#else
void SendDebugUartMsg(uint8_t *Msg)
{
	CDC_Transmit_FS(Msg, strlen((char *)Msg));
	delayMiliSecond(PRINT_TIME_DELAY);
}
#endif

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if (huart->Instance == USART3)
	{
		Uart3_CommandHandler();
		HAL_UART_Receive_IT(&huart3, DebugUartInputMsg, 1);
	}
	else
	{
		while(1);
	}

}

void printErrorHandler(void)
{
	uint8_t DebugUarOutputtMsg[100];

	sprintf((char *)DebugUarOutputtMsg, "System Error on line: %dr\n", __LINE__);
	SendDebugUartMsg(DebugUarOutputtMsg);
}
