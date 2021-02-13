/*
 * MainSupport.c
 *
 *  Created on: Feb 9, 2021
 *      Author: IMR HabPC
 */

#include "MainSupport.h"
#include "TD_SupportIO.h"

//#include "stm32746g_discovery_qspi.h"


void main_Init(void)
{

	// SET GPIO DEFAULT CONTIONDS
	STATUS_LED_OFF();

	// TURN ON BACK LIGHT AND SET BACK LIGHT CONDITIONS
	DISPLAY_ON();
	if (DISPLAY_EN_STATUS() != GPIO_PIN_SET)
		Error_Handler();
	BACKLIGHT_ON();

	// QUAD SPI INIT
//	BSP_QSPI_Init();
//	BSP_QSPI_MemoryMappedMode();
//	HAL_NVIC_DisableIRQ(QUADSPI_IRQn);

} //END OF FUNCTION main_Init



void main_WhileLoop(void)
{

	static uint32_t StatusLED_Counter = 0;

	StatusLED_Counter++;
	if (StatusLED_Counter > 250)
	{
		STATUS_LED_TOGGLE();
		StatusLED_Counter = 0;
	}

} //END OF main_WhileLoop




