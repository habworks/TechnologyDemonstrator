/*
 * FT5426.c
 *
 *  Created on: Mar 2, 2021
 *      Author: IMR HabPC
 */

#include "FT5426.h"
#include "i2c.h"

bool_t readTouchRegister(uint8_t RegisterAddress, uint8_t *Buffer, uint8_t BytesToRead)
{
	HAL_StatusTypeDef ReadStatus;

	ReadStatus = HAL_I2C_Mem_Read(&hi2c3, FT5426_SLAVE_ADDRESS, RegisterAddress, 1, Buffer, BytesToRead, FT5426_RW_TIMEOUT_DELAY);
	if (ReadStatus == HAL_OK)
		return(TRUE);
	else
		return(FALSE);

}
