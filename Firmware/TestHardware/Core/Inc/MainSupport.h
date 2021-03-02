/*
 * MainSupport.h
 *
 *  Created on: Jan 15, 2021
 *      Author: IMR HabPC
 */

#ifndef INC_MAINSUPPORT_H_
#define INC_MAINSUPPORT_H_

#include "main.h"
#include "stdint.h"


#define PRINT_TIME_DELAY					10U			// Value in miliseconds
// ADC STUFF
#define NUMBER_OF_ADC_CHANNELS				3U
#define ADC_SAMPLES_PER_ACQUISION			5U
#define ADC_DMA_SIZE 						(NUMBER_OF_ADC_CHANNELS * ADC_SAMPLES_PER_ACQUISION)
#define BAT_DIVIDER_RATIO					(100.0/(100.0 + 40.2))
#define VADC_REF							3.3
// FMC - SDRAM STUFF
#define BUFFER_SIZE         				((uint32_t)0x1000)
#define SDRAM_BANK_ADDR                 	((uint32_t)0xC0000000)  // SDRAM USES EXTERNAL MEMORY BANK 5 MAPPED TO THIS ADDRESS
#define WRITE_READ_ADDR     				((uint32_t)0x0800)
#define SDRAM_TIMEOUT     					((uint32_t)0xFFFF)
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

#define INTEGER_ROUNDING							(0.5)
// TYPEDEFS AND ENUMS
typedef _Bool bool;

#ifndef bool_t
	#ifdef FALSE
		#undef FALSE
	#endif
	#ifdef TRUE
		#undef TRUE
	#endif
typedef enum
{
	FALSE = 0,
	TRUE
}bool_t;
#endif

typedef struct
{
	uint32_t	StatusLedMsCounter;
	uint32_t	RedLedMsCounter;
	uint32_t	GreenLedMsCounter;
	uint32_t	MiliSecondCounter;
	uint8_t		UartCmdChar;
	uint32_t	ADC_UiReadSum;
	uint32_t	ADC_BatReadSum;
}Type_BoardStatus;

extern volatile Type_BoardStatus BoardStatus;

// PROTOTYPE FUNCTIONS
void main_Init(void);
void main_WhileLoop(void);
void printHelpMenu(void);
// SDRAM
void SDRAM_InitSequence(void);
void fillBufferWalkBit(uint32_t *Buffer, uint32_t SizeOfBuffer);


#endif /* INC_MAINSUPPORT_H_ */
