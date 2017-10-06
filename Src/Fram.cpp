/**
 * @file Fram.cpp
 */
#include "Fram.h"

Fram::Fram() :
	Spi(hspi, GPIOA, GPIO_PIN_14)
{
	__HAL_RCC_SPI3_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_SPI3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	hspi.Instance = SPI3;
	hspi.Init.Mode = SPI_MODE_MASTER;
	hspi.Init.Direction = SPI_DIRECTION_2LINES;
	hspi.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi.Init.NSS = SPI_NSS_SOFT;
	hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi.Init.CRCPolynomial = 10;

	setTypeDef(hspi);

	HAL_SPI_Init(&hspi);
	resetChipSelect();
}


int Fram::writeStatusResister(const uint8_t data){
	std::vector<uint8_t> writedata(2);
	writedata[0] = static_cast<uint8_t>(FramCommands::WRSR);
	writedata[1] = data;
	return rwMultiByte(writedata, writedata, 0, 2);
}

int Fram::readStatusResister(uint8_t& data){
	std::vector<uint8_t> writedata(1);
	std::vector<uint8_t> readdata(1);
	writedata[0] = static_cast<uint8_t>(FramCommands::RDSR);
	readdata[0] = 0x00;
	int retval = rwMultiByte(readdata, writedata, 1, 1);
	data = readdata[0];
	return retval;
}

bool Fram::readDeviceId(){
	// uint8_t writedata[5];
	// uint8_t readdata[5];
	std::vector<uint8_t> writedata(1);
	std::vector<uint8_t> readdata(4);
	writedata[0] = static_cast<uint8_t>(FramCommands::RDID);

	uint8_t bulkdata = 0x0;
	HAL_SPI_Transmit(&port, &bulkdata, 1, 1000);
	HAL_Delay(1);

	rwMultiByte(readdata, writedata, 4, 1);

	// setChipSelect();
	// HAL_SPI_TransmitReceive(&port, writedata, readdata, 5, 1000);
	// resetChipSelect();

	if (readdata[0] == 0x04
		&& readdata[1] == 0x7F
		&& readdata[2] == 0x01
		&& readdata[3] == 0x01) {
		return true;
	} else {
		return false;
	}
}


int Fram::writeEnable(){
	setChipSelect();
	int retval = writeSingleByte(static_cast<uint8_t>(FramCommands::WREN));
	resetChipSelect();
	return retval;
}

int Fram::writeData(const std::vector<uint8_t> &data, const uint16_t addr, const uint8_t num){
	std::vector<uint8_t> writedata(num+3);
	writedata[0] = static_cast<uint8_t>(FramCommands::WRITE);
	writedata[1] = static_cast<uint8_t>(((addr) >> 8)&0x00FF);
	writedata[2] = static_cast<uint8_t>((addr)&0x00FF);
	for (int i=0; i<num; i++)
		writedata[i+3] = data[i];
	writeEnable();
	// HAL_Delay(1); // ここコメントアウトした2017年10月5日
	return rwMultiByte(writedata, writedata, 0, num+3);
}

int Fram::readData(std::vector<uint8_t> &data, const uint16_t addr, const uint8_t num){
	std::vector<uint8_t> writedata(3);
	writedata[0] = static_cast<uint8_t>(FramCommands::READ);
	writedata[1] = static_cast<uint8_t>(((addr) >> 8)&0x00FF);
	writedata[2] = static_cast<uint8_t>((addr)&0x00FF);
	rwMultiByte(data, writedata, num, 3);
	return 0;
}


bool Fram::writeUInt16(const uint16_t data, const uint16_t addr) {
	std::vector<uint8_t> senddata(2);
	senddata.at(0) = (data >> 8) & 0xFF;
	senddata.at(1) = data & 0xFF;
	writeData(senddata, addr, 2);
	return true;
}

bool Fram::writeInt16(const int16_t data, const uint16_t addr) {
	std::vector<uint8_t> senddata(2);
	senddata.at(0) = (data >> 8) & 0xFF;
	senddata.at(1) = data & 0xFF;
	writeData(senddata, addr, 2);
	return true;
}

