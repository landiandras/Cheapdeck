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
#include <string.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include "Flash.h"

enum EventType{
	wait,
	down,
	up
};

typedef struct {
	uint8_t type;
	uint16_t value;
}KeyboardEvent;


uint16_t GetKeys();
void Debounce();
void ReadColumn(uint8_t col);
void SetColumn(uint8_t col);
void ScanButtons();
void WriteMacrosToFlash();


#endif /* INC_BUTTONS_H_ */
