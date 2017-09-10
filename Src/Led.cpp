/**
 * @file Led.cpp
 */

#include "Led.h"

Led::Led(){
	gpio_port = GPIOC;
	gpio_channel = GPIO_PIN_13;

	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_13;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void Led::initPort(LedNumbers num){
	GPIO_InitTypeDef GPIO_InitStructure;
	switch(num){
	case LedNumbers::FRONT :
		break;
	case LedNumbers::TOP1 :
		GPIO_InitStructure.Pin = GPIO_PIN_10;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
		break;
	case LedNumbers::TOP2 :
		GPIO_InitStructure.Pin = GPIO_PIN_9;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
		break;
	}
}

void Led::deinitPort(LedNumbers num){
	switch(num){
	case LedNumbers::FRONT :
		break;
	case LedNumbers::TOP1 :
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);
		break;
	case LedNumbers::TOP2 :
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
		break;
	}
}

void Led::setType(LedNumbers num){
	switch(num){
	case LedNumbers::FRONT :
		gpio_port = GPIOC;
		gpio_channel = GPIO_PIN_13;
		break;
	case LedNumbers::TOP1 :
		gpio_port = GPIOA;
		gpio_channel = GPIO_PIN_10;
		break;
	case LedNumbers::TOP2 :
		gpio_port = GPIOA;
		gpio_channel = GPIO_PIN_9;
		break;
	}
}

void Led::on(LedNumbers num){
	setType(num);
	HAL_GPIO_WritePin(gpio_port, gpio_channel, GPIO_PIN_SET);
}

void Led::off(LedNumbers num){
	setType(num);
	HAL_GPIO_WritePin(gpio_port, gpio_channel, GPIO_PIN_RESET);
}

bool Led::isFlicking(LedNumbers num){
	if(flick_params.at(static_cast<uint8_t>(num)).start_time == 0) return false;
	else return true;
}

void Led::flickSync(LedNumbers num, float freq, uint16_t time){
	flickAsync(num, freq, time);
	while(isFlicking(num));
}

void Led::flickAsync(LedNumbers num, float freq, uint16_t time){
	flick_params.at(static_cast<uint8_t>(num)).start_time = Timer::getTime();
	flick_params.at(static_cast<uint8_t>(num)).freq = freq;
	flick_params.at(static_cast<uint8_t>(num)).time = time;
}

void Led::flickStop(LedNumbers num){
	flick_params.at(static_cast<uint8_t>(num)).start_time = 0;
	off(num);
}


void Led::interrupt(){
	uint32_t t;
	uint32_t t2;
	int8_t i = -1;
	for(auto& n : flick_params){
		++i;
		if(n.start_time == 0) continue;
		t2 = static_cast<uint32_t>(1000/n.freq);
		t = (Timer::getTime() - n.start_time) % t2;
		if((n.time != 0) && ((Timer::getTime() - n.start_time) > n.time)){
			n.start_time = 0;
			off(static_cast<LedNumbers>(i));
			continue;
		}
		if(t > (t2/2)) on(static_cast<LedNumbers>(i));
		else off(static_cast<LedNumbers>(i));
	}
}


Led *Led::getInstance(){
	static Led instance;
	return &instance;
}
