/**
 * @file Speaker.cpp
 */

#include "Speaker.h"

Speaker::Speaker(){
	sound_time = 0;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_TIM9_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OC_InitTypeDef TIM_OC_InitStructure;

	GPIO_InitStructure.Pin = GPIO_PIN_2;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Alternate = GPIO_AF3_TIM9;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	htim.Instance = TIM9;
	htim.Init.Prescaler = 200-1;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = 420-1;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_PWM_Init(&htim);

	TIM_OC_InitStructure.OCMode = TIM_OCMODE_PWM1;
	TIM_OC_InitStructure.Pulse = 0;
	TIM_OC_InitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM_OC_InitStructure.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim, &TIM_OC_InitStructure, TIM_CHANNEL_1);
}

void Speaker::playSound(int freq, int length, bool sync){
	TIM_OC_InitTypeDef TIM_OC_InitStructure;

	htim.Instance = TIM9;
	htim.Init.Prescaler = 32-1;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = static_cast<uint16_t>(5250000/freq)-1;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_PWM_Init(&htim);

	TIM_OC_InitStructure.OCMode = TIM_OCMODE_PWM1;
	TIM_OC_InitStructure.Pulse = (htim.Init.Period/2)-1;
	TIM_OC_InitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM_OC_InitStructure.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim, &TIM_OC_InitStructure, TIM_CHANNEL_1);

	if(sync){
		HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_1);
		HAL_Delay(length);
		HAL_TIM_PWM_Stop(&htim, TIM_CHANNEL_1);
	} else {
		/// @todo 非同期で動作するように
		HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_1);
		sound_time = length;
	}
}

void Speaker::interrupt(){
	if(sound_time == 1){
		sound_time = 0;
		HAL_TIM_PWM_Stop(&htim, TIM_CHANNEL_1);
	} else if(sound_time > 1){
		-- sound_time;
	}
}

void Speaker::playMusic(MusicNumber n){
	switch(n){
	case MusicNumber::KIRBY3_CURSOR:
		playSound(1568, 50, true);
		break;
	case MusicNumber::KIRBY3_POWERON:
		playSound(880, 100, true);
		playSound(1175, 200, true);
		break;
	case MusicNumber::KIRBY3_SELECT:
		playSound(1174, 30, true);
		playSound(1661, 30, true);
		playSound(2093, 30, true);
		break;
	case MusicNumber::KIRBY64_BEGINNER_1:
		playSound(440, 160, true);
		HAL_Delay(80);
		playSound(440, 80, true);
		playSound(587, 80, true);
		HAL_Delay(80);
		playSound(740, 160, true);
		HAL_Delay(160);
		playSound(587, 160, true);
		playSound(659, 160, true);
		playSound(740, 160, true);
		playSound(784, 160, true);
		HAL_Delay(80);
		playSound(740, 80, true);
		HAL_Delay(160);
		playSound(587, 160, true);
		playSound(659, 80, true);
		break;
	case MusicNumber::KIRBY64_BEGINNER_2:
		playSound(988, 160, true);
		HAL_Delay(80);
		playSound(988, 80, true);
		playSound(1109, 160, true);
		playSound(1175, 160, true);
		HAL_Delay(160);
		playSound(1109, 160, true);
		playSound(988, 160, true);
		playSound(1175, 160, true);
		playSound(880, 160, true);
		HAL_Delay(80);
		playSound(880, 60, true);
		HAL_Delay(20);
		playSound(880, 160, true);
		playSound(988, 160, true);
		playSound(880, 320, true);		
		break;
	case MusicNumber::KIRBY64_POPSTAR_1:
		
		break;
	case MusicNumber::KIRBY64_POPSTAR_2:
		
		break;
	case MusicNumber::KIRBY_CLEAR_LONG:
		
		break;
	case MusicNumber::KIRBY_CLEAR_SHORT:
		
		break;
	case MusicNumber::KIRBY3_COLLECT:
		
		break;
	case MusicNumber::KIRBY_DYING:
		playSound(1975, 50, true);
		HAL_Delay(30);
		playSound(1975, 50, true);
		HAL_Delay(30);
		playSound(1975, 50, true);
		HAL_Delay(100);
		playSound(1975, 50, true);
		HAL_Delay(30);
		playSound(1975, 50, true);
		HAL_Delay(30);
		playSound(1975, 50, true);
		HAL_Delay(100);
		playSound(1975, 50, true);
		HAL_Delay(30);
		playSound(1975, 50, true);
		HAL_Delay(30);
		playSound(1975, 50, true);
		break;
	case MusicNumber::KIRBY_1UP:
		
		break;
	case MusicNumber::KIRBY_DIE:
		
		break;
	case MusicNumber::KIRBY_GOURMETRACE:
		
		break;
	case MusicNumber::OIRABOKODAZE1:
		playSound(466, 600, true);
		playSound(554, 300, true);
		playSound(622, 300, true);
		playSound(698, 450, true);
		playSound(740, 150, true);
		playSound(698, 300, true);
		playSound(554, 300, true);
		playSound(622, 450, true);
		playSound(698, 150, true);
		playSound(622, 300, true);
		playSound(554, 300, true);
		playSound(523, 270, true); HAL_Delay(30);
		playSound(523, 300, true);
		playSound(415, 550, true);
		HAL_Delay(50);
		playSound(466, 600, true);
		playSound(554, 300, true);
		playSound(622, 300, true);
		playSound(698, 450, true);
		playSound(740, 150, true);
		playSound(698, 300, true);
		playSound(554, 300, true);
		playSound(622, 300, true); HAL_Delay(150);
		playSound(622, 150, true);
		playSound(554, 300, true);
		playSound(554, 300, true);
		playSound(466, 600, true);
		break;
	case MusicNumber::KIRBY3_ORIG_DESELECT:
		playSound(2093, 30, true);
		playSound(1661, 30, true);
		playSound(1174, 30, true);
		break;
	case MusicNumber::KANSAIDENKIHOANKYOUKAI:
		playSound(494, 360, true); //kan
		playSound(392, 240, true); //sa
		playSound(370, 120, true); //i
		HAL_Delay(240);
		playSound(494, 120, true); //de
		playSound(392, 240, true); //n
		playSound(370, 120, true); //ki
		playSound(494, 360, true); //ho
		playSound(554, 240, true); //a
		playSound(494, 120, true); //n
		playSound(554, 360, true); //kyou
		playSound(554, 360, true); //kai
		break;
	case MusicNumber::HIRAPA:
		playSound(440, 400, true);
		playSound(784, 200, true);
		playSound(659, 800, true); 
		break;
	case MusicNumber::KIRBYKAGAMI_OP:
		playSound(262, 100, true);
		HAL_Delay(20);
		playSound(262, 120, true);
		HAL_Delay(120);
		playSound(262, 120, true);
		playSound(294, 120, true);
		HAL_Delay(120);
		playSound(330, 120, true);
		HAL_Delay(120);
		playSound(339, 150, true); 
		break;
	}
}

Speaker* Speaker::getInstance(){
	static Speaker instance;
	return &instance;
}
