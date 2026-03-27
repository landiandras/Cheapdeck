/*
 * USB.c
 *
 *  Created on: Mar 27, 2026
 *      Author: Landi András
 */

#include "USB.h"
#include "usbd_hid.h"

extern uint8_t HID_MOUSE_ReportDesc[HID_MOUSE_REPORT_DESC_SIZE];

void InitUSB(){
	//HID_MOUSE_ReportDesc[3]=0x01;
}
