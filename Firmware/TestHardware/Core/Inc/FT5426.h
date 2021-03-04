/*
 * FT5426.h
 *
 *  Created on: Mar 2, 2021
 *      Author: IMR HabPC
 */

#ifndef INC_FT5426_H_
#define INC_FT5426_H_

#include "MainSupport.h"

// FT5426 CONSTANTS
#define FT5426_SLAVE_ADDRESS			((uint8_t)0x70)
#define FT5426_DEVICE_ID				((uint8_t)0x79)
#define FT5426_RW_TIMEOUT_DELAY			(10U)

// FT5426 REGISTERS
#define FT5426_REGISTER_GESTURE			((uint8_t)0x01)
#define FT5426_REGISTER_TOUCH_POINTS	((uint8_t)0x02)
#define FT5426_REGISTER_TOUCH1_XH		((uint8_t)0x03)
#define FT5426_REGISTER_TOUCH1_XL		((uint8_t)0x04)
#define FT5426_REGISTER_TOUCH1_YH		((uint8_t)0x05)
#define FT5426_REGISTER_TOUCH1_YL		((uint8_t)0x06)
#define FT5426_REGISTER_DEVICE_ID		((uint8_t)0xA8)


// FUNCTION PROTOTYPES
bool_t readTouchRegister(uint8_t RegisterAddress, uint8_t *Buffer, uint8_t BytesToRead);

#endif /* INC_FT5426_H_ */
