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


void SetBrightness(uint8_t brightness);
void SendCommandBlocking(uint8_t command);
void SendDataBlocking(uint8_t* data, uint8_t size);
void SendDataDMA(uint8_t* data, uint8_t size);
void SelectColumn(uint8_t col);
void SelectRow(uint8_t row);
void SelectPage(uint8_t page);

void InitDisplay();

void ClearDisplay();
void PaintDisplayBlocking();
void PaintDisplayDMA();
extern void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);
void DMA_CpltCallback(DMA_HandleTypeDef *dma);



#endif /* INC_DISPLAY_H_ */
