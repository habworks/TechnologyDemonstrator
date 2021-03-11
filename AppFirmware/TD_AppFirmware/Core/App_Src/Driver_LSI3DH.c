/** ****************************************************************************************************
 * @file 			PWM_AccelerometerTaskSupport.c
 * @brief			This is the Header file provides driver api functions for chip LSI3DH
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
 * @note            This file provides the driver API for chip LSI3DH - 3 Axis Accelerometer
 * 					The chip is interface on SPI1
 *
 *
 * 					This is an embedded application
 *		            It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *			          IMR Technology Demonstrator Schematic: IMR002S REV 1
 *				      IMR Technology Demonstrator Schematic: IMR002S REV 1
 *
 * @copyright		IMR Engineering
 * **************************************************************************************************** */

#include "Driver_LSI3DH.h"
#include "spi.h"
#include "math.h"


/*******************************************************************************************************
* @brief Performs an access of the LIS3DH_REGISTER_WHO_AM_I in search of a known response.  A read to said
* register is performed by writting the register value then reading the response.  If the IC is operational
* and online it will respond with its ID, if not response will not match
*
* @date				3/9/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note LIS3DH connected via SPI1
*
* @param void
* @return True or False
*
* STEP 1: Perform Register Access for reading
* STEP 2: Read the register contents, compare and return
****************************************************************************************************** */
bool_t isAccelerometerOnLine(void)
{
	uint8_t RegisterAddress = LIS3DH_REGISTER_WHO_AM_I;
	uint8_t WhoAmI_Response = 22;
	HAL_StatusTypeDef CallStatus;

	// STEP 1: Perform Register Access for reading
	LIS3DH_CS_ENABLE();
	RegisterAddress |= LIS3DH_READ_OPERATION;
	CallStatus = HAL_SPI_Transmit(&hspi1, &RegisterAddress, 1, LIS3DH_SPI_BLOCK_TIMEOUT);
	if (CallStatus != HAL_OK)
		return(FALSE);

	// STEP 2: Read the register contents, compare and return
	CallStatus = HAL_SPI_Receive(&hspi1, &WhoAmI_Response, 1, LIS3DH_SPI_BLOCK_TIMEOUT);
	LIS3DH_CS_DISABLE();
	if ((WhoAmI_Response != LIS3DH_WHO_AM_I_VALUE) || (CallStatus != HAL_OK))
		return(FALSE);
	else
		return(TRUE);
} // END OF isAccelerometerOnLine



/*******************************************************************************************************
* @brief Read of LIS3DH register value.  Register is read and return by pointer
*
* @date				3/9/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note LIS3DH connected via SPI1
*
* @param Register Address, Pointer to Value Read, Number of bytes to read
* @return True or False on read success
*
* STEP 1: Perform Register Access for reading
* STEP 2: Read the register contents and return
****************************************************************************************************** */
bool_t readAccelerometerRegister(uint8_t RegisterAddress, uint8_t *ReadValue, uint8_t BytesToRead)
{
	HAL_StatusTypeDef CallStatus;

	// STEP 1: Perform Register Access for reading
	RegisterAddress |= LIS3DH_READ_OPERATION;
	LIS3DH_CS_ENABLE();
	CallStatus = HAL_SPI_Transmit(&hspi1, &RegisterAddress, 1, LIS3DH_SPI_BLOCK_TIMEOUT);
	if (CallStatus != HAL_OK)
		return(FALSE);

	//STEP 2: Read the register contents and return
	CallStatus = HAL_SPI_Receive(&hspi1, ReadValue, BytesToRead, LIS3DH_SPI_BLOCK_TIMEOUT);
	LIS3DH_CS_DISABLE();
	if (CallStatus != HAL_OK)
		return(FALSE);
	else
		return(TRUE);
} // END OF readAccelerometerRegister



/*******************************************************************************************************
* @brief Write buffer to register.  The buffer contains the data to be written with the register address
* as the starting index of the buffer
*
* @date				3/9/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note LIS3DH connected via SPI1
*
* @param Write buffer contents, Number of bytes to write
* @return True or False on write success
*
* STEP 1: Transmit buffer contents return success status
****************************************************************************************************** */
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

} // END OF writeAccelerometerRegister


