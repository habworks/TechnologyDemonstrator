/** ****************************************************************************************************
 * @file 			AnalogDigitalTaskSupport.h
 * @brief			This is the Header file used to support AnalogDigitalTaskSupport.c
 * ****************************************************************************************************
 * @author			Hab S. Collector \n
 * Last Edited By:	Hab S. Collector \n
 *
 * @date			3/7/21 \n
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


#ifndef APP_INC_ANALOGDIGITALTASKSUPPORT_H_
#define APP_INC_ANALOGDIGITALTASKSUPPORT_H_
#ifdef __cplusplus
extern"C" {
#endif

#include "MainSupport.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim1;

// DEFINES
// ADC SPECIFIC
#define NUMBER_OF_ADC_CHANNELS				3U
#define ADC_SAMPLES_PER_ACQUISION			5U
#define ADC_DMA_SIZE 						(NUMBER_OF_ADC_CHANNELS * ADC_SAMPLES_PER_ACQUISION)
#define BAT_DIVIDER_RATIO					(100.0/(100.0 + 40.2)) //Resistor divider
#define ADC_VREF							3.3
#define ADC_12B_FULL_SCALE					((uint16_t)0xFFF)
#define ADC_10B_FULL_SCALE					((uint16_t)0x3FF)
#define ADC_8B_FULL_SCALE					((uint16_t)0xFF)
#define ADC_6B_FULL_SCALE					((uint16_t)0x3F)
// MISC
#define ASCII_DIGIT_CHAR_OFFSET				((uint8_t)0x30)
#define ASCII_CAP_LETTER_CHAR_OFFSET		((uint8_t)0x41)

// TYPEDEFS AND ENUMS
typedef struct
{
	uint32_t	ADC_UserInterfaceSum;
	uint32_t	ADC_BatReadSum;
	uint32_t 	ADC_TempSensorSum;
}Type_AnalogDigitalSupportTask;

// FUNCTION PROTOTYPES
void analogDigitaTask_Init(void);
void analogDigitalTask_ForLoop(void);
void avgAndConvertAnalogChannels(const Type_AnalogDigitalSupportTask *);
bool_t ADC_FullScale(ADC_HandleTypeDef *ADC_Handle, uint16_t *FullScale);
uint8_t getUserInterfaceHexSwitchValue(void);
bool_t convertHexSwitchValueToCharacter(uint8_t HexSwitchValue, char *HexSwitchChar);

#ifdef __cplusplus
}
#endif
#endif /* APP_INC_ANALOGDIGITALTASKSUPPORT_H_ */
