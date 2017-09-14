/**
 * @file Fram.h
 */
#pragma once

#include "stm32f4xx_hal.h"

#include <vector>
#include "Spi.h"

#include "Map.h"

enum class FramCommands : unsigned char {
	WRSR = 0x1,
	WRITE = 0x2,
	READ = 0x3,
	WRDI = 0x4,
	RDSR = 0x5,
	WREN = 0x6,
	RDID = 0x9F,
};


// 16Kbit = 2KByte Max 0x7FF

class Fram : protected Spi{
private:
	explicit Fram();

	SPI_HandleTypeDef hspi;

public:
	static Fram *getInstance();

	int writeStatusResister(const uint8_t data);
	int readStatusResister(uint8_t& data);
	bool readDeviceId();

	int writeEnable();

	int writeData(const std::vector<uint8_t> &data, const uint16_t addr, const uint8_t num);
	int readData(std::vector<uint8_t> &data, const uint16_t addr, const uint8_t num);

	// num = (0 .. 9);
	bool saveMap(const Map& map, const int num);
	bool loadMap(Map& map, const int num);
};
