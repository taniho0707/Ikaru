/**
 * @file Usart.cpp
 */

#include "Usart.h"


Usart::Usart(UART_HandleTypeDef uart){
	port = uart;
}

void Usart::setTypeDef(UART_HandleTypeDef uart){
	port = uart;
}

uint8_t Usart::getChecksum(const uint8_t *data, const uint8_t num){
	uint8_t cs = 0x00;
	for (uint8_t i=0; i<num; i++) {
		cs += data[i];
	}
	cs = ~cs;
	++cs;
	return cs;
}
uint8_t Usart::getChecksum(const std::vector<uint8_t>&data, const uint8_t num){
	uint8_t cs = 0x00;
	for (auto v : data) {
		cs += v;
	}
	cs = ~cs;
	++cs;
	return cs;
}

void Usart::send1byte(char data){
	uint8_t newdata = data;
	HAL_UART_Transmit(&port, &newdata, 1, 1000);
	// USART_SendData(usart_port, (uint16_t)data);
	// while( USART_GetFlagStatus(usart_port, USART_FLAG_TXE) == RESET );
}
void Usart::send1byte(uint8_t data){
	HAL_UART_Transmit(&port, &data, 1, 1000);
	// USART_SendData(usart_port, (uint16_t)data);
	// while( USART_GetFlagStatus(usart_port, USART_FLAG_TXE) == RESET );
}

void Usart::sendnbyte(char *data, const int len){
	// int i;
	// for(i=0; i<len; ++i) send1byte(data[i]);
	HAL_UART_Transmit(&port, (uint8_t *)(data), len, 1000);
}
void Usart::sendnbyte(std::vector<uint8_t>& c, const int n){
	for(auto v : c) send1byte(v);
}


void Usart::sendbydma(const char *data, const int len){
	// strcpy(send_buf, data);
	// DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
	// DMA_Cmd(DMA2_Stream7, ENABLE);
	// USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}
void Usart::sendbydma(const std::vector<uint8_t>& c, const int n){
	// USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}

bool Usart::recv1byte(uint8_t& data){
	// while(USART_GetFlagStatus(usart_port, USART_FLAG_RXNE) == RESET);
	// data = USART_ReceiveData(usart_port);
	return true;
}

bool Usart::recvnbyte(std::vector<uint8_t>& c, const int n){
	// c.clear();
	// for (int i=0; i<n; i++) {
	// 	c.push_back(static_cast<uint8_t>(USART_ReceiveData(usart_port)));
	// }
	return true;
}

