/*
 * USB.c
 *
 *  Created on: Mar 27, 2026
 *      Author: Landi András
 */

#include "USB.h"
#include "Buttons.h"
#include "Graphics.h"
#include "Display.h"

extern uint8_t numberofrows;
extern uint8_t numberofcolumns;
extern bool USBPacketReceived;
HIDdata HIDdataOut = {0};
HIDdata HIDdataIn = {0};
extern bool drawingallowed;
extern uint8_t Frame[8][128];
extern bool KeysChanged;
extern int32_t cntr;
extern USBD_HandleTypeDef hUsbDeviceFS;
extern bool mute_flag;


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
extern KeyAssignment ButtonAssignments[12];


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
	 uint32_t flashAddress = GetFlashAddress();
	 if (flashAddress != 0x08060000) {
		 flashAddress -= sizeof(ButtonAssignments);
	 }

	 memcpy(ButtonAssignments, (const void*)flashAddress, sizeof(ButtonAssignments));
}

void SaveAssignments(){
	FlashAppend((uint32_t*)ButtonAssignments, sizeof(ButtonAssignments));
}

void ChangeButtonAssignment(uint8_t but, uint8_t MODIFIER, uint8_t KEYCODE, char* ascii){
	if(but>11) return;
	const int index_map[12] = {0, 3, 6, 9, 1, 4, 7, 10, 2, 5, 8, 11};
	int index = index_map[but];
	if(KEYCODE == 0xE3){
		MODIFIER |= 0x08;
		KEYCODE = 0x00;
	}
	ButtonAssignments[index].KEYCODE = KEYCODE;
	ButtonAssignments[index].MODIFIER = MODIFIER;
	strcpy(ButtonAssignments[index].ascii, ascii);
	SaveAssignments(); //TODO re-enable flash saving once done
}

char* getascii(uint8_t but){
	if(but > 12 ) return NULL;
	return ButtonAssignments[but].ascii;
}

void ProcessIncomingUSBData(){
	if (!USBPacketReceived) return;

	    if (HIDdataIn.DATA[0] == 1) {

	        uint8_t key_index = HIDdataIn.DATA[1];
	        uint8_t modifiers = HIDdataIn.DATA[2];
	        uint8_t hid_keycode = HIDdataIn.DATA[3];
	        char* key_name = (char*)&(HIDdataIn.DATA[4]);

	        ChangeButtonAssignment(key_index, modifiers, hid_keycode, key_name);


	        UpdateDisplayForProgramming(key_index, modifiers, key_name);
	    }

	    // Clear data and flag so we are ready for the next packet
	    memset(&HIDdataIn, 0, sizeof(HIDdataIn));
	    USBPacketReceived = false;
}


void SendKeyboardReport(USBD_CUSTOM_HID_HandleTypeDef *hhid) {
    // Only proceed if keys changed and USB bus is idle
    if (!KeysChanged || hhid == NULL || hhid->state != CUSTOM_HID_IDLE) return;

	HIDkeypress keyreport = {0};

    uint16_t current_keys = GetKeys();
    keyreport = ButtonsToReport(current_keys);

    uint8_t result = USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t*)&keyreport, sizeof(keyreport));

    if (result == USBD_OK) {
        KeysChanged = false; // Successfully sent, clear flag

        // Only update the display if keys are actively being pressed (not released)
        if (current_keys > 0) {
            UpdateDisplayForKeyPress(keyreport.MODIFIER, current_keys);
        }
    }
}

void SendMediaControlReport(USBD_CUSTOM_HID_HandleTypeDef *hhid) {
    if (hhid == NULL || hhid->state != CUSTOM_HID_IDLE) return;
    static uint8_t encoder_state = 0;
    uint8_t media_report[64] = {0};
    if (encoder_state == 0) {
        if (mute_flag) {
            media_report[0] = 0x03;
            media_report[1] = 0x01; // Mute
            if(USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, media_report, 64) == USBD_OK){
                mute_flag = 0;
                encoder_state = 1;
            }
        }
        else if (cntr > 0) {
            media_report[0] = 0x03;
            media_report[1] = 0x02; // Volume Up
            if(USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, media_report, 64) == USBD_OK){
                cntr--;
                encoder_state = 1;
            }
        }
        else if (cntr < 0) {
            media_report[0] = 0x03;
            media_report[1] = 0x04; // Volume Down
            if(USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, media_report, 64) == USBD_OK){
                cntr++;
                encoder_state = 1;
            }
        }
    }
    else if (encoder_state == 1) {
        media_report[0] = 0x03;
        media_report[1] = 0x00; // Release
        if(USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, media_report, 64) == USBD_OK){
            encoder_state = 0;
        }
    }
}
