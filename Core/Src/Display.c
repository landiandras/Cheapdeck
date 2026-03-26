/*
 * Display.c
 *
 *  Created on: Mar 1, 2026
 *      Author: Landi András
 */

#include "Display.h"

uint8_t Frame[8][128] = {0};
extern TIM_HandleTypeDef htim2;

void SetBrightness(uint8_t brightness){
	if(brightness > 100) brightness = 100;
	TIM2->CCR4 = brightness;
}

void SendCommand(uint8_t command){
	HAL_GPIO_WritePin(DIS_A0_GPIO_Port, DIS_A0_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &command, 1, HAL_MAX_DELAY);
}

void SendData(uint8_t* data, uint8_t size){
	HAL_GPIO_WritePin(DIS_A0_GPIO_Port, DIS_A0_Pin, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi1, data, size, HAL_MAX_DELAY);
}

void SelectColumn(uint8_t col){
	if(col>127) return;
	uint8_t low = col&0x0F;
	uint8_t high = col&0xF0>>4;
	SendCommand(0x10|high);
	SendCommand(0x00|low);
}

void SelectRow(uint8_t row){
	if(row>63) return;
	uint8_t page = row/8;
	SendCommand(0xB0|page);
}

void SelectPage(uint8_t page){
	if(page>7) return;
	SendCommand(0xB0|page);
}

void InitDisplay(){
	HAL_GPIO_WritePin(DIS_A0_GPIO_Port, DIS_A0_Pin, GPIO_PIN_RESET);

	//RES pin LOW
	HAL_GPIO_WritePin(DIS_RES_GPIO_Port, DIS_RES_Pin, GPIO_PIN_RESET);
	HAL_Delay(20);

	//RES pin HIGH
	HAL_GPIO_WritePin(DIS_RES_GPIO_Port, DIS_RES_Pin, GPIO_PIN_SET);
	HAL_Delay(1);

	HAL_GPIO_WritePin(DIS_CS_GPIO_Port, DIS_CS_Pin, GPIO_PIN_RESET);

	//LCD BIAS setting
	SendCommand(0xA2);

	//ADC Selection
	SendCommand(0xA0);

	//Common output mode selection
	SendCommand(0xC0);

	//Display Star Line: 0
	SendCommand(0x40);

	//TODO: maybe break this into 3 pieces?
	//Set internal V0 regulator resistor ratio for V0 voltage
	SendCommand(0x2F);

	//Electronic Volume Mode, set in the middle for now, calibrate later.
	SendCommand(0x81);
	SendCommand(0x20);

	//V0 Voltage Regulator Internal Resistor Ratio Set
	SendCommand(0x24);

	//Booster ratio set
	SendCommand(0xF8);
	SendCommand(0x00);

	//Display ON
	SendCommand(0xAF);

	//Clear the display
	//TODO: use memcpy instead, presumably pointless either way, Frame is initialised with zeros but what the hell
	for(uint8_t i = 0; i<8; ++i){
		for(uint8_t j = 0; j<128; j++){
			Frame[i][j] = 0;
		}
	}
	PaintDisplay();

}

void WriteBlock(uint8_t data, uint8_t col, uint8_t page){
	if(col>127 || page > 7) return;
	SelectPage(page);
	SelectColumn(col);
	//SendData(data);
}

void PaintDisplay(){

	SelectColumn(0);
	for(uint8_t i = 0; i<8; ++i){
		SelectPage(i);
		SendData(Frame[i], 128);
		SelectColumn(0);
	}
}

