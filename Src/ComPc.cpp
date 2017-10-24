/**
 * @file ComPc.cpp
 */

#include "ComPc.h"

ComPc::ComPc(USART_TypeDef *port) :
	Usart(huart1)
{
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart1);

	setTypeDef(huart1);

	// // OLD Settinga
	// GPIO_InitTypeDef GPIO_InitStructure;
	// USART_InitTypeDef USART_InitStructure;
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	// RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	// RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	// GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	// // GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9/* | GPIO_Pin_10*/;
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	// GPIO_Init(GPIOA, &GPIO_InitStructure);

	// USART_InitStructure.USART_BaudRate = 115200;
	// USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// USART_InitStructure.USART_Parity = USART_Parity_No;
	// USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// USART_InitStructure.USART_Mode = /*USART_Mode_Rx | */USART_Mode_Tx;
	// USART_Init(USART1, &USART_InitStructure);
	// // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	// USART_Cmd(USART1, ENABLE);

	// DMA_DeInit(DMA2_Stream7);
	// DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	// DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));
	// DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)send_buf;
	// DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	// DMA_InitStructure.DMA_BufferSize = BUFSIZE;
	// DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	// DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	// DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	// DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	// DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	// DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	// DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	// DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	// DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	// DMA_Init(DMA2_Stream7, &DMA_InitStructure);

	// // NVIC_InitTypeDef NVIC_InitStructure;
	// // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// // NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	// // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	// // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	// // NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	// // NVIC_Init(&NVIC_InitStructure);
}


ComPc *ComPc::getInstance(){
	static ComPc instance(USART1);
	return &instance;
}


uint8_t ComPc::bit2hex(const uint8_t val){
	if(val < 0x0A) return '0' + val;
	else if(val > 0x0F) return 'X';
	else return 'A' + (val - 0x0A);
}

std::string ComPc::hex(const uint8_t val){
	std::string tmp;
	tmp += " 0x";
	for (int i=1; i>=0; i--) {
		tmp += bit2hex((val>>(4*i)) & 0x0F);
	}
	tmp += " ";
	return tmp;
}

std::string ComPc::hex(const uint16_t val){
	std::string tmp;
	tmp += " 0x";
	for (auto i=3; i>=0; i--) {
		tmp += bit2hex((val>>(4*i)) & 0x0F);
	}
	tmp += " ";
	return tmp;
}

std::string ComPc::hex(const uint32_t val){
	std::string tmp;
	tmp += " 0x";
	for (auto i=7; i>=0; i--) {
		tmp += bit2hex((val>>(4*i)) & 0x0F);
	}
	tmp += " ";
	return tmp;
}

std::string ComPc::dec(const float val){
	std::string tmp;
	float c = val;
	unsigned long div = 10000000;
	if(val < 0){
		tmp += "-";
		c = -1 * val;
	}
	for (auto i=0; i<8; i++) {
		tmp += bit2hex(static_cast<uint8_t>(c/div)%10);
		div /= 10;
	}
	tmp += ".";
	div = 10;
	for (auto i=0; i<8; i++) {
		tmp += bit2hex((static_cast<uint8_t>(c*div)) %10);
		div *= 10;
	}
	return tmp;
}



void ComPc::sendDecimal(const uint32_t val, const bool isPlus){
	char tmp[12 + 1];
	uint16_t pos = 12;
	uint32_t v = val;
	tmp[pos] = 0;
	do {
		--pos;
		tmp[pos] = '0' + (v % 10);
		v /= 10;
	} while(v != 0);
	if(isPlus == false) {
		--pos;
		tmp[pos] = '-';
	}
	sendnbyte(&tmp[pos], 12 - pos);
}
void ComPc::sendDecimal(const int32_t val){
	bool pls = true;
	int32_t v = val;
	if(val < 0){
		pls = false;
		v = -v;
	}
	sendDecimal(static_cast<uint32_t>(v), pls);
}

ComPc& ComPc::operator << (const char chr) {
	send1byte(chr);
	return *this;
}
ComPc& ComPc::operator << (const uint8_t chr) {
	sendDecimal(static_cast<uint32_t>(chr), true);
	return *this;
}
ComPc& ComPc::operator << (const int8_t chr) {
	sendDecimal(static_cast<uint32_t>(chr));
	return *this;
}
ComPc& ComPc::operator << (const uint16_t chr) {
	sendDecimal(static_cast<uint32_t>(chr), true);
	return *this;
}
ComPc& ComPc::operator << (const int16_t chr) {
	sendDecimal(static_cast<uint32_t>(chr));
	return *this;
}
ComPc& ComPc::operator << (const uint32_t chr) {
	sendDecimal(static_cast<uint32_t>(chr), true);
	return *this;
}
ComPc& ComPc::operator << (const int32_t chr) {
	sendDecimal(static_cast<uint32_t>(chr));
	return *this;
}
ComPc& ComPc::operator << (const char* str) {
	for (int i=0; str[i] != '\0'; i++) {
		send1byte(str[i]);
	}
	return *this;
}
// ComPc& ComPc::operator << (const std::string& str) {
// 	sendnbyte(str.c_str(), str.length());
// 	return *this;
// }

uint16_t ComPc::printf(const char *fmt, ...){
	char buffer[300];
	int len;

	va_list ap;
	va_start(ap, fmt);

	len = vsprintf(buffer, fmt, ap);
	// sendbydma(buffer, len);
	sendnbyte(buffer, len);
	va_end(ap);
	return static_cast<uint16_t>(len);
}

