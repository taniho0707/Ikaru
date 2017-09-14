/**
 * @file Motor.cpp
 */

#include "Motor.h"

Motor::Motor() :
	MAX_PERIOD(336-1)
{
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;
	
	__HAL_RCC_TIM4_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	htim.Instance = TIM4;
	htim.Init.Prescaler = 0;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = MAX_PERIOD;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_PWM_Init(&htim);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_3);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	enabled = false;
}

void Motor::setForward(MotorSide side){
	if(side == MotorSide::LEFT)
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
}

void Motor::setBackward(MotorSide side){
	if(side == MotorSide::LEFT)
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
}


void Motor::enable(){
	HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_3);
	enabled = true;
}

void Motor::disable(){
	HAL_TIM_PWM_Stop(&htim, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim, TIM_CHANNEL_3);
	enabled = false;
}

bool Motor::isEnabled(){
	return enabled;
}

void Motor::setDuty(MotorSide side, int16_t duty){
	int16_t duty_t;
	if(duty < 0){
		if(side == MotorSide::LEFT){
			setBackward(side);
		} else {
			setForward(side);
		}
		duty_t = -1 * duty;
	} else {
		if(side == MotorSide::LEFT){
			setForward(side);
		} else {
			setBackward(side);
		}
		duty_t = duty;
	}
	if(duty_t > MAX_PERIOD) duty_t = MAX_PERIOD;
	static TIM_OC_InitTypeDef sConfigOC;
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = duty_t;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if(side == MotorSide::RIGHT){
		HAL_TIM_PWM_Stop(&htim, TIM_CHANNEL_1);
		HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_1);
	} else {
		HAL_TIM_PWM_Stop(&htim, TIM_CHANNEL_3);
		HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_3);
	}
}

Motor* Motor::getInstance(){
	static Motor instance;
	return &instance;
}
