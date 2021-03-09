/** ****************************************************************************************************
 * @file 			QuadSPI_Support.h
 * @brief			This is the Header file used to support QuadSPI_Support.C
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

#ifndef APP_INC_QUADSPI_SUPPORT_H_
#define APP_INC_QUADSPI_SUPPORT_H_
#ifdef __cplusplus
extern"C" {
#endif

#include "MainSupport.h"


// FUNCTION PROTOTYPES
void quadSPI_InitSupport(void);

#ifdef __cplusplus
}
#endif
#endif /* APP_INC_QUADSPI_SUPPORT_H_ */
