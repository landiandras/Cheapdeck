/*
 * Flash.c
 *
 *  Created on: May 5, 2026
 *      Author: Landi András
 */

#include "Flash.h"
#include "USB.h"
#define SECTOR_SIZE		0x20000
#define FLASH_EMPTY		0xFFFFFFFF
#define SECTOR_ADDRESS	0x08060000

extern KeyAssignment ButtonAssignments[12];

KeyAssignment* last_valid_struct = NULL; // Points to the active config
KeyAssignment* next_write_ptr    = NULL; // Points to where the next save goes


uint32_t current_ptr = 0;


void EraseFlash() {
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError = 0;

    EraseInitStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector       = FLASH_SECTOR_7;
    EraseInitStruct.NbSectors    = 1;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
    }
    HAL_FLASH_Lock();
}

void ScanFlashSector() {
    current_ptr = SECTOR_ADDRESS;
    uint32_t sector_end = SECTOR_ADDRESS + SECTOR_SIZE;
    uint32_t chunk_size = sizeof(ButtonAssignments);

    while ((current_ptr + chunk_size) <= sector_end) {
        if (*(uint32_t*)current_ptr == FLASH_EMPTY) {
            break; // Found the first empty spot!
        }
        current_ptr += chunk_size;
    }
}

void FlashAppend(uint32_t* data, uint32_t size_in_bytes) {
	//check if sector is full
    if ((current_ptr + size_in_bytes) > (SECTOR_ADDRESS + SECTOR_SIZE)) {
        EraseFlash();
        current_ptr = SECTOR_ADDRESS;
        return;
    }

    HAL_FLASH_Unlock();

    uint32_t WritingAddress = current_ptr;

    // Ceiling division to ensure we write enough words
    uint32_t num_words = (size_in_bytes + 3) / 4;

    for(uint32_t i = 0; i < num_words; ++i) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, WritingAddress, data[i]) != HAL_OK) {
            while(1) {
                // TODO: Handle flash program fault
            }
        }
        WritingAddress += 4;
    }

    // Update global pointer for the next write operation
    // We strictly advance it by the exact size we expect a config to take
    // to maintain alignment for future scans.
    current_ptr += sizeof(ButtonAssignments);

    HAL_FLASH_Lock();
}

uint32_t GetFlashAddress(){
	return current_ptr;
}

