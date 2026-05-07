/*
 * USB.h
 *
 *  Created on: Mar 27, 2026
 *      Author: Landi András
 */

#ifndef INC_USB_H_
#define INC_USB_H_


#include "main.h"
#include "Flash.h"
#include <string.h>

HIDkeypress ButtonsToReport(uint16_t buttons);
void LoadAssignments();
void SaveAssignments();
void ChangeButtonAssignment(uint8_t but, uint8_t MODIFIER, uint8_t KEYCODE);

#endif /* INC_USB_H_ */
