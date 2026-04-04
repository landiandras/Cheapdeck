/*
 * Display.c
 *
 *  Created on: Mar 1, 2026
 *      Author: Landi András
 */

#include "Display.h"

extern uint8_t Frame[8][128];
__attribute__((aligned(4))) uint8_t Buffer[8][128];
uint8_t FrameBlockCounter;
extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
bool istransmitting = false;
extern bool drawingallowed;

void SetBrightness(uint8_t brightness){
	if(brightness > 100) brightness = 100;
	TIM2->CCR4 = brightness;
}

void ChangeBrightness(int8_t brightness){
	if(brightness > 0 && TIM2->CCR4 + brightness > 100) TIM2->CCR4 = 100;
	else if (brightness < 0 && TIM2->CCR4 + brightness > 100) TIM2->CCR4 = 0;
	else TIM2->CCR4 +=brightness;
}

void SendCommandBlocking(uint8_t command){
	HAL_GPIO_WritePin(DIS_A0_GPIO_Port, DIS_A0_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &command, 1, HAL_MAX_DELAY);
}

void SendDataBlocking(uint8_t* data, uint8_t size){
	HAL_GPIO_WritePin(DIS_A0_GPIO_Port, DIS_A0_Pin, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi1, data, size, HAL_MAX_DELAY);
}

void SendDataDMA(uint8_t* data, uint8_t size){
	HAL_GPIO_WritePin(DIS_A0_GPIO_Port, DIS_A0_Pin, GPIO_PIN_SET);
	HAL_SPI_Transmit_DMA(&hspi1, data, size);
}

void SelectColumn(uint8_t col){
	if(col>127) return;
	uint8_t low = col&0x0F;
	uint8_t high = (col&0xF0)>>4;
	SendCommandBlocking(0x10|high);
	SendCommandBlocking(0x00|low);
}

void SelectRow(uint8_t row){
	if(row>63) return;
	uint8_t page = row/8;
	SendCommandBlocking(0xB0|page);
}

void SelectPage(uint8_t page){
	if(page>7) return;
	SendCommandBlocking(0xB0|page);
}

void InitDisplay(){
    hdma_memtomem_dma2_stream0.XferCpltCallback = DMA_CpltCallback;

	HAL_GPIO_WritePin(DIS_A0_GPIO_Port, DIS_A0_Pin, GPIO_PIN_RESET);

	//RES pin LOW
	HAL_GPIO_WritePin(DIS_RES_GPIO_Port, DIS_RES_Pin, GPIO_PIN_RESET);
	HAL_Delay(20);

	//RES pin HIGH
	HAL_GPIO_WritePin(DIS_RES_GPIO_Port, DIS_RES_Pin, GPIO_PIN_SET);
	HAL_Delay(1);

	HAL_GPIO_WritePin(DIS_CS_GPIO_Port, DIS_CS_Pin, GPIO_PIN_RESET);

	//LCD BIAS setting
	SendCommandBlocking(0xA2);

	//ADC Selection
	SendCommandBlocking(0xA0);

	//Common output mode selection
	SendCommandBlocking(0xC0);

	//Display Star Line: 0
	SendCommandBlocking(0x40);

	//TODO: maybe break this into 3 pieces?
	//Set internal V0 regulator resistor ratio for V0 voltage
	SendCommandBlocking(0x2F);

	//Electronic Volume Mode, set in the middle for now, calibrate later.
	SendCommandBlocking(0x81);
	SendCommandBlocking(0x20);

	//V0 Voltage Regulator Internal Resistor Ratio Set
	SendCommandBlocking(0x24);

	//Booster ratio set
	SendCommandBlocking(0xF8);
	SendCommandBlocking(0x00);

	//enable backlight
	SetBrightness(30);

	//Display ON
	SendCommandBlocking(0xAF);

	//Clear the display
	ClearDisplay();
	PaintDisplayBlocking();

}

void ClearDisplay(){
	memset(Frame, 0, sizeof(Frame));
}


void PaintDisplayBlocking(){
	SelectColumn(0);
	for(uint8_t i = 0; i<8; ++i){
		SelectPage(i);
		SendDataBlocking(Frame[i], 128);
		SelectColumn(0);
	}
}

void PaintDisplayDMA(){
	if(istransmitting) return;
	istransmitting = true;
	drawingallowed = false;
	HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream0, (uint32_t)Frame, (uint32_t)Buffer, 256);

}

//Removed most of the function calls to speed things up
void DMA_CpltCallback(DMA_HandleTypeDef *dma){
	if(dma == NULL) return;
	if(dma == &hdma_memtomem_dma2_stream0){
		drawingallowed = true;
		uint8_t command[] = {0x10, 0x00};
		DIS_A0_GPIO_Port->BSRR = DIS_A0_Pin<<16;
		HAL_SPI_Transmit(&hspi1, command, 2, HAL_MAX_DELAY);
		FrameBlockCounter = 1;
		DIS_A0_GPIO_Port->BSRR = DIS_A0_Pin;
		HAL_SPI_Transmit_DMA(&hspi1, Buffer[0], 128);
	}
}


//Removed most of the function calls to speed things up
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi){
	if(hspi == NULL) return;
	if(FrameBlockCounter >= 8) {
		FrameBlockCounter = 0;
		istransmitting = false;
		return;
	}

	uint8_t command[] = {0xB0|FrameBlockCounter, 0x00, 0x10}; //set page to FrameBlockCounter and reset column to 0
	DIS_A0_GPIO_Port->BSRR = DIS_A0_Pin<<16;
	HAL_SPI_Transmit(&hspi1, command, 3, HAL_MAX_DELAY);
	DIS_A0_GPIO_Port->BSRR = DIS_A0_Pin;
	HAL_SPI_Transmit_DMA(&hspi1, Buffer[FrameBlockCounter++], 128);

}
