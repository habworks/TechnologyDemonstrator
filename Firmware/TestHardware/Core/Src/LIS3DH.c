/*
 * LIS3DH.c
 *
 *  Created on: Feb 23, 2021
 *      Author: IMR HabPC
 */

#include "LIS3DH.h"
#include "spi.h"
#include "math.h"


bool_t isAccelerometerOnLine(void)
{

	uint8_t RegisterAddress = LIS3DH_REGISTER_WHO_AM_I;
	uint8_t WhoAmI_Response = 22;
	HAL_StatusTypeDef CallStatus;

	LIS3DH_CS_ENABLE();
	RegisterAddress |= LIS3DH_READ_OPERATION;
	CallStatus = HAL_SPI_Transmit(&hspi1, &RegisterAddress, 1, LIS3DH_SPI_BLOCK_TIMEOUT);
	if (CallStatus != HAL_OK)
		return(FALSE);
	CallStatus = HAL_SPI_Receive(&hspi1, &WhoAmI_Response, 1, LIS3DH_SPI_BLOCK_TIMEOUT);
	LIS3DH_CS_DISABLE();
	if ((WhoAmI_Response != LIS3DH_WHO_AM_I_VALUE) || (CallStatus != HAL_OK))
		return(FALSE);
	else
		return(TRUE);

} // END OF isAccelerometerOnLine


bool_t readAccelerometerRegister(uint8_t RegisterAddress, uint8_t *ReadValue, uint8_t BytesToRead)
{

	HAL_StatusTypeDef CallStatus;

	RegisterAddress |= LIS3DH_READ_OPERATION;
	LIS3DH_CS_ENABLE();
	CallStatus = HAL_SPI_Transmit(&hspi1, &RegisterAddress, 1, LIS3DH_SPI_BLOCK_TIMEOUT);
	if (CallStatus != HAL_OK)
		return(FALSE);
	CallStatus = HAL_SPI_Receive(&hspi1, ReadValue, BytesToRead, LIS3DH_SPI_BLOCK_TIMEOUT);
	LIS3DH_CS_DISABLE();
	if (CallStatus != HAL_OK)
		return(FALSE);
	else
		return(TRUE);

}


bool_t writeAccelerometerRegister(uint8_t *WriteBuffer, uint8_t BufferLength)
{

	HAL_StatusTypeDef CallStatus;

	LIS3DH_CS_ENABLE();
	CallStatus = HAL_SPI_Transmit(&hspi1, WriteBuffer, BufferLength, LIS3DH_SPI_BLOCK_TIMEOUT);
	LIS3DH_CS_DISABLE();
	if (CallStatus != HAL_OK)
		return(FALSE);
	else
		return(TRUE);

}

// HAB NEED ANOTHER FUNCTION TO CALCULATE ANGLES IN 3D
// SEE THIS LINK: https://www.analog.com/media/en/technical-documentation/application-notes/AN-1057.pdf
float computeAcelerometerG(uint8_t LowByte, uint8_t HighByte, uint8_t BitResolution, uint8_t FullScale)
{
	uint16_t TwosCompliment;
	int16_t G_Count;
	float G_Value;

	// LOAD BYTE TO 16BIT REGISTER
	TwosCompliment = HighByte;
	TwosCompliment <<= 8;
	TwosCompliment |= LowByte;
	// REMOVE LEFT JUSTIFY FOR 16B VALUE IF 8 YOUR HAVE TO REMOVE 8 ZEROS ON RIGHT, IF 10 YOUR REMOVE 6 ZEROS, IF 12 YOU REMOVE 4 ZEROS
	TwosCompliment >>= (sizeof(uint16_t) * 8) - BitResolution;

	// G_Count is the 2's compliment removed to a +/- Count for the g value
	G_Count = convertTwoCompliment(TwosCompliment, 10);
	G_Value = (float)(G_Count * (FullScale / pow(2, BitResolution)));
	return(G_Value);
}


int16_t convertTwoCompliment(uint16_t TwosValue, uint8_t NumberOfBits)
{
	uint32_t Range = pow(2, NumberOfBits);

	if (TwosValue < (Range / 2))
	{
		// Value is positive
		return((int16_t)TwosValue);
	}
	else
	{
		// Value is negative
		int16_t SignedNumber16b;
		int32_t SignedNumber32b;
		uint16_t TwosValueNegative = TwosValue;
		TwosValueNegative = ~TwosValueNegative;
		switch(NumberOfBits)
		{
		case 8:
			TwosValueNegative &= 0x00FF;
			break;

		case 10:
			TwosValueNegative &= 0x03FF;
			break;

		case 12:
			TwosValueNegative &= 0x0FFF;
			break;

		default:
			break;		// Hab fix Good place for test and return
		}
		TwosValueNegative++;
		SignedNumber32b = TwosValueNegative;
		SignedNumber32b *= -1;
		SignedNumber16b = (int16_t)SignedNumber32b;
		return(SignedNumber16b);
	}

}

// hab this does not measure room temperature but only temp variations - not useful to me discard later
uint16_t temperatureAccelerometer(void)
{

	uint8_t ADC_TempLowByte;
	uint8_t ADC_TempHighByte;
	uint16_t ADC_Count;
	uint8_t BitResolution = 10;

	// ADC IS 10bit:
	readAccelerometerRegister(LIS3DH_REGISTER_OUT_ADC3_L, &ADC_TempLowByte, sizeof(ADC_TempLowByte));
	readAccelerometerRegister(LIS3DH_REGISTER_OUT_ADC3_H, &ADC_TempHighByte, sizeof(ADC_TempLowByte));
	ADC_Count = ADC_TempHighByte;
	ADC_Count <<= 8;
	ADC_Count |= ADC_TempLowByte;
	// REMOVE LEFT JUSTIFY FOR 16B VALUE IF 8 YOUR HAVE TO REMOVE 8 ZEROS ON RIGHT, IF 10 YOUR REMOVE 6 ZEROS, IF 12 YOU REMOVE 4 ZEROS
	ADC_Count >>= (sizeof(uint16_t) * 8) - BitResolution;

	ADC_Count = convertTwoCompliment( ADC_Count,  10);
	return(ADC_Count);
}
