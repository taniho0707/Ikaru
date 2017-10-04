
#include "WallSensor.h"

using namespace std;

/// @todo add wait REDEN flag
WallSensor::WallSensor() :
	VAL_REF_FLEFT(190),
	// VAL_REF_LEFT(630),   // Q
	// VAL_REF_FRONT(1150), // Q
	// VAL_REF_RIGHT(780),  // Q
	// VAL_REF_LEFT(320),  // H_OLD
	// VAL_REF_FRONT(370), // H_OLD
	// VAL_REF_RIGHT(210), // H_OLD 	
	VAL_REF_LEFT(350),  // H
	VAL_REF_FRONT(290), // H
	VAL_REF_RIGHT(180), // H
	VAL_REF_FRIGHT(120),
	
	VAL_THR_FLEFT(165),
	// VAL_THR_LEFT(400),  // Q
	// VAL_THR_FRONT(250), // Q
	// VAL_THR_RIGHT(400), // Q
	// VAL_THR_LEFT(285),  // H_OLD
	// VAL_THR_FRONT(135), // H_OLD
	// VAL_THR_RIGHT(155), // H_OLD 	
	VAL_THR_LEFT(320),  // H
	VAL_THR_FRONT(145), // H
	VAL_THR_RIGHT(150), // H
	VAL_THR_FRIGHT(185),
	
	VAL_THR_CONTROL_LEFT(100),
	VAL_THR_CONTROL_RIGHT(110),
	
	// VAL_THR_GAP_FLEFT(120), //old
	// VAL_THR_GAP_LEFT(260),  //old
	// VAL_THR_GAP_RIGHT(140), //old
	// VAL_THR_GAP_FRIGHT(120),//old
	VAL_THR_GAP_FLEFT(120),
	VAL_THR_GAP_LEFT(310),
	VAL_THR_GAP_RIGHT(150),
	VAL_THR_GAP_FRIGHT(185),
	VAL_THR_GAP_DIAGO_FLEFT(120),
	VAL_THR_GAP_DIAGO_LEFT(250),
	VAL_THR_GAP_DIAGO_RIGHT(130),
	VAL_THR_GAP_DIAGO_FRIGHT(130),
	
	VAL_THR_SLALOM_FLEFT(500),
	VAL_THR_SLALOM_LEFT(500),
	VAL_THR_SLALOM_RIGHT(500),
	VAL_THR_SLALOM_FRIGHT(500),
	
	THR_WALL_DISAPPEAR(50) // サーキットなら50に，ハーフなら5に
{
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	hadc.Instance = ADC1;
	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.ScanConvMode = DISABLE;
	hadc.Init.ContinuousConvMode = DISABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.NbrOfConversion = 1;
	hadc.Init.DMAContinuousRequests = DISABLE;
	hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	HAL_ADC_Init(&hadc);
	s_config.Rank = 1;
	s_config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	s_config.Channel = ADC_CHANNEL_4|ADC_CHANNEL_5|ADC_CHANNEL_6|ADC_CHANNEL_7|ADC_CHANNEL_8;
	HAL_ADC_ConfigChannel(&hadc, &s_config);

	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct3;
	GPIO_InitStruct3.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
	GPIO_InitStruct3.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct3.Pull = GPIO_NOPULL;
	GPIO_InitStruct3.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct3);
	GPIO_InitStruct3.Pin = GPIO_PIN_10|GPIO_PIN_2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct3);

	thr_straight_value[0] = VAL_THR_FLEFT;
	thr_straight_value[1] = VAL_THR_LEFT;
	thr_straight_value[2] = VAL_THR_FRONT;
	thr_straight_value[3] = VAL_THR_RIGHT;
	thr_straight_value[4] = VAL_THR_FRIGHT;
	ref_straight_value[0] = VAL_REF_FLEFT;
	ref_straight_value[1] = VAL_REF_LEFT;
	ref_straight_value[2] = VAL_REF_FRONT;
	ref_straight_value[3] = VAL_REF_RIGHT;
	ref_straight_value[4] = VAL_REF_FRIGHT;

	had_gap[0] = false;
	had_gap[1] = false;
	is_waiting_gap[0] = false;
	is_waiting_gap[1] = false;

	enabled = true;
}

bool WallSensor::isWorking(){
	return is_working;
}

void WallSensor::onLed(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
}
void WallSensor::onLed(SensorPosition pos){
	switch(pos){
	case SensorPosition::FLeft:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
		break;
	case SensorPosition::Left:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
		break;
	case SensorPosition::Front:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_SET);
		break;
	case SensorPosition::Right:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		break;
	case SensorPosition::FRight:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
		break;
	}
}

