/*
 * HM01B0_FUNC.h
 *
 *  Created on: Nov 15, 2018
 *      Author: Ali
 */

#ifndef HM01B0_FUNC_H_
#define HM01B0_FUNC_H_

#include <stdbool.h>

#include "HM01B0_I2C.h"
#include "HM01B0Regs.h"

//MACROS----------------------------------------------------------------------------------------------------------------------------//
#define BITSET(port,pin)    port |= (pin)
#define BITCLR(port,pin)    port &= ~(pin)
#define BITTOG(port,pin)    port ^= (pin)

//FUNCTIONS----------------------------------------------------------------------------------------------------------------------------//

void hm01b0_init();



#endif /* HM01B0_FUNC_H_ */
