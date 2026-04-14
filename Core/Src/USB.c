/*
 * USB.c
 *
 *  Created on: Mar 27, 2026
 *      Author: Landi András
 */

#include "USB.h"

extern uint8_t numberofrows;
extern uint8_t numberofcolumns;

uint8_t ButtonAssignments[] = {
		0x04,
		0x05,
		0x06,
		0x07,
		0x08,
		0x09,
		0x0A,
		0x0B,
		0x0C,
		0x0D,
		0x0E,
		0x0F
};

HIDkeypress ButtonsToReport(uint16_t buttons){
	HIDkeypress result = {0};
	result.REPORTID = 0x01;
	uint8_t pressedkeys = 0;
	for(size_t i = 0; i<numberofrows*numberofcolumns; ++i){
		if(buttons&(1U<<i)){
			*(&result.KEYCODE1 + pressedkeys++) = ButtonAssignments[i];
			if(pressedkeys > 5) break;
		}
	}
	return result;
}