void WallSensor::offLed(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
}
void WallSensor::offLed(SensorPosition pos){
	switch(pos){
	case SensorPosition::FLeft:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
		break;
	case SensorPosition::Left:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
		break;
	case SensorPosition::Front:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_RESET);
		break;
	case SensorPosition::Right:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		break;
	case SensorPosition::FRight:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
		break;
	}
}

void WallSensor::start(){
	is_working = true;
}

void WallSensor::stop(){
	is_working = false;
}


void WallSensor::setBrightValue(SensorPosition pos){
	// ADCのフラグクリアしておきたい
	ADC_ChannelConfTypeDef s_config;
	switch(pos){
	case SensorPosition::FLeft:
		s_config.Channel = ADC_CHANNEL_8;
		break;
	case SensorPosition::Left:
		s_config.Channel = ADC_CHANNEL_7;
		break;
	case SensorPosition::Front:
		s_config.Channel = ADC_CHANNEL_6;
		break;
	case SensorPosition::Right:
		s_config.Channel = ADC_CHANNEL_5;
		break;
	case SensorPosition::FRight:
		s_config.Channel = ADC_CHANNEL_4;
		break;
	}
	s_config.Rank = 1;
	s_config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	HAL_ADC_ConfigChannel(&hadc, &s_config);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 10);
	bright_value[static_cast<uint8_t>(pos)] = HAL_ADC_GetValue(&hadc);
}

void WallSensor::setDarkValue(SensorPosition pos){
	// ADCのフラグクリアしておきたい
	ADC_ChannelConfTypeDef s_config;
	switch(pos){
	case SensorPosition::FLeft:
		s_config.Channel = ADC_CHANNEL_8;
		break;
	case SensorPosition::Left:
		s_config.Channel = ADC_CHANNEL_7;
		break;
	case SensorPosition::Front:
		s_config.Channel = ADC_CHANNEL_6;
		break;
	case SensorPosition::Right:
		s_config.Channel = ADC_CHANNEL_5;
		break;
	case SensorPosition::FRight:
		s_config.Channel = ADC_CHANNEL_4;
		break;
	}
	s_config.Rank = 1;
	s_config.SamplingTime = ADC_SAMPLETIME_144CYCLES;
	/// @todo 変換時間は要調整
	HAL_ADC_ConfigChannel(&hadc, &s_config);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 10);
	dark_value[static_cast<uint8_t>(pos)] = HAL_ADC_GetValue(&hadc);
}

void WallSensor::calcValue(uint8_t num){
	for(int i=0; i<5; ++i){
		log_value.at(num).at(i) = bright_value.at(i) - dark_value.at(i);
	}
	return;
}

void WallSensor::setAvgValue(){
	pair<uint16_t, uint16_t> max;
	pair<uint16_t, uint16_t> min;
	for(int i=0; i<4; ++i){
		max.first = 0;
		min.first = 1023;
		last_value.at(i) = current_value.at(i);
		for(int j=0; j<4; ++j){
			if(log_value.at(j).at(i) > max.first){
				max.first = log_value.at(j).at(i);
				max.second = j;
			}
			if(log_value.at(j).at(i) < min.first){
				min.first = log_value.at(j).at(i);
				min.second = j;
			}
		}
		current_value.at(i) = [&]{
			uint16_t ret = 0;
			uint8_t n = (max.second == min.second ? 3 : 2);
			for(int k=0; k<4; ++k){
				if(k != max.second && k != min.second){
					ret += log_value.at(k).at(i);
				}
			}
			return (ret /= n);
		}();
	}
	// buf.push(current_value);
}


