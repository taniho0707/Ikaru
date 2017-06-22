/**
 * @file Led.cpp
 */

#include "Led.h"

Led::Led(){
	GPIO_TypeDef * gpio_port = GPIOA;
	uint16_t gpio_channel = GPIO_PIN_11;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStructure;
	/*Configure GPIO pin : PtPin */
	GPIO_InitStructure.Pin = GPIO_PIN_11;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Led::setType(LedNumbers num){
	// switch(num){
	// case LedNumbers::FRONT :
	// 	gpio_port = GPIOH;
	// 	gpio_channel = GPIO_Pin_1;
	// 	break;
	// case LedNumbers::LEFT1 :
	// 	gpio_port = GPIOC;
	// 	gpio_channel = GPIO_Pin_14;
	// 	break;
	// case LedNumbers::LEFT2 :
	// 	gpio_port = GPIOC;
	// 	gpio_channel = GPIO_Pin_15;
	// 	break;
	// case LedNumbers::LEFT3 :
	// 	gpio_port = GPIOH;
	// 	gpio_channel = GPIO_Pin_0;
	// 	break;
	// case LedNumbers::RIGHT :
	// 	gpio_port = GPIOC;
	// 	gpio_channel = GPIO_Pin_13;
	// 	break;
	// case LedNumbers::TOP1 :
	// 	gpio_port = GPIOA;
	// 	gpio_channel = GPIO_Pin_10;
	// 	break;
	// case LedNumbers::TOP2 :
	// 	gpio_port = GPIOA;
	// 	gpio_channel = GPIO_Pin_11;
	// 	break;
	// }
}

void Led::on(LedNumbers num){
	setType(num);
	// HAL_GPIO_WritePin(gpio_port, gpio_channel, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
}

void Led::off(LedNumbers num){
	setType(num);
	// HAL_GPIO_WritePin(gpio_port, gpio_channel, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
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
	// flick_params.at(static_cast<uint8_t>(num)).start_time = Timer::getTime();
	// flick_params.at(static_cast<uint8_t>(num)).freq = freq;
	// flick_params.at(static_cast<uint8_t>(num)).time = time;
}

void Led::flickStop(LedNumbers num){
	flick_params.at(static_cast<uint8_t>(num)).start_time = 0;
	off(num);
}


void Led::interrupt(){
	// uint32_t t;
	// uint32_t t2;
	// int8_t i = -1;
	// for(auto& n : flick_params){
	// 	++i;
	// 	if(n.start_time == 0) continue;
	// 	t2 = static_cast<uint32_t>(1000/n.freq);
	// 	t = (Timer::getTime() - n.start_time) % t2;
	// 	if((n.time != 0) && ((Timer::getTime() - n.start_time) > n.time)){
	// 		n.start_time = 0;
	// 		off(static_cast<LedNumbers>(i));
	// 		continue;
	// 	}
	// 	if(t > (t2/2)) on(static_cast<LedNumbers>(i));
	// 	else off(static_cast<LedNumbers>(i));
	// }
}


Led *Led::getInstance(){
	static Led instance;
	return &instance;
}
