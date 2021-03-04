/*
 * LIS3DH.h
 *
 *  Created on: Feb 23, 2021
 *      Author: IMR HabPC
 */

#ifndef INC_LIS3DH_H_
#define INC_LIS3DH_H_

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


// CONSTANTS
#define	LIS3DH_WHO_AM_I_VALUE				((uint8_t)0x33)
#define LIS3DH_READ_OPERATION				((uint8_t)0x80)			// TO BE ORed WITH REGISTER ADDRESS ON READS
#define LIS3DH_SPI_BLOCK_TIMEOUT			(10U)

// MACROS
#define LIS3DH_CS_ENABLE()					HAL_GPIO_WritePin(ACEL_NEN_GPIO_Port, ACEL_NEN_Pin, GPIO_PIN_RESET)
#define LIS3DH_CS_DISABLE()					HAL_GPIO_WritePin(ACEL_NEN_GPIO_Port, ACEL_NEN_Pin, GPIO_PIN_SET)

// FUNCTION PROTOTYPES
bool_t isAccelerometerOnLine(void);
bool_t readAccelerometerRegister(uint8_t RegisterAddress, uint8_t *ReadValue, uint8_t BytesToRead);
bool_t writeAccelerometerRegister(uint8_t *WriteBuffer, uint8_t BufferLength);
float computeAcelerometerG(uint8_t LowByte, uint8_t HighByte, uint8_t BitResolution, uint8_t FullScale);
int16_t convertTwoCompliment(uint16_t Value, uint8_t NumberOfBits);
uint16_t temperatureAccelerometer(void);

#endif /* INC_LIS3DH_H_ */