void WallSensor::interrupt(){
	if(is_working == false) return;
	
	array<int, 5> tmp = {0};

	s_config.Channel = ADC_CHANNEL_4;
	HAL_ADC_ConfigChannel(&hadc, &s_config);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 10);
	tmp[static_cast<uint8_t>(SensorPosition::FRight)] = HAL_ADC_GetValue(&hadc);
	s_config.Channel = ADC_CHANNEL_5;
	HAL_ADC_ConfigChannel(&hadc, &s_config);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 10);
	tmp[static_cast<uint8_t>(SensorPosition::Right)] = HAL_ADC_GetValue(&hadc);
	s_config.Channel = ADC_CHANNEL_6;
	HAL_ADC_ConfigChannel(&hadc, &s_config);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 10);
	tmp[static_cast<uint8_t>(SensorPosition::Front)] = HAL_ADC_GetValue(&hadc);
	s_config.Channel = ADC_CHANNEL_7;
	HAL_ADC_ConfigChannel(&hadc, &s_config);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 10);
	tmp[static_cast<uint8_t>(SensorPosition::Left)] = HAL_ADC_GetValue(&hadc);
	s_config.Channel = ADC_CHANNEL_8;
	HAL_ADC_ConfigChannel(&hadc, &s_config);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 10);
	tmp[static_cast<uint8_t>(SensorPosition::FLeft)] = HAL_ADC_GetValue(&hadc);

	onLed();
	for(int i=0; i<5000; ++i);
	// 待ち時間を極力減らしたい

	s_config.Channel = ADC_CHANNEL_4;
	HAL_ADC_ConfigChannel(&hadc, &s_config);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 10);
	current_value[static_cast<uint8_t>(SensorPosition::FRight)] = HAL_ADC_GetValue(&hadc) - tmp[static_cast<uint8_t>(SensorPosition::FRight)];
	s_config.Channel = ADC_CHANNEL_5;
	HAL_ADC_ConfigChannel(&hadc, &s_config);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 10);
	current_value[static_cast<uint8_t>(SensorPosition::Right)] = HAL_ADC_GetValue(&hadc) - tmp[static_cast<uint8_t>(SensorPosition::Right)];
	s_config.Channel = ADC_CHANNEL_6;
	HAL_ADC_ConfigChannel(&hadc, &s_config);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 10);
	current_value[static_cast<uint8_t>(SensorPosition::Front)] = HAL_ADC_GetValue(&hadc) - tmp[static_cast<uint8_t>(SensorPosition::Front)];
	s_config.Channel = ADC_CHANNEL_7;
	HAL_ADC_ConfigChannel(&hadc, &s_config);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 10);
	current_value[static_cast<uint8_t>(SensorPosition::Left)] = HAL_ADC_GetValue(&hadc) - tmp[static_cast<uint8_t>(SensorPosition::Left)];
	s_config.Channel = ADC_CHANNEL_8;
	HAL_ADC_ConfigChannel(&hadc, &s_config);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 10);
	current_value[static_cast<uint8_t>(SensorPosition::FLeft)] = HAL_ADC_GetValue(&hadc) - tmp[static_cast<uint8_t>(SensorPosition::FLeft)];

	offLed();
	
	checkGap();
}


uint16_t WallSensor::getValue(SensorPosition pos){
	return current_value[static_cast<uint8_t>(pos)];
}
int16_t WallSensor::getDiffValue(SensorPosition pos){
	return current_value[static_cast<uint8_t>(pos)] - ref_straight_value[static_cast<uint8_t>(pos)];
}
uint16_t WallSensor::getLastValue(SensorPosition pos){
	return last_value[static_cast<uint8_t>(pos)];
}

bool WallSensor::isExistWall(SensorPosition pos){
	if(current_value[static_cast<uint8_t>(pos)] > thr_straight_value[static_cast<uint8_t>(pos)]) return true;
	else return false;
}


float WallSensor::getDistance(SensorPosition pos){
	return param_a.at(static_cast<uint8_t>(pos))/log(getValue(pos))+param_b.at(static_cast<uint8_t>(pos));
}

void WallSensor::calibrate(SensorPosition pos, float x1, float value1, float x2, float value2){
	param_a.at(static_cast<uint8_t>(pos)) = (x1-x2)/(1/log(value1)-1/log(value2));
	param_b.at(static_cast<uint8_t>(pos)) = x1 - param_a.at(static_cast<uint8_t>(pos))/log(value1);
}

void WallSensor::getCalibrationParams(array<float, 5>& a, array<float, 5>& b){
	for (int i=0; i<5; ++i) {
		a.at(i) = param_a.at(i);
		b.at(i) = param_b.at(i);
	}
}


bool WallSensor::canSlalom(){
	if(getValue(SensorPosition::Left) > VAL_THR_SLALOM_LEFT || getValue(SensorPosition::Right) > VAL_THR_SLALOM_RIGHT){
		return false;
	} else if(isExistWall(SensorPosition::Left) && (getValue(SensorPosition::Left) > VAL_THR_SLALOM_LEFT || getValue(SensorPosition::Right) > VAL_THR_SLALOM_RIGHT)){
		return false;
	} else {
		return true;
	}
}

Walldata WallSensor::getWall(){
	Walldata w;
	if(isExistWall(SensorPosition::Left)) w.addWall(MouseAngle::LEFT);
	if(isExistWall(SensorPosition::Right)) w.addWall(MouseAngle::RIGHT);
	if(isExistWall(SensorPosition::Front)) w.addWall(MouseAngle::FRONT);
	return w;
}