bool Fram::writeUInt32(const uint32_t data, const uint16_t addr) {
	std::vector<uint8_t> senddata(4);
	senddata.at(0) = (data >> 24) & 0xFF;
	senddata.at(1) = (data >> 16) & 0xFF;
	senddata.at(2) = (data >>  8) & 0xFF;
	senddata.at(3) = data & 0xFF;
	writeData(senddata, addr, 4);
	return true;
}

uint16_t Fram::readUInt16(const uint16_t addr) {
	std::vector<uint8_t> readdata(2);
	readData(readdata, addr, 2);
	return static_cast<uint16_t>((readdata.at(0) << 8) | readdata.at(1));
}

int16_t Fram::readInt16(const uint16_t addr) {
	std::vector<uint8_t> readdata(2);
	readData(readdata, addr, 2);
	return static_cast<int16_t>((readdata.at(0) << 8) | readdata.at(1));
}

uint32_t Fram::readUInt32(const uint16_t addr) {
	std::vector<uint8_t> readdata(4);
	readData(readdata, addr, 4);
	return static_cast<uint32_t>((readdata.at(0) << 24) | (readdata.at(1) << 16) | (readdata.at(2) << 8) | readdata.at(3));
}


bool Fram::saveMap(const Map& map, const int num){
	if (num > 4) return false;
	uint16_t baseaddr = 376 * num; // 1 map is 376 byte
	std::vector<uint8_t> v(128);
	for(int i=0; i<31; ++i){
		v.at(4*i  ) = (map.column[i] & 0xFF000000) >> 24;
		v.at(4*i+1) = (map.column[i] & 0x00FF0000) >> 16;
		v.at(4*i+2) = (map.column[i] & 0x0000FF00) >> 8;
		v.at(4*i+3) = (map.column[i] & 0x000000FF);
	}
	writeData(v, baseaddr, 124);
	for(int i=0; i<31; ++i){
		v.at(4*i  ) = (map.row[i] & 0xFF000000) >> 24;
		v.at(4*i+1) = (map.row[i] & 0x00FF0000) >> 16;
		v.at(4*i+2) = (map.row[i] & 0x0000FF00) >> 8;
		v.at(4*i+3) = (map.row[i] & 0x000000FF);
	}
	writeData(v, baseaddr+124, 124);
	for(int i=0; i<32; ++i){
		v.at(4*i  ) = (map.reached[i] & 0xFF000000) >> 24;
		v.at(4*i+1) = (map.reached[i] & 0x00FF0000) >> 16;
		v.at(4*i+2) = (map.reached[i] & 0x0000FF00) >> 8;
		v.at(4*i+3) = (map.reached[i] & 0x000000FF);
	}
	writeData(v, baseaddr+248, 128);
	return true;
}

bool Fram::loadMap(Map& map, const int num){
	if (num > 4) return false;
	uint16_t baseaddr = 376 * num;
	std::vector<uint8_t> v(128);
	readData(v, baseaddr, 124);
	for(auto i=0; i<31; ++i)
		map.column[i] = ((  static_cast<uint32_t>(v[4*i]) << 24)
						 | (static_cast<uint32_t>(v[4*i+1]) << 16)
						 | (static_cast<uint32_t>(v[4*i+2]) << 8)
						 | (static_cast<uint32_t>(v[4*i+3])));
	readData(v, baseaddr+124, 124);
	for(auto i=0; i<31; ++i)
		map.row[i] = ((  static_cast<uint32_t>(v[4*i]) << 24)
					  | (static_cast<uint32_t>(v[4*i+1]) << 16)
					  | (static_cast<uint32_t>(v[4*i+2]) << 8)
					  | (static_cast<uint32_t>(v[4*i+3])));
	readData(v, baseaddr+248, 128);
	for(auto i=0; i<32; ++i)
		map.reached[i] = ((  static_cast<uint32_t>(v[4*i]) << 24)
						  | (static_cast<uint32_t>(v[4*i+1]) << 16)
						  | (static_cast<uint32_t>(v[4*i+2]) << 8)
						  | (static_cast<uint32_t>(v[4*i+3])));
	return true;
}


Fram *Fram::getInstance(){
	static Fram instance;
	return &instance;
}

