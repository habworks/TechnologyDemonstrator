/** ****************************************************************************************************
 * @file 			Driver_LSI3DH.h
 * @brief			This is the Header file used to support LSI3DH.c
 * ****************************************************************************************************
 * @author			Hab S. Collector \n
 * Last Edited By:	Hab S. Collector \n
 *
 * @date			3/9/21 \n
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

#ifndef APP_INC_DRIVER_LSI3DH_H_
#define APP_INC_DRIVER_LSI3DH_H_
#ifdef __cplusplus
extern"C" {
#endif

#include "MainSupport.h"

// REGISTER ADDRESS VALUES
#define LIS3DH_REGISTER_OUT_ADC3_L			((uint8_t)0x0C)
#define LIS3DH_REGISTER_OUT_ADC3_H			((uint8_t)0x0D)
#define LIS3DH_REGISTER_WHO_AM_I			((uint8_t)0x0F)
#define LIS3DH_REGISTER_TEMP_CFG_REG		((uint8_t)0x1F)
#define LIS3DH_REGISTER_CTRL_REG1			((uint8_t)0x20)
#define LIS3DH_REGISTER_CTRL_REG2			((uint8_t)0x21)
#define LIS3DH_REGISTER_CTRL_REG3			((uint8_t)0x22)
#define LIS3DH_REGISTER_CTRL_REG4			((uint8_t)0x23)
#define LIS3DH_REGISTER_CTRL_REG5			((uint8_t)0x24)
#define LIS3DH_REGISTER_CTRL_REG6			((uint8_t)0x25)
#define LIS3DH_REGISTER_OUT_X_L				((uint8_t)0x28)
#define LIS3DH_REGISTER_OUT_X_H				((uint8_t)0x29)
#define LIS3DH_REGISTER_OUT_Y_L				((uint8_t)0x2A)
#define LIS3DH_REGISTER_OUT_Y_H				((uint8_t)0x2B)
#define LIS3DH_REGISTER_OUT_Z_L				((uint8_t)0x2C)
#define LIS3DH_REGISTER_OUT_Z_H				((uint8_t)0x2D)

// DEFINES
#define	LIS3DH_WHO_AM_I_VALUE				((uint8_t)0x33)
#define LIS3DH_READ_OPERATION				((uint8_t)0x80)			// TO BE ORed WITH REGISTER ADDRESS ON READS
#define LIS3DH_SPI_BLOCK_TIMEOUT			(10U)

// MACROS
#define LIS3DH_CS_ENABLE()					HAL_GPIO_WritePin(ACEL_NEN_GPIO_Port, ACEL_NEN_Pin, GPIO_PIN_RESET)
#define LIS3DH_CS_DISABLE()					HAL_GPIO_WritePin(ACEL_NEN_GPIO_Port, ACEL_NEN_Pin, GPIO_PIN_SET)

// TYPEDEFS AND STRUCTS
typedef struct
{
	float		Acel_X;
	float		Acel_Y;
	float		Acel_Z;
}Type_AccelerometerG;

// FUNCTION PROTOTYPES
bool_t isAccelerometerOnLine(void);
bool_t readAccelerometerRegister(uint8_t RegisterAddress, uint8_t *ReadValue, uint8_t BytesToRead);
bool_t writeAccelerometerRegister(uint8_t *WriteBuffer, uint8_t BufferLength);
float computeAcelerometerG(uint8_t LowByte, uint8_t HighByte, uint8_t BitResolution, uint8_t FullScale);
int16_t convertTwoCompliment(uint16_t Value, uint8_t NumberOfBits);
bool_t computeThreeAxisAngularMeaure(const Type_AccelerometerG *AccelerometerG, Type_AccelerometerAngle *AccelerometerAngle);
uint16_t temperatureAccelerometer(void);

#ifdef __cplusplus
}
#endif
#endif /* APP_INC_DRIVER_LSI3DH_H_ */
