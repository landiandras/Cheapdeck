/*
 * buttons.h
 *
 *  Created on: Feb 28, 2026
 *      Author: Landi András
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "stm32f4xx_hal.h"


uint16_t GetKeys();
void Debounce();
void ReadColumnBitwise(uint8_t col);
void SetColumn(uint8_t col);
void ScanButtonsBitwise();


#endif /* INC_BUTTONS_H_ */
