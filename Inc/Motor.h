/**
 * @file Motor.h
 */
#pragma once

#include "stm32f4xx_hal.h"

enum class MotorSide : uint8_t{
	LEFT,
	RIGHT,
};

class Motor{
private:
	explicit Motor();

	bool enabled;

	TIM_HandleTypeDef htim;

	const uint16_t MAX_PERIOD;

	void setForward(MotorSide);
	void setBackward(MotorSide);
	
public:
	void enable();
	void disable();
	bool isEnabled();

	void setDuty(MotorSide, int16_t);

	static Motor* getInstance();
};
