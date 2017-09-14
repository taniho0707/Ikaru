
/**
 * @file Usart.h
 */
#pragma once

#include <string>
#include <cstdarg>
#include <cstdio>
#include "stm32f4xx.h"

#include "Usart.h"


class ComPc : protected Usart{
private:

	explicit ComPc(USART_TypeDef *port);

	uint8_t bit2hex(const uint8_t val);

	DMA_InitTypeDef DMA_InitStructure;
	UART_HandleTypeDef huart1;

public:

	static ComPc *getInstance();

	std::string hex(const uint8_t val);
	std::string hex(const uint16_t val);
	std::string hex(const uint32_t val);
	std::string dec(const float val);

	void sendDecimal(const uint32_t val, const bool isPlus);
	void sendDecimal(const int32_t val);

	ComPc& operator << (const char chr);
	ComPc& operator << (const uint8_t chr);
	ComPc& operator << (const int8_t chr);
	ComPc& operator << (const uint16_t chr);
	ComPc& operator << (const int16_t chr);
	ComPc& operator << (const uint32_t chr);
	ComPc& operator << (const int32_t chr);
	ComPc& operator << (const char* str);
	/* ComPc& operator << (const std::string& str); */

	uint16_t printf(const char *fmt, ...);

};
