/**
 * @file Flash.cpp
 */

#include "Flash.h"

Flash::Flash(){
	
}

bool Flash::eraseSector(const uint32_t sector){
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.Sector = sector;
	EraseInitStruct.NbSectors = 1;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP|FLASH_FLAG_PGPERR|FLASH_FLAG_WRPERR);
	uint32_t PageError = 0;
	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
	HAL_FLASH_Lock();
}

bool Flash::writeData(const uint32_t addr, const float data){
	bool b = false;
	HAL_FLASH_Unlock();
	if(HAL_FLASH_Program(TYPEPROGRAM_WORD, addr, *(uint32_t *)(&data)) == HAL_OK) b = true;
	HAL_FLASH_Lock();
	return b;
}

bool Flash::readData(const uint32_t addr, float& data){
	uint32_t ret;
	HAL_FLASH_Unlock();
	data = *(float *)(addr);
	HAL_FLASH_Lock();
	return true;
}
