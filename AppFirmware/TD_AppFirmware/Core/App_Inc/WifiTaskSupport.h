/** ****************************************************************************************************
 * @file 			WifiTaskSupport.h
 * @brief			This is the Header file used to support WifiTaskSupport.c
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

#ifndef APP_INC_WIFITASKSUPPORT_H_
#define APP_INC_WIFITASKSUPPORT_H_
#ifdef __cplusplus
extern"C" {
#endif

#include "MainSupport.h"

// DEFINES
// VERY SIMPLE MSG PROTOCOL: Always FourBytes, First Byte always Header Byte, Last Byte always simple sum
#define HEADER_BYTE           	0xF0
#define HEADER_INDEX          	0
#define DATA_SELECT_INDEX   	1
#define DATA_VALUE_INDEX    	2
#define SIMPLE_CHECKSUM_INDEX 	3
#define SIMPLE_PROTOCOL_LENGTH	4
// FOR LED STATE RECEPTION
#define RED_LED               	1     // For use in Data Select Index 1
#define GREEN_LED             	2
#define LED_ON                	1     // For use in Data Value Index 2
#define LED_OFF               	0
// FOR UI UPDATE TRANSMISSION
#define UI_UPDATE_1           	1     // For UI channel Select Index
#define UI_UPDATE_2           	2


// FUNCITON PROTOTYPES
void WIFI_AP_Task_Init(void);
void WIFI_AP_Task_ForLoop(void);
void WIFI_UartRxHandler(const uint8_t *RxDataBuffer, uint8_t BufferSize);
void calculateSimpleCheckSum(uint8_t *Buffer, uint8_t Size);

#ifdef __cplusplus
}
#endif
#endif /* APP_INC_WIFITASKSUPPORT_H_ */
