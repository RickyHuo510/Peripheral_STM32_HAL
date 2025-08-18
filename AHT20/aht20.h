/*
 * aht20.h
 *
 *  Created on: Jul 30, 2024
 *      Author: RickyHuo
 */

#ifndef INC_AHT20_H_
#define INC_AHT20_H_

#include "i2c.h"


void AHT20_Init();
void AHT20_read(float*,float*);



#endif /* INC_AHT20_H_ */
