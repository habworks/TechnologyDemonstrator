/*
 * Time.h
 *
 *  Created on: Jan 16, 2021
 *      Author: IMR HabPC
 */

#ifndef INC_TIME_H_
#define INC_TIME_H_

#include "MainSupport.h"
#include "tim.h"

// DEFINES
#define FULL_TIMER_CLOCK_RATE			((uint32_t)(100000000))

// FUNCTION PROTOTYPES
void delayMiliSecond(uint32_t DelayTime);
bool_t init_AudioTimer(uint32_t InteruptFrequency);

#endif /* INC_TIME_H_ */