/*******************************************************************************************************
* @brief Calculates the individual accelerometer g per axis.  This function intended for g acceleration
* when single axis is being rotated.  It accepts the parameters for a single axis and computes the g
*
* @date				3/9/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note LIS3DH connected via SPI1
* @note Low Byte / High Byte when combined is 2's compliment left justified.  Bytes form 16b number but is left justified based on resolution
* @note SEE THIS LINK: https://www.analog.com/media/en/technical-documentation/application-notes/AN-1057.pdf
*
* @param Axis Low Byte, Axis High Byte, Resolution of conversion, Full Scale g setting(+/-): 2, 4, 8, 16
* @return Accelerometer g as a float value - for non impact max min is +/- 1g
*
* STEP 1: Join Low and High byte to form 2's Compliment - note it is is left justified based on resolution
* STEP 2: Convert 2's compliment value then scale based on Full g setting and bit resolution
****************************************************************************************************** */
float computeAcelerometerG(uint8_t LowByte, uint8_t HighByte, uint8_t BitResolution, uint8_t FullScale)
{
	uint16_t TwosCompliment;
	int16_t G_Count;
	float G_Value;

	// STEP 1: Join Low and High byte to form 2's Compliment - note it is is left justified based on resolution
	// LOAD BYTE TO 16BIT REGISTER
	TwosCompliment = HighByte;
	TwosCompliment <<= 8;
	TwosCompliment |= LowByte;
	// REMOVE LEFT JUSTIFY FOR 16B VALUE IF 8 YOUR HAVE TO REMOVE 8 ZEROS ON RIGHT, IF 10 YOUR REMOVE 6 ZEROS, IF 12 YOU REMOVE 4 ZEROS
	TwosCompliment >>= (sizeof(uint16_t) * 8) - BitResolution;

	// STEP 2: Convert 2's compliment value then scale based on Full g setting and bit resolution
	// G_Count is the 2's compliment removed to a +/- Count for the g value
	G_Count = convertTwoCompliment(TwosCompliment, 10);
	G_Value = (float)(G_Count * (FullScale / pow(2, BitResolution)));
	return(G_Value);
} // END OF computeAcelerometerG



/*******************************************************************************************************
* @brief Converts 2's a  number to a 16b signed integer.  Note function can accept various bit sizes
*
* @date				3/9/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Only tested on 16b 2's compliment numbers
*
* @param 2's Compliment Value, Size of value in bits
* @return Signed integer conversion value
*
* STEP 1: Calculate range and determine if a positive or negative value - if positive return with that
* STEP 2: For negative value: Invert, Mask Based on the number of bits, Add 1 and return value
****************************************************************************************************** */
int16_t convertTwoCompliment(uint16_t TwosValue, uint8_t NumberOfBits)
{
	// STEP 1: Calculate range and determine if a positive or negative value - if positive return with that
	uint32_t Range = pow(2, NumberOfBits);
	if (TwosValue < (Range / 2))
	{
		// Value is positive
		return((int16_t)TwosValue);
	}
	else
	{
		// Value is negative
		// STEP 2: For negative value: Invert, Mask Based on the number of bits, Add 1 and return value
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
			break;		// TODO : Hab make better this should be a fault or return false
		}
		TwosValueNegative++;
		SignedNumber32b = TwosValueNegative;
		SignedNumber32b *= -1;
		SignedNumber16b = (int16_t)SignedNumber32b;
		return(SignedNumber16b);
	}
} // END OF convertTwoCompliment



// HAB NEED ANOTHER FUNCTION TO CALCULATE ANGLES IN 3D
// SEE THIS LINK: https://www.analog.com/media/en/technical-documentation/application-notes/AN-1057.pdf



bool_t computeThreeAxisAngularMeaure(const Type_AccelerometerG *AccelerometerG, Type_AccelerometerAngle *AccelerometerAngle)
{
	float TempResult;
	float AnsInRad;

	// STEP 1: Calculate Angle x
	TempResult = sqrt(pow(AccelerometerG->Acel_Y, 2.0) + pow(AccelerometerG->Acel_Z, 2.0));
	AnsInRad = (float)atan(AccelerometerG->Acel_X / TempResult);
	AccelerometerAngle->X_Degrees = AnsInRad * 180.0 / M_PI;

	// STEP 2: Calculate Angle y
	TempResult = sqrt(pow(AccelerometerG->Acel_X, 2.0) + pow(AccelerometerG->Acel_Z, 2.0));
	AnsInRad = (float)atan(AccelerometerG->Acel_Y / TempResult);
	AccelerometerAngle->Y_Degrees = AnsInRad * 180.0 / M_PI;

	// STEP 3: Calculate Angle z
	TempResult = sqrt(pow(AccelerometerG->Acel_X, 2.0) + pow(AccelerometerG->Acel_Y, 2.0));
	AnsInRad = (float)atan(TempResult / AccelerometerG->Acel_Z );
	AccelerometerAngle->Z_Degrees = AnsInRad * 180.0 / M_PI;

	return(TRUE);
} // END OF computeThreeAxisAngularMeaure

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



