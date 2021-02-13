/*
 * TD_SDRAM.h
 *
 *  Created on: Feb 9, 2021
 *      Author: IMR HabPC
 */

#ifndef APP_INC_TD_SDRAM_H_
#define APP_INC_TD_SDRAM_H_
#ifdef __cplusplus
extern"C" {
#endif


// DEFINES
#define REFRESH_COUNT 							1262U
#define BUFFER_SIZE         					((uint32_t)0x1000)
#define SDRAM_BANK_ADDR                 		((uint32_t)0xC0000000)  // SDRAM USES EXTERNAL MEMORY BANK 5 MAPPED TO THIS ADDRESS
#define WRITE_READ_ADDR     					((uint32_t)0x0800)
#define SDRAM_TIMEOUT     						((uint32_t)0xFFFF)
#define SDRAM_MODEREG_BURST_LENGTH_1         	((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2            ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4            ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8            ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL     ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED    ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2             ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3             ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD   ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE    ((uint16_t)0x0200)

// FUNCTION PROTOTYPES
void SDRAM_InitSequence(void);


#ifdef __cplusplus
}
#endif
#endif /* APP_INC_TD_SDRAM_H_ */
