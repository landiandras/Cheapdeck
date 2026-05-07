/*
 * USB.c
 *
 *  Created on: Mar 27, 2026
 *      Author: Landi András
 */

#include "USB.h"

extern uint8_t numberofrows;
extern uint8_t numberofcolumns;

typedef struct __attribute__((packed)){
	uint8_t MODIFIER;
	uint8_t KEYCODE;
}KeyAssignment;

/*
__attribute__((aligned(4))) KeyAssignment ButtonAssignments[] = {
		{0, 'q'-'a' + 0x04},
		{0, 'a'-'a' + 0x04},
		{0x02, 'y'-'a' + 0x04},
		{0, 'w'-'a' + 0x04},
		{0, 's'-'a' + 0x04},
		{0, 'x'-'a' + 0x04},
		{0, 'e'-'a' + 0x04},
		{0, 'd'-'a' + 0x04},
		{0, 'c'-'a' + 0x04},
		{0, 'r'-'a' + 0x04},
		{0, 'f'-'a' + 0x04},
		{0, 'v'-'a' + 0x04}
};
*/
__attribute__((aligned(4))) KeyAssignment ButtonAssignments[12] = {0};


HIDkeypress ButtonsToReport(uint16_t buttons){
	HIDkeypress result = {0};
	result.REPORTID = 0x01;
	uint8_t pressedkeys = 0;
	for(size_t i = 0; i<numberofrows*numberofcolumns; ++i){
		if(buttons&(1U<<i)){
			*(&result.KEYCODE1 + pressedkeys++) = ButtonAssignments[i].KEYCODE;
			result.MODIFIER|=ButtonAssignments[i].MODIFIER;
			if(pressedkeys > 5) break;
		}
	}
	return result;
}

void LoadAssignments(){
	 uint32_t flashAddress = 0x08060000;
	 memcpy(ButtonAssignments, (const void*)flashAddress, sizeof(ButtonAssignments));
}

void SaveAssignments(){
	WriteToFlash((uint32_t*)ButtonAssignments, sizeof(ButtonAssignments));
}

void ChangeButtonAssignment(uint8_t but, uint8_t MODIFIER, uint8_t KEYCODE){
	if(but>11) return;
	const int index_map[12] = {0, 3, 6, 9, 1, 4, 7, 10, 2, 5, 8, 11};
	int index = index_map[but];
	ButtonAssignments[index].KEYCODE = KEYCODE;
	ButtonAssignments[index].MODIFIER = MODIFIER;
	SaveAssignments();
}
