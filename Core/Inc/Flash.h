/*
 * Flash.h
 *
 *  Created on: May 5, 2026
 *      Author: Landi András
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "main.h"
#include "USB.h"

void EraseFlash();
void FlashAppend(uint32_t* data, uint32_t size_in_bytes);
uint32_t GetFlashAddress();
void ScanFlashSector();
#endif /* INC_FLASH_H_ */
