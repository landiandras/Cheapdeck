/*
 * Display.h
 *
 *  Created on: Mar 1, 2026
 *      Author: Landi András
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "stm32f4xx_hal.h"

extern SPI_HandleTypeDef hspi1;

void SetBrightness(uint8_t brightness);
void SendCommand(uint8_t command);
void SendData(uint8_t* data, uint8_t size);
void SelectColumn(uint8_t col);
void SelectRow(uint8_t row);
void SelectPage(uint8_t page);

void InitDisplay();

void WriteBlock(uint8_t data, uint8_t col, uint8_t block);
void PaintDisplay();



#endif /* INC_DISPLAY_H_ */
