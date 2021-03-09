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

// MACROS
#define UNUSED_FUNCTION(X) 					(void)X      // To avoid gcc/g++ warnings
#define TEST_RETURN_IF_FAIL(__condition__)	do \
											{ \
												if(__condition__) \
												{  \
													;  \
												} \
                                            } while(0)

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
	PA_SCREEN
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
	Type_ActiveScreen				ActiveScreen;
	Type_AnalogDigitalScreen		AnalogDigitalScreen;
}Type_TechDemoStatus;
extern Type_TechDemoStatus TechDemoStatus;

// FUNCTION PROTOTYPES
void main_Init(void);
void main_WhileLoop(void);
// CONSIDER MOVING
void defaultTouchGFxTask_Init(void);


#ifdef __cplusplus
}
#endif
#endif /* APP_INC_MAINSUPPORT_H_ */
