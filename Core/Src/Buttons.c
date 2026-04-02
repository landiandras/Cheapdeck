/*
 * Buttons.c
 *
 *  Created on: Feb 28, 2026
 *      Author: Landi András
 *      #define Col0_Pin GPIO_PIN_0
 *		#define Col0_GPIO_Port GPIOB
 */



#include <Buttons.h>

const uint8_t BounceDuration = 5;
const uint8_t numberofcolumns = 4;
const uint8_t numberofrows = 3;
const GPIO_Pin columns[]={
		{Col0_GPIO_Port, Col0_Pin},
		{Col1_GPIO_Port, Col1_Pin},
		{Col2_GPIO_Port, Col2_Pin},
		{Col3_GPIO_Port, Col3_Pin}
};
const GPIO_Pin rows[]={
		{Row0_GPIO_Port, Row0_Pin},
		{Row1_GPIO_Port, Row1_Pin},
		{Row2_GPIO_Port, Row2_Pin}
};



uint16_t oldkeys = 0;
uint16_t newkeys = 0;
uint16_t lockouts[5] = {0};

uint32_t ButtonsLockout[12] = {0};
bool buttons[12] = {0};

void ReadColumnBitwise(uint8_t col){
	uint32_t temp = 0;
	int8_t shift = 4 - (col * 3);
	temp = ((GPIOB->IDR) & 0x0070);
	if(shift >= 0) temp = temp >> shift;
	else temp = temp << -shift;
	newkeys |= temp;
}

bool GetButtonState(uint8_t but){
	if(but > (numberofcolumns * numberofrows)) return false;
	return (buttons[but]);
}

void ButtonPressed(uint8_t but){
	if(but > (numberofcolumns * numberofrows)) return;
	if(buttons[but] == BounceDuration) return;
	buttons[but] = true;
}

void ButtonDePressed(uint8_t but) {
	if(but > (numberofcolumns * numberofrows)) return;
	if(buttons[but] == 0) return;
	buttons[but] = false;
}

void SetColumn(uint8_t col){
	for(uint8_t i = 0; i<numberofcolumns; ++i){
		if(i!=col) HAL_GPIO_WritePin(columns[i].port, columns[i].pin, GPIO_PIN_SET);
		else HAL_GPIO_WritePin(columns[i].port, columns[i].pin, GPIO_PIN_RESET);
	}
}

void ReadColumn(uint8_t col){
	for(uint8_t i = 0; i<numberofrows; ++i){
		if(HAL_GetTick() - ButtonsLockout[i+(col*numberofrows)] > BounceDuration){
			if(HAL_GPIO_ReadPin(rows[i].port, rows[i].pin) == GPIO_PIN_RESET) ButtonPressed(col+(numberofcolumns*i));
			else ButtonDePressed(col+(numberofcolumns*i));
			ButtonsLockout[i+(col*numberofrows)] = HAL_GetTick();
		}
	}
}

void ScanButtonsBitwise(){
	oldkeys = newkeys;
	newkeys = 0;
	for(uint8_t i = 0; i<numberofcolumns; ++i){
		SetColumn(i);
		ReadColumnBitwise(i);
	}
	uint16_t changedkeys = oldkeys^newkeys;
	if(changedkeys){
		for(uint8_t i = 0; i<numberofcolumns*numberofrows; ++i){
		}
	}
}

void ScanButtons(){
	for(uint8_t i = 0; i<numberofcolumns; ++i){
		SetColumn(i);
		ReadColumn(i);
	}
}
