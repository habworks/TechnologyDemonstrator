/*
 * TLP0401A.h
 *
 *  Created on: Feb 22, 2021
 *      Author: IMR HabPC
 */

#include "MainSupport.h"

#ifndef INC_TLP0401A_H_
#define INC_TLP0401A_H_

#define TPL0401A_ADDRESS			((uint8_t)(0x5C))
#define TPL0401A_REGISTER_RW		((uint8_t)(0x00))
#define TPL0401A_TOTAL_STEPS 		((uint8_t)(0x80))
#define TPL0401A_RW_TIMEOUT_DELAY	(10U)


bool_t setDigitalPotAttenuation(float PercentValue);


#endif /* INC_TLP0401A_H_ */
