/*
 * TD_SDRAM.c
 *
 *  Created on: Feb 9, 2021
 *      Author: IMR HabPC
 */

#include "TD_SDRAM.h"
#include "fmc.h"
//#include "stm32f7xx_hal_sdram.h"
//extern SDRAM_HandleTypeDef hsdram1;


void SDRAM_InitSequence(void)
{

	FMC_SDRAM_CommandTypeDef FMC_SDRAMCommandStructure;
	uint32_t tmpr = 0;

	/* Step 1 --------------------------------------------------------------------*/
	/* Configure a clock configuration enable command */
	FMC_SDRAMCommandStructure.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	FMC_SDRAMCommandStructure.AutoRefreshNumber = 1;
	FMC_SDRAMCommandStructure.ModeRegisterDefinition = 0;
	while( (HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY) );	// Check Busy Flag before sending command
	if (HAL_SDRAM_SendCommand(&hsdram1, &FMC_SDRAMCommandStructure, SDRAM_TIMEOUT) != HAL_OK)
		Error_Handler();

	/* Step 2 --------------------------------------------------------------------*/
	/* Configure a PALL (precharge all) command */
	FMC_SDRAMCommandStructure.CommandMode = FMC_SDRAM_CMD_PALL;
	FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	FMC_SDRAMCommandStructure.AutoRefreshNumber = 1;
	FMC_SDRAMCommandStructure.ModeRegisterDefinition = 0;
	while( (HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY) );	// Check Busy Flag before sending command
	if (HAL_SDRAM_SendCommand(&hsdram1, &FMC_SDRAMCommandStructure, SDRAM_TIMEOUT) != HAL_OK)
		Error_Handler();

	/* Step 3 --------------------------------------------------------------------*/
	/* Configure a Auto-Refresh command */
	FMC_SDRAMCommandStructure.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	FMC_SDRAMCommandStructure.AutoRefreshNumber = 4; // ******HAB THERE IS A DIFFERENCE HERE *******
	FMC_SDRAMCommandStructure.ModeRegisterDefinition = 0;
	while( (HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY) );	// Check Busy Flag before sending command
	if (HAL_SDRAM_SendCommand(&hsdram1, &FMC_SDRAMCommandStructure, SDRAM_TIMEOUT) != HAL_OK)
		Error_Handler();

	/* Step 4 --------------------------------------------------------------------*/
	/* Program the external memory mode register */
	tmpr = (uint32_t)	SDRAM_MODEREG_BURST_LENGTH_1     |
					SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL  |
					SDRAM_MODEREG_CAS_LATENCY_3      |   // ******HAB THERE IS A DIFFERENCE HERE *******
					SDRAM_MODEREG_OPERATING_MODE_STANDARD |
					SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	/* Step 5 --------------------------------------------------------------------*/
	/* Configure a load Mode register command*/
	FMC_SDRAMCommandStructure.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	FMC_SDRAMCommandStructure.AutoRefreshNumber = 1;
	FMC_SDRAMCommandStructure.ModeRegisterDefinition = tmpr;
	while( (HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY) );	// Check Busy Flag before sending command
	if (HAL_SDRAM_SendCommand(&hsdram1, &FMC_SDRAMCommandStructure, SDRAM_TIMEOUT) != HAL_OK)
		Error_Handler();

	/* Step 6 --------------------------------------------------------------------*/
	/* Set the refresh rate counter */
	/* (7.81 us x Freq) - 20 */
	/* Set the device refresh counter */
	while( (HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY) );	// Check Busy Flag before sending command
	HAL_SDRAM_ProgramRefreshRate(&hsdram1, REFRESH_COUNT); // local

	while( (HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY) );	// Check Busy Flag before returning
}
