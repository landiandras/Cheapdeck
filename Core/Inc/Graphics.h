/*
 * Graphics.h
 *
 *  Created on: Apr 2, 2026
 *      Author: Landi András
 */

#ifndef INC_GRAPHICS_H_
#define INC_GRAPHICS_H_

#include <math.h>
#include "main.h"

void WriteCharacter(uint8_t Frame[8][128], char c, uint8_t page, uint8_t column);
void WriteString(uint8_t Frame[8][128], char* c, uint8_t page, uint8_t column);
void WriteLine(uint8_t Frame[8][128], char* c, uint8_t page);
void UpdateDisplayForProgramming(uint8_t key_index, uint8_t modifiers, char* macro_string);
void UpdateDisplayForKeyPress(uint8_t modifiers, uint16_t pressed_keys);

#endif /* INC_GRAPHICS_H_ */
