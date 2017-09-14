/**
 * @file Spi.h
 */

#pragma once

#include "stm32f4xx.h"

#include <vector>

#include "ComPc.h"


class Spi{
private:

protected:
	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;
	SPI_HandleTypeDef port;

	uint8_t buffer[8];

	void setChipSelect();
	void resetChipSelect();

	int readSingleByte(uint8_t &data);
	int writeSingleByte(uint8_t data);

	int readMultiByte(std::vector<uint8_t> &data, const uint8_t num);
	int writeMultiByte(const std::vector<uint8_t> &data, const uint8_t num);

	int rwMultiByte(
		std::vector<uint8_t> &data_read, const std::vector<uint8_t> &data_write,
		const uint8_t num_read, const uint8_t num_write);

	void setTypeDef(SPI_HandleTypeDef spi);

public:
	Spi(SPI_HandleTypeDef spi, GPIO_TypeDef *gpio, uint16_t gpiopin);
};
