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
#include "usbd_customhid.h"


typedef struct __attribute__((packed)){
	uint8_t MODIFIER;
	uint8_t KEYCODE;
	char ascii[32];
}KeyAssignment;

HIDkeypress ButtonsToReport(uint16_t buttons);
void LoadAssignments();
void SaveAssignments();
void ChangeButtonAssignment(uint8_t but, uint8_t MODIFIER, uint8_t KEYCODE, char* ascii);
char* getascii(uint8_t but);
void ProcessIncomingUSBData();
void SendKeyboardReport(USBD_CUSTOM_HID_HandleTypeDef *hhid);
void SendMediaControlReport(USBD_CUSTOM_HID_HandleTypeDef *hhid);

#endif /* INC_USB_H_ */
