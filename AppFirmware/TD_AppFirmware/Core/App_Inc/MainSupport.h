/*
 * MainSupport.h
 *
 *  Created on: Feb 9, 2021
 *      Author: IMR HabPC
 */

#ifndef APP_INC_MAINSUPPORT_H_
#define APP_INC_MAINSUPPORT_H_
#ifdef __cplusplus
extern"C" {
#endif

#include "main.h"
#include "stdint.h"

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


// FUNCTION PROTOTYPES
void main_Init(void);
void main_WhileLoop(void);


#ifdef __cplusplus
}
#endif
#endif /* APP_INC_MAINSUPPORT_H_ */
