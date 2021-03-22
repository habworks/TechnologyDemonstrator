/*
 * MainSupport.h
 *
 *  Created on: Feb 9, 2021
 *      Author: IMR HabPC
 */

#ifndef APP_INC_MAINSUPPORT_H_
#define APP_INC_MAINSUPPORT_H_
#ifdef __cplusplus
extern"C" {
#endif

#include "main.h"
#include "stdint.h"

// DEFINES
#define FIRMWARE_REV_MAJOR			1U
#define FIRMWARE_REV_MINOR			0U
#define ROUNDING					0.5

// TYPEDEFS AND ENUMS
//typedef _Bool bool;
#ifndef bool_t
	#ifdef FALSE
		#undef FALSE
	#endif
	#ifdef TRUE
		#undef TRUE
	#endif
typedef enum
{
	FALSE = 0,
	TRUE
}bool_t;
#endif

typedef enum
{
	NO_ACTIVE_SCREEN,
	SPLASH_SCREEN,
	MAIN_MENU_SCREEN,
	AD_SCREEN,
	PA_SCREEN,
	WIFI_SCREEN
}Type_ActiveScreen;

typedef struct
{
	float		UserInterfaceVoltage;
	float		BatteryVoltage;
	float		ChipTempConversioninCelcius;
	uint8_t		HexSwitchValue;
	char		HexSwitchChar;
	bool_t		RefreshGraph;
}Type_AnalogDigitalScreen;

typedef struct
{
	uint8_t		Red_LED;
	uint8_t		Green_LED;
}Type_LED_PWM_Percentage;

typedef struct
{
	float 	X_Degrees;
	float	Y_Degrees;
	float	Z_Degrees;
}Type_AccelerometerAngle;

typedef struct
{
	Type_LED_PWM_Percentage		PWM_Percent;
	Type_AccelerometerAngle		AccelerometerAngle;
}Type_PWM_AccelerometerScreen;

typedef struct
{
	uint8_t						UartMsgDataRx[6];
	uint8_t						UastMsgDataTx[6];
	bool_t						PacketReceive;
	uint8_t						UpdateCount1;
	uint8_t						UpdateCount2;
}Type_WifiScreen;

typedef struct
{
	Type_ActiveScreen				ActiveScreen;
	Type_AnalogDigitalScreen		AnalogDigitalScreen;
	Type_PWM_AccelerometerScreen	PWM_AccelerometerScreen;
	Type_WifiScreen					WifiScreen;
}Type_TechDemoStatus;
extern Type_TechDemoStatus TechDemoStatus;


// MACROS
#define SUPPRESS_WARNING(FunctionName) 				(void)FunctionName      // To avoid gcc/g++ warnings
#define TEST_RETURN_TRUE_OR_FAIL(__condition__)		do \
													{ \
														if(__condition__) \
															return(TRUE);  \
														else \
															Error_Handler(); \
													} while(0)
#define TEST_FAIL_IF_FALSE(__condition__)		do \
													{ \
														if((__condition__) != TRUE) \
															Error_Handler(); \
													} while(0)


// FUNCTION PROTOTYPES
void main_Init(void);
void main_WhileLoop(void);
// CONSIDER MOVING
void defaultTouchGFxTask_Init(void);

#ifdef __cplusplus
}
#endif
#endif /* APP_INC_MAINSUPPORT_H_ */
