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
	PWR_5V_OFF();
	WIFI_OFF();
	AUDIO_AMP_OFF();
	VUSB_PWR_OFF();

	// DISPLAY STUFF
	// TURN ON BACK LIGHT AND SET BACK LIGHT CONDITIONS
	TOUCH_RST_ENABLE();
	DISPLAY_ON();
	if (DISPLAY_EN_STATUS() != GPIO_PIN_SET)
		Error_Handler();
	BACKLIGHT_ON();
	TOUCH_RST_DISABLE();

} //END OF FUNCTION main_Init



void main_WhileLoop(void)
{

	static uint32_t StatusLED_Counter = 0;

	StatusLED_Counter++;
	if (StatusLED_Counter > 150)
	{
		STATUS_LED_TOGGLE();
		StatusLED_Counter = 0;
	}

} //END OF main_WhileLoop




