/*
 * Flash.c
 *
 *  Created on: May 5, 2026
 *      Author: Landi András
 */

#include "Flash.h"


void WriteToFlash(uint32_t* data, uint32_t size_in_bytes) {
    uint32_t flashAddress = 0x08060000;

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError = 0;

    EraseInitStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector       = FLASH_SECTOR_7;
    EraseInitStruct.NbSectors    = 1;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
        HAL_FLASH_Lock();
        return;
    }

    // 3. Program the data
    uint32_t num_words = size_in_bytes / 4;
    if (size_in_bytes % 4 != 0) {
            num_words++;
        }

    for(uint32_t i = 0; i < num_words; ++i) {
    	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashAddress, data[i]) != HAL_OK) {
            while(1){
            	int asd = 1;
            }
        }
        flashAddress += 4;
     }


    // 4. Lock the Flash to disable the flash control register access
    HAL_FLASH_Lock();
}


