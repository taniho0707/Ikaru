/**
 * @file Spi.cpp
 */

#include "Spi.h"


Spi::Spi(SPI_HandleTypeDef spi, GPIO_TypeDef *gpio, uint16_t gpiopin){
	port = spi;
	cs_port = gpio;
	cs_pin = gpiopin;
	for (int i=0; i<8; i++) buffer[i] = 0x00;
}

void Spi::setTypeDef(SPI_HandleTypeDef spi){
	port = spi;
}

void Spi::setChipSelect(){
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
}
void Spi::resetChipSelect(){
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}


int Spi::readSingleByte(uint8_t &data){
	uint8_t bulkdata = 0x00;
	HAL_SPI_TransmitReceive(&port, &bulkdata, &data, 1, 1000);
	return 0;
}

int Spi::writeSingleByte(uint8_t data){
	uint8_t bulk;
	HAL_SPI_TransmitReceive(&port, &data, &bulk, 1, 1000);
	return 0;
}

int Spi::readMultiByte(std::vector<uint8_t> &data, const uint8_t num){
	uint8_t recv = 0x00;
	if(data.capacity() < num) return -1;
	for (int i=0; i<num; i++) {
		readSingleByte(recv);
		data[i] = recv;
	}
	return 0;
}

int Spi::writeMultiByte(const std::vector<uint8_t> &data, const uint8_t num){
	if(data.capacity() < num) return -1;
	for (int i=0; i<num; i++) {
		writeSingleByte(data[i]);
	}
	return 0;
}

int Spi::rwMultiByte(
	std::vector<uint8_t> &data_read, const std::vector<uint8_t> &data_write,
	const uint8_t num_read, const uint8_t num_write)
{
	setChipSelect();
	int retval = 0;
	if(num_write > 0) retval += writeMultiByte(data_write, num_write);
	if(num_read > 0) retval += readMultiByte(data_read, num_read);
	resetChipSelect();
	return retval;
}