void WallSensor::waitGap(SensorPosition sp){
	int tmp;
	if (sp == SensorPosition::Left) tmp = 0;
	else if (sp == SensorPosition::Right) tmp = 1;
	else if (sp == SensorPosition::FLeft) tmp = 2;
	else if (sp == SensorPosition::FRight) tmp = 3;
	is_waiting_gap[tmp] = true;
}

void WallSensor::checkGap(){
	if(!had_gap[0]){
		if(is_waiting_gap[0]
		   && current_value.at(static_cast<uint8_t>(SensorPosition::Left)) < VAL_THR_GAP_LEFT){
			is_waiting_gap[0] = false;
			had_gap[0] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::Left)) > VAL_THR_GAP_LEFT+10){
			is_waiting_gap[0] = true;
		}
	}
	if(!had_gap[1]){
		if(is_waiting_gap[1]
		   && current_value.at(static_cast<uint8_t>(SensorPosition::Right)) < VAL_THR_GAP_RIGHT){
			is_waiting_gap[1] = false;
			had_gap[1] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::Right)) > VAL_THR_GAP_RIGHT+10){
			is_waiting_gap[1] = true;
		}
	}
	if(!had_gap[2]){
		if(is_waiting_gap[2]
		   && current_value.at(static_cast<uint8_t>(SensorPosition::FLeft)) < VAL_THR_GAP_FLEFT){
			is_waiting_gap[2] = false;
			had_gap[2] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::FLeft)) > VAL_THR_GAP_FLEFT+5){
			is_waiting_gap[2] = true;
		}
	}
	if(!had_gap[3]){
		if(is_waiting_gap[3]
		   && current_value.at(static_cast<uint8_t>(SensorPosition::FRight)) < VAL_THR_GAP_FRIGHT){
			is_waiting_gap[3] = false;
			had_gap[3] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::FRight)) > VAL_THR_GAP_FRIGHT+5){
			is_waiting_gap[3] = true;
		}
	}
}

bool WallSensor::hadGap(SensorPosition sp){
	int tmp;
	if (sp == SensorPosition::Left) tmp = 0;
	else if (sp == SensorPosition::Right) tmp = 1;
	else if (sp == SensorPosition::FLeft) tmp = 2;
	else if (sp == SensorPosition::FRight) tmp = 3;
	if(had_gap[tmp]){
		had_gap[tmp] = false;
		return true;
	} else {
		return false;
	}
}


void WallSensor::waitGapDiago(SensorPosition sp){
	int tmp;
	if (sp == SensorPosition::Left) tmp = 0;
	else if (sp == SensorPosition::Right) tmp = 1;
	else if (sp == SensorPosition::FLeft) tmp = 2;
	else if (sp == SensorPosition::FRight) tmp = 3;
	is_waiting_gap_diago[tmp] = true;
}

void WallSensor::checkGapDiago(){
	if(!had_gap_diago[0]){
		if(is_waiting_gap_diago[0]
		   && current_value.at(static_cast<uint8_t>(SensorPosition::Left)) < VAL_THR_GAP_DIAGO_LEFT){
			is_waiting_gap_diago[0] = false;
			had_gap_diago[0] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::Left)) > VAL_THR_GAP_DIAGO_LEFT+5){
			is_waiting_gap_diago[0] = true;
		}
	}
	if(!had_gap_diago[1]){
		if(is_waiting_gap_diago[1]
		   && current_value.at(static_cast<uint8_t>(SensorPosition::Right)) < VAL_THR_GAP_DIAGO_RIGHT){
			is_waiting_gap_diago[1] = false;
			had_gap_diago[1] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::Right)) > VAL_THR_GAP_DIAGO_RIGHT+5){
			is_waiting_gap_diago[1] = true;
		}
	}
	if(!had_gap_diago[2]){
		if(is_waiting_gap_diago[2]
		   && current_value.at(static_cast<uint8_t>(SensorPosition::FLeft)) < VAL_THR_GAP_DIAGO_FLEFT){
			is_waiting_gap_diago[2] = false;
			had_gap_diago[2] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::FLeft)) > VAL_THR_GAP_DIAGO_FLEFT+5){
			is_waiting_gap_diago[2] = true;
		}
	}
	if(!had_gap_diago[3]){
		if(is_waiting_gap_diago[3]
		   && current_value.at(static_cast<uint8_t>(SensorPosition::FRight)) < VAL_THR_GAP_DIAGO_FRIGHT){
			is_waiting_gap_diago[3] = false;
			had_gap_diago[3] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::FRight)) > VAL_THR_GAP_DIAGO_FRIGHT+5){
			is_waiting_gap_diago[3] = true;
		}
	}
}

