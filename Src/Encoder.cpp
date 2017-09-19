/**
 * @file Encoder.cpp
 */

#include "Encoder.h"


const uint16_t Encoder::MEDIAN = 30000;
const float Encoder::PULSE_L = 0.0068992;
const float Encoder::PULSE_R = 0.0068992;

Encoder::Encoder(){
	velocity_l = 0.0;
	velocity_r = 0.0;
	for (auto &i : hist_l) i = 0;
	for (auto &i : hist_r) i = 0;
	ite_hist = 0;
	last_l = 0.0f;
	last_r = 0.0f;

	__HAL_RCC_TIM5_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct6;
	GPIO_InitStruct6.Pin = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct6.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct6.Pull = GPIO_NOPULL;
	GPIO_InitStruct6.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct6.Alternate = GPIO_AF2_TIM5;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct6);
	GPIO_InitStruct6.Alternate = GPIO_AF1_TIM2;
	GPIO_InitStruct6.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct6);
	GPIO_InitStruct6.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct6);

	TIM_HandleTypeDef htim2;
	TIM_HandleTypeDef htim5;
	TIM_Encoder_InitTypeDef sConfig_enl;
	TIM_MasterConfigTypeDef sMasterConfig;
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 65534;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Instance = TIM5;
	htim5.Init.Prescaler = 0;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 65534;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	sConfig_enl.EncoderMode = TIM_ENCODERMODE_TI12;
	sConfig_enl.IC1Polarity = TIM_ICPOLARITY_RISING;
	sConfig_enl.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig_enl.IC1Prescaler = TIM_ICPSC_DIV1;
	sConfig_enl.IC1Filter = 0;
	sConfig_enl.IC2Polarity = TIM_ICPOLARITY_RISING;
	sConfig_enl.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig_enl.IC2Prescaler = TIM_ICPSC_DIV1;
	sConfig_enl.IC2Filter = 0;
	HAL_TIM_Encoder_Init(&htim2, &sConfig_enl);
	HAL_TIM_Encoder_Init(&htim5, &sConfig_enl);
	
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
	HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig);

	TIM2->CNT = MEDIAN;
	TIM5->CNT = MEDIAN;
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
}

float Encoder::getVelocity(EncoderSide s){
	if(s == EncoderSide::LEFT) return velocity_l;
	else return velocity_r;
}

float Encoder::getVelocityCorrect(EncoderSide s){
	if(s == EncoderSide::LEFT) return hist_l[ite_hist];
	else return hist_r[ite_hist];
}

void Encoder::interrupt(){
	last_l = PULSE_L * static_cast<int16_t>(MEDIAN - TIM2->CNT);
	last_r = PULSE_R * static_cast<int16_t>(MEDIAN - TIM5->CNT);
	TIM2->CNT = MEDIAN;
	TIM5->CNT = MEDIAN;
	if(++ite_hist >= BUFSIZE) ite_hist = 0;
	hist_l[ite_hist] = last_l;
	hist_r[ite_hist] = last_r;
	velocity_l = 0;
	velocity_r = 0;
	for (auto i : hist_l) velocity_l += i;
	for (auto i : hist_r) velocity_r += i;
	velocity_l /= BUFSIZE;
	velocity_r /= BUFSIZE;
}

Encoder* Encoder::getInstance(){
	static Encoder instance;
	return &instance;
}

