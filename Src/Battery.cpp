
#include "Battery.h"

using namespace std;

Battery::Battery(){
	value_raw = 0;
	value = 0;
	enabled = false;

	__HAL_RCC_ADC1_CLK_ENABLE();
	
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	HAL_ADC_Init(&hadc1);
	s_config.Channel = ADC_CHANNEL_3;
	s_config.Rank = 1;
	s_config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	HAL_ADC_ConfigChannel(&hadc1, &s_config);

	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Battery::scan(){
	s_config.Channel = ADC_CHANNEL_3;
	s_config.Rank = 1;
	s_config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	HAL_ADC_ConfigChannel(&hadc1, &s_config);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 10);
	value_raw = HAL_ADC_GetValue(&hadc1);
	value = static_cast<float>(value_raw)*3.3f*2.0f/4095.0f;
}

float Battery::getVoltage(){
	return value;
}

uint32_t Battery::getValue(){
	return value_raw;
}

void Battery::start(){
	enabled = true;
}

void Battery::stop(){
	enabled = false;
}

void Battery::interrupt(){
	if (enabled) scan();
}

Battery *Battery::getInstance(){
	static Battery instance;
	return &instance;
}
