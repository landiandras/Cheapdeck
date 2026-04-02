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

typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
} GPIO_Pin;


void ReadColumnBitwise(uint8_t col);
bool GetButtonState(uint8_t but);
void ButtonPressed(uint8_t but);
void ButtonDePressed(uint8_t but);
void SetColumn(uint8_t col);
void ReadColumn(uint8_t col);
void ScanButtonsBitwise();
void ScanButtons();


#endif /* INC_BUTTONS_H_ */
