#pragma once

#include "stm32f4xx_hal.h"

class Battery{
private:
	Battery();

	uint32_t value_raw;
	float value;

	bool enabled;

	ADC_HandleTypeDef hadc1;
	ADC_ChannelConfTypeDef s_config;

public:
	void scan();
	float getVoltage();
	uint32_t getValue();

	void start();
	void stop();

	void interrupt();

	static Battery* getInstance();
};

