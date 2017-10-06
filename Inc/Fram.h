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


// 16Kbit = 2KByte = 2048Byte Max 0x7FF
// 1 map =  376Byte
// 2 map =  752Byte
// 3 map = 1128Byte
// 4 map = 1504Byte
// 5 map = 1880Byte
// (u)int16_t = 2Byte
// 0x758(1880) から 0x7FF がパラメータ領域
// 0x758 にsize_t(sizeof(size_t)=4)のハッシュ値

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

	bool writeUInt16(const uint16_t data, const uint16_t addr);
	bool writeInt16(const int16_t data, const uint16_t addr);
	bool writeUInt32(const uint32_t data, const uint16_t addr);
	uint16_t readUInt16(const uint16_t addr);
	int16_t readInt16(const uint16_t addr);
	uint32_t readUInt32(const uint16_t addr);

	// num = (0 .. 4);
	bool saveMap(const Map& map, const int num);
	bool loadMap(Map& map, const int num);
};
