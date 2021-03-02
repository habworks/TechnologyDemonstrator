/*
 * AdcSupport.h
 *
 *  Created on: Jan 17, 2021
 *      Author: IMR HabPC
 */

#ifndef INC_ADCSUPPORT_H_
#define INC_ADCSUPPORT_H_

#include "MainSupport.h"

#define ADC_DEFINED_FOR_USE
#define DAC_FULL_SCALE			((uint16_t)0x1000)
#define DAC_REF_VOLTAGE			((float)3.300)


// PROTOTYPE FUNCTIONS
uint16_t ADC_FullScale(void);
bool_t DAC_SetVoltage(float OutputVoltage);



#endif /* INC_ADCSUPPORT_H_ */
