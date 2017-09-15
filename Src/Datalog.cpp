/**
 * @file Datalog.cpp
 */

#include "Datalog.h"

Datalog::Datalog() :
	POS_MAX(131072),
	POS_ZERO(0x08080000) //512kByte Sector8-11
{
	pos = 0;
}

bool Datalog::writeFloat(const float data){
	if(pos >= POS_MAX) return false;
	return writeData(POS_ZERO + (4*(pos++)), data);
}

bool Datalog::writeFloat(const float data, const uint32_t p){
	if(p >= POS_MAX) return false;
	return writeData(POS_ZERO + (4*p), data);
}

float Datalog::readFloat(const uint32_t pos){
	float ret = 0.0f;
	readData(POS_ZERO + (4*pos), ret);
	return ret;
}

bool Datalog::cleanFlash(){
	eraseSector(FLASH_SECTOR_8);
	eraseSector(FLASH_SECTOR_9);
	eraseSector(FLASH_SECTOR_10);
	eraseSector(FLASH_SECTOR_11);
	pos = 0;
}


const uint32_t Datalog::getSize(){
	return POS_MAX;
}

Datalog* Datalog::getInstance(){
	static Datalog instance;
	return &instance;
}

