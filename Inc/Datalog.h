/*
 * @file Datalog.h
 */

#pragma once

#include "stm32f4xx_hal.h"
#include "Flash.h"

class Datalog : public Flash{
private:
	Datalog();
	
	uint32_t pos;
	const uint32_t POS_MAX;
	const uint32_t POS_ZERO;
	
public:
	bool writeFloat(const float);
	bool writeFloat(const float, const uint32_t);

	float readFloat(const uint32_t);

	bool cleanFlash();
	const uint32_t getSize();
	
	static Datalog* getInstance();
};
