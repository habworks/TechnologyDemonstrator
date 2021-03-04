/*
 * TPL0401A.c
 *
 *  Created on: Feb 22, 2021
 *      Author: IMR HabPC
 */

#include "TPL0401A.h"
#include "TimerSupport.h"
#include "i2c.h"

// Hab for application make this into two functions one to set one to read
bool_t setDigitalPotAttenuation(float PercentValue)
{

	uint8_t StepSet[2];
	uint8_t StepRead[10];	// NOTE ONLY 1 VALUE TO READ BACK - OVERSIZED AS A PERCAUTION
	HAL_StatusTypeDef Status_RW;

	if ((PercentValue > 100.0) || (PercentValue < 0))
		return(FALSE);
	StepSet[0] = TPL0401A_REGISTER_RW;
	StepSet[1] = (uint8_t)((PercentValue / 100.0) * (TPL0401A_TOTAL_STEPS - 1) + INTEGER_ROUNDING);
	StepSet[1] &= 0x7F;
	Status_RW = HAL_I2C_Master_Transmit(&hi2c1, TPL0401A_ADDRESS, StepSet, sizeof(StepSet), TPL0401A_RW_TIMEOUT_DELAY);
	if (Status_RW != HAL_OK)
		return(FALSE);

	//delayMiliSecond(1);  // REALLY NOT NEEDED

	Status_RW = HAL_I2C_Master_Receive(&hi2c1, TPL0401A_ADDRESS, StepRead, 1, TPL0401A_RW_TIMEOUT_DELAY);
	if (Status_RW != HAL_OK)
			return(FALSE);

	if (StepSet[1] != StepRead[0])
		return(FALSE);
	else
		return(TRUE);

} // END OF setDigitalPotAttenuation