bool WallSensor::hadGapDiago(SensorPosition sp){
	int tmp;
	if (sp == SensorPosition::Left) tmp = 0;
	else if (sp == SensorPosition::Right) tmp = 1;
	else if (sp == SensorPosition::FLeft) tmp = 2;
	else if (sp == SensorPosition::FRight) tmp = 3;
	if(had_gap_diago[tmp]){
		had_gap_diago[tmp] = false;
		return true;
	} else {
		return false;
	}
}


int16_t WallSensor::getCorrection(uint16_t max){
	if(!enabled) return 0;

	int16_t tmpR = getValue(SensorPosition::Right) - VAL_REF_RIGHT;
	int16_t tmpL = VAL_REF_LEFT - getValue(SensorPosition::Left);
	bool is_singlewall = false;

	// if(current_value[static_cast<uint8_t>(SensorPosition::Left)] > VAL_THR_CONTROL_LEFT && current_value[static_cast<uint8_t>(SensorPosition::Right)] > VAL_THR_CONTROL_RIGHT) return 0;

	if(!isExistWall(SensorPosition::Left) || ((static_cast<int16_t>(getLastValue(SensorPosition::Left))-static_cast<int16_t>(getValue(SensorPosition::Left))) > THR_WALL_DISAPPEAR)){
		tmpL = 0;
		is_singlewall = true;
	}
	if(!isExistWall(SensorPosition::Right) || ((static_cast<int16_t>(getLastValue(SensorPosition::Right))-static_cast<int16_t>(getValue(SensorPosition::Right))) > THR_WALL_DISAPPEAR)){
		tmpR = 0;
		is_singlewall = true;
	}

	int16_t retval = tmpR + tmpL;
	if(is_singlewall) retval *= 2;

	if(abs(retval) > max){
		if(retval > 0){
			return max;
		} else {
			return -1 * max;
		}
	}
	
	return retval;
}

int16_t WallSensor::getCorrectionComb(uint16_t max){
	if(!enabled) return 0;

	int16_t tmpR = getValue(SensorPosition::Right) - VAL_REF_RIGHT;
	int16_t tmpL = VAL_REF_LEFT - getValue(SensorPosition::Left);

	if(tmpR < 0) tmpR = 0;
	if(tmpL > 0) tmpL = 0;

	int16_t retval = tmpR + tmpL;
	
	if(abs(retval) > max){
		if(retval > 0){
			return max;
		} else {
			return -1 * max;
		}
	}
	
	return retval;
}


WallSensor *WallSensor::getInstance(){
	static WallSensor instance;
	return &instance;
}


// void TIM1_BRK_TIM9_IRQHandler(void){
// 	static WallSensor* s = WallSensor::getInstance();
// 	static uint8_t c = 0;
// 	static uint8_t d = 0;

// 	if(TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET){
// 		TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
		
// 		if(s->isWorking()){
// 			/// @todo check enable
// 			switch(c){
// 			case 0:
// 				s->onLed(SensorPosition::FLeft);
// 				s->onLed(SensorPosition::Left);
// 				s->onLed(SensorPosition::Right);
// 				s->onLed(SensorPosition::FRight);
// 				for(int i=0; i<800; ++i);
// 				s->setBrightValue(SensorPosition::FLeft);
// 				s->setBrightValue(SensorPosition::Left);
// 				s->setBrightValue(SensorPosition::Right);
// 				s->setBrightValue(SensorPosition::FRight);
// 				s->offLed(SensorPosition::FLeft);
// 				s->offLed(SensorPosition::Left);
// 				s->offLed(SensorPosition::Right);
// 				s->offLed(SensorPosition::FRight);
// 				for(int i=0; i<800; ++i);
// 				s->setDarkValue(SensorPosition::FLeft);
// 				s->setDarkValue(SensorPosition::Left);
// 				s->setDarkValue(SensorPosition::Right);
// 				s->setDarkValue(SensorPosition::FRight);
// 				s->calcValue(d);
// 				if(++d > 3) d = 0;
// 				break;
// 			case 1:
// 				break;
// 			case 2:
// 				break;
// 			case 3:
// 				s->setAvgValue();
// 				s->checkGap();
// 				s->checkGapDiago();
// 				break;
// 			}
// 			if(++c > 3) c = 0;
// 		}
// 	}
// }
