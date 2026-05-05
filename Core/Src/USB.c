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
		'q'-'a' + 0x04,
		'a'-'a' + 0x04,
		'y'-'a' + 0x04,
		'w'-'a' + 0x04,
		's'-'a' + 0x04,
		'x'-'a' + 0x04,
		'e'-'a' + 0x04,
		'd'-'a' + 0x04,
		'c'-'a' + 0x04,
		'r'-'a' + 0x04,
		'f'-'a' + 0x04,
		'v'-'a' + 0x04,
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

