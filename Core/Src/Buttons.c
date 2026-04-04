/*
 * Buttons.c
 *
 *  Created on: Feb 28, 2026
 *      Author: Landi András
 *
 */



#include <Buttons.h>

const uint8_t BounceDuration = 5;
const uint8_t numberofcolumns = 4;
const uint8_t numberofrows = 3;


uint16_t newkeys = 0;
extern uint16_t changes;
uint32_t ButtonsLockout[12] = {0};





void SetColumn(uint8_t col){
	Col0_GPIO_Port->BSRR = 15U;					//set all column GPIO to 1 (unselected)
	Col0_GPIO_Port->BSRR = 1U << (col+16);		//set the correct column to 0 (selected)
}

void ReadColumnBitwise(uint8_t col){
	uint32_t temp = 0;
	int8_t shift = 4 - (col * 3);
	temp = ((GPIOB->IDR) & 0x0070);
	if(shift >= 0) temp = temp >> shift;
	else temp = temp << -shift;
	newkeys |= temp;
}


void ScanButtonsBitwise(){
	uint16_t oldkeys = newkeys;
	newkeys = 0;
	for(uint8_t i = 0; i<numberofcolumns; ++i){
		SetColumn(i);
		ReadColumnBitwise(i);
	}
	newkeys ^= 0x0FFF;								//invert the logic here so that 1 means pressed
	changes = oldkeys^newkeys;						//check for changes
	if(changes) Debounce();							//confirm changes (debounce)
}

void Debounce(){
	for(uint8_t i = 0; i< numberofcolumns*numberofrows; ++i){
		if(changes & (1U<<i) && ((HAL_GetTick() - ButtonsLockout[i]) < BounceDuration)){
			changes &= (0xFFFE << i);
		}
		else if (changes & (1U<<i)){
			ButtonsLockout[i] = HAL_GetTick();
		}
	}
}
