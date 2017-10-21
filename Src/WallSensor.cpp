
#include "WallSensor.h"

using namespace std;

// 壁切れの閾値を10から30にした

/// @todo add wait REDEN flag
WallSensor::WallSensor() :
	VAL_REF_FLEFT(190),
	// VAL_REF_LEFT(630),   // Q
	// VAL_REF_FRONT(1150), // Q
	// VAL_REF_RIGHT(780),  // Q
	// VAL_REF_LEFT(300),  // H_TOHOKU
	// VAL_REF_FRONT(360), // H_TOHOKU
	// VAL_REF_RIGHT(245), // H_TOHOKU
	VAL_REF_LEFT(200),  // H
	VAL_REF_FRONT(360), // H
	VAL_REF_RIGHT(230), // H
	VAL_REF_FRIGHT(120),
	
	VAL_THR_FLEFT(165),
	// VAL_THR_LEFT(400),  // Q
	// VAL_THR_FRONT(250), // Q
	// VAL_THR_RIGHT(400), // Q
	// VAL_THR_LEFT(250),  // H_TOHOKU
	// VAL_THR_FRONT(145), // H_TOHOKU
	// VAL_THR_RIGHT(170), // H_TOHOKU
	VAL_THR_LEFT(160),  // H
	VAL_THR_FRONT(110), // H
	VAL_THR_RIGHT(190), // H
	VAL_THR_FRIGHT(185),
	VAL_THR_FRONT_SUB(200),
	
	VAL_THR_CONTROL_LEFT(100),
	VAL_THR_CONTROL_RIGHT(110),
	
	// VAL_THR_GAP_FLEFT(120), //TOHOKU
	// VAL_THR_GAP_LEFT(220),  //TOHOKU
	// VAL_THR_GAP_RIGHT(140), //TOHOKU
	// VAL_THR_GAP_FRIGHT(120),//TOHOKU
	VAL_THR_GAP_FLEFT(120),
	VAL_THR_GAP_LEFT(145),
	VAL_THR_GAP_RIGHT(180),
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

	// loadParams();
	// 必ず初期化する

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
	for(int i=0; i<5; ++i){
		max.first = 0;
		min.first = 1023;
		last_value.at(i) = current_value.at(i);
		for(int j=0; j<5; ++j){
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
			for(int k=0; k<5; ++k){
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

	// setAvgValue();
	
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

bool WallSensor::isExistFrontWall() {
	if(current_value[static_cast<uint8_t>(SensorPosition::Front)] > valid_val_thr_front_sub) return true;
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
	if(getValue(SensorPosition::Left) > valid_val_thr_slalom_left || getValue(SensorPosition::Right) > valid_val_thr_slalom_right){
		return false;
	} else if(isExistWall(SensorPosition::Left) && (getValue(SensorPosition::Left) > valid_val_thr_slalom_left || getValue(SensorPosition::Right) > valid_val_thr_slalom_right)){
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
		   && current_value.at(static_cast<uint8_t>(SensorPosition::Left)) < valid_val_thr_gap_left){
			is_waiting_gap[0] = false;
			had_gap[0] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::Left)) > valid_val_thr_gap_left+10){
			is_waiting_gap[0] = true;
		}
	}
	if(!had_gap[1]){
		if(is_waiting_gap[1]
		   && current_value.at(static_cast<uint8_t>(SensorPosition::Right)) < valid_val_thr_gap_right){
			is_waiting_gap[1] = false;
			had_gap[1] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::Right)) > valid_val_thr_gap_right+10){
			is_waiting_gap[1] = true;
		}
	}
	if(!had_gap[2]){
		if(is_waiting_gap[2]
		   && current_value.at(static_cast<uint8_t>(SensorPosition::FLeft)) < valid_val_thr_gap_fleft){
			is_waiting_gap[2] = false;
			had_gap[2] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::FLeft)) > valid_val_thr_gap_fleft+5){
			is_waiting_gap[2] = true;
		}
	}
	if(!had_gap[3]){
		if(is_waiting_gap[3]
		   && current_value.at(static_cast<uint8_t>(SensorPosition::FRight)) < valid_val_thr_gap_fright){
			is_waiting_gap[3] = false;
			had_gap[3] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::FRight)) > valid_val_thr_gap_fright+5){
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
		   && current_value.at(static_cast<uint8_t>(SensorPosition::Left)) < valid_val_thr_gap_diago_left){
			is_waiting_gap_diago[0] = false;
			had_gap_diago[0] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::Left)) > valid_val_thr_gap_diago_left+5){
			is_waiting_gap_diago[0] = true;
		}
	}
	if(!had_gap_diago[1]){
		if(is_waiting_gap_diago[1]
		   && current_value.at(static_cast<uint8_t>(SensorPosition::Right)) < valid_val_thr_gap_diago_right){
			is_waiting_gap_diago[1] = false;
			had_gap_diago[1] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::Right)) > valid_val_thr_gap_diago_right+5){
			is_waiting_gap_diago[1] = true;
		}
	}
	if(!had_gap_diago[2]){
		if(is_waiting_gap_diago[2]
		   && current_value.at(static_cast<uint8_t>(SensorPosition::FLeft)) < valid_val_thr_gap_diago_fleft){
			is_waiting_gap_diago[2] = false;
			had_gap_diago[2] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::FLeft)) > valid_val_thr_gap_diago_fleft+5){
			is_waiting_gap_diago[2] = true;
		}
	}
	if(!had_gap_diago[3]){
		if(is_waiting_gap_diago[3]
		   && current_value.at(static_cast<uint8_t>(SensorPosition::FRight)) < valid_val_thr_gap_diago_fright){
			is_waiting_gap_diago[3] = false;
			had_gap_diago[3] = true;
		} else if(current_value.at(static_cast<uint8_t>(SensorPosition::FRight)) > valid_val_thr_gap_diago_fright+5){
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

	int16_t tmpR = getValue(SensorPosition::Right) - valid_val_ref_right;
	int16_t tmpL = valid_val_ref_left - getValue(SensorPosition::Left);
	bool is_singlewall = false;

	// if(current_value[static_cast<uint8_t>(SensorPosition::Left)] > VAL_THR_CONTROL_LEFT && current_value[static_cast<uint8_t>(SensorPosition::Right)] > VAL_THR_CONTROL_RIGHT) return 0;

	if(!isExistWall(SensorPosition::Left) || ((static_cast<int16_t>(getLastValue(SensorPosition::Left))-static_cast<int16_t>(getValue(SensorPosition::Left))) > valid_thr_wall_disappear)){
		tmpL = 0;
		is_singlewall = true;
	}
	if(!isExistWall(SensorPosition::Right) || ((static_cast<int16_t>(getLastValue(SensorPosition::Right))-static_cast<int16_t>(getValue(SensorPosition::Right))) > valid_thr_wall_disappear)){
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

	int16_t tmpR = getValue(SensorPosition::Right) - valid_val_ref_right;
	int16_t tmpL = valid_val_ref_left - getValue(SensorPosition::Left);

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


uint32_t WallSensor::getParamsHash(){
	std::hash<std::string> hash_fn;
	std::string parameters = "";
	parameters += to_string(VAL_REF_FLEFT);
	parameters += to_string(VAL_REF_LEFT);
	parameters += to_string(VAL_REF_FRONT);
	parameters += to_string(VAL_REF_RIGHT);
	parameters += to_string(VAL_REF_FRIGHT);
	parameters += to_string(VAL_THR_FLEFT);
	parameters += to_string(VAL_THR_LEFT);
	parameters += to_string(VAL_THR_FRONT);
	parameters += to_string(VAL_THR_RIGHT);
	parameters += to_string(VAL_THR_FRIGHT);
	parameters += to_string(THR_WALL_DISAPPEAR);
	parameters += to_string(VAL_THR_CONTROL_LEFT);
	parameters += to_string(VAL_THR_CONTROL_RIGHT);
	parameters += to_string(VAL_THR_GAP_FLEFT);
	parameters += to_string(VAL_THR_GAP_LEFT);
	parameters += to_string(VAL_THR_GAP_RIGHT);
	parameters += to_string(VAL_THR_GAP_FRIGHT);
	parameters += to_string(VAL_THR_GAP_DIAGO_FLEFT);
	parameters += to_string(VAL_THR_GAP_DIAGO_LEFT);
	parameters += to_string(VAL_THR_GAP_DIAGO_RIGHT);
	parameters += to_string(VAL_THR_GAP_DIAGO_FRIGHT);
	parameters += to_string(VAL_THR_SLALOM_FLEFT);
	parameters += to_string(VAL_THR_SLALOM_LEFT);
	parameters += to_string(VAL_THR_SLALOM_RIGHT);
	parameters += to_string(VAL_THR_SLALOM_FRIGHT);
	parameters += to_string(VAL_THR_FRONT_SUB);
	uint32_t hash = static_cast<uint32_t>(hash_fn(parameters));
	return hash;
}

bool WallSensor::isLatestFram(){
	Fram* fram = Fram::getInstance();
	uint32_t hash_fram = fram->readUInt32(1880);
	uint32_t hash_flash = getParamsHash();
	if (hash_fram == hash_flash) return true;
	else return false;
}

void WallSensor::saveParamsToFram(){
	Fram* fram = Fram::getInstance();
	uint16_t addr = 1884;
	fram->writeUInt16(valid_val_ref_fleft, addr); addr+=2;
	fram->writeUInt16(valid_val_ref_left, addr); addr+=2;
	fram->writeUInt16(valid_val_ref_front, addr); addr+=2;
	fram->writeUInt16(valid_val_ref_right, addr); addr+=2;
	fram->writeUInt16(valid_val_ref_fright, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_fleft, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_left, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_front, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_right, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_fright, addr); addr+=2;
	fram->writeInt16(valid_thr_wall_disappear, addr); addr+=2;
	fram->writeInt16(valid_val_thr_control_left, addr); addr+=2;
	fram->writeInt16(valid_val_thr_control_right, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_gap_fleft, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_gap_left, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_gap_right, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_gap_fright, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_gap_diago_fleft, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_gap_diago_left, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_gap_diago_right, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_gap_diago_fright, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_slalom_fleft, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_slalom_left, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_slalom_right, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_slalom_fright, addr); addr+=2;
	fram->writeUInt16(valid_val_thr_front_sub, addr); addr+=2;
}

void WallSensor::loadParamsFromFram(){
	Fram* fram = Fram::getInstance();
	uint16_t addr = 1884;
	valid_val_ref_fleft = fram->readUInt16(addr); addr+=2;
	valid_val_ref_left = fram->readUInt16(addr); addr+=2;
	valid_val_ref_front = fram->readUInt16(addr); addr+=2;
	valid_val_ref_right = fram->readUInt16(addr); addr+=2;
	valid_val_ref_fright = fram->readUInt16(addr); addr+=2;
	valid_val_thr_fleft = fram->readUInt16(addr); addr+=2;
	valid_val_thr_left = fram->readUInt16(addr); addr+=2;
	valid_val_thr_front = fram->readUInt16(addr); addr+=2;
	valid_val_thr_right = fram->readUInt16(addr); addr+=2;
	valid_val_thr_fright = fram->readUInt16(addr); addr+=2;
	valid_thr_wall_disappear = fram->readInt16(addr); addr+=2;
	valid_val_thr_control_left = fram->readInt16(addr); addr+=2;
	valid_val_thr_control_right = fram->readInt16(addr); addr+=2;
	valid_val_thr_gap_fleft = fram->readUInt16(addr); addr+=2;
	valid_val_thr_gap_left = fram->readUInt16(addr); addr+=2;
	valid_val_thr_gap_right = fram->readUInt16(addr); addr+=2;
	valid_val_thr_gap_fright = fram->readUInt16(addr); addr+=2;
	valid_val_thr_gap_diago_fleft = fram->readUInt16(addr); addr+=2;
	valid_val_thr_gap_diago_left = fram->readUInt16(addr); addr+=2;
	valid_val_thr_gap_diago_right = fram->readUInt16(addr); addr+=2;
	valid_val_thr_gap_diago_fright = fram->readUInt16(addr); addr+=2;
	valid_val_thr_slalom_fleft = fram->readUInt16(addr); addr+=2;
	valid_val_thr_slalom_left = fram->readUInt16(addr); addr+=2;
	valid_val_thr_slalom_right = fram->readUInt16(addr); addr+=2;
	valid_val_thr_slalom_fright = fram->readUInt16(addr); addr+=2;
	valid_val_thr_front_sub = fram->readUInt16(addr); addr+=2;
	
	thr_straight_value[0] = valid_val_thr_fleft;
	thr_straight_value[1] = valid_val_thr_left;
	thr_straight_value[2] = valid_val_thr_front;
	thr_straight_value[3] = valid_val_thr_right;
	thr_straight_value[4] = valid_val_thr_fright;
	ref_straight_value[0] = valid_val_ref_fleft;
	ref_straight_value[1] = valid_val_ref_left;
	ref_straight_value[2] = valid_val_ref_front;
	ref_straight_value[3] = valid_val_ref_right;
	ref_straight_value[4] = valid_val_ref_fright;
}

void WallSensor::loadParamsFromFlash(){
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

	valid_val_ref_fleft            = VAL_REF_FLEFT;
	valid_val_ref_left             = VAL_REF_LEFT;
	valid_val_ref_front            = VAL_REF_FRONT;
	valid_val_ref_right            = VAL_REF_RIGHT;
	valid_val_ref_fright           = VAL_REF_FRIGHT;
	valid_val_thr_fleft            = VAL_THR_FLEFT;
	valid_val_thr_left             = VAL_THR_LEFT;
	valid_val_thr_front            = VAL_THR_FRONT;
	valid_val_thr_right            = VAL_THR_RIGHT;
	valid_val_thr_fright           = VAL_THR_FRIGHT;
	valid_thr_wall_disappear       = THR_WALL_DISAPPEAR;
	valid_val_thr_control_left     = VAL_THR_CONTROL_LEFT;
	valid_val_thr_control_right    = VAL_THR_CONTROL_RIGHT;
	valid_val_thr_gap_fleft        = VAL_THR_GAP_FLEFT;
	valid_val_thr_gap_left         = VAL_THR_GAP_LEFT;
	valid_val_thr_gap_right        = VAL_THR_GAP_RIGHT;
	valid_val_thr_gap_fright       = VAL_THR_GAP_FRIGHT;
	valid_val_thr_gap_diago_fleft  = VAL_THR_GAP_DIAGO_FLEFT;
	valid_val_thr_gap_diago_left   = VAL_THR_GAP_DIAGO_LEFT;
	valid_val_thr_gap_diago_right  = VAL_THR_GAP_DIAGO_RIGHT;
	valid_val_thr_gap_diago_fright = VAL_THR_GAP_DIAGO_FRIGHT;
	valid_val_thr_slalom_fleft     = VAL_THR_SLALOM_FLEFT;
	valid_val_thr_slalom_left      = VAL_THR_SLALOM_LEFT;
	valid_val_thr_slalom_right     = VAL_THR_SLALOM_RIGHT;
	valid_val_thr_slalom_fright    = VAL_THR_SLALOM_FRIGHT;
	valid_val_thr_front_sub        = VAL_THR_FRONT_SUB;
}

void WallSensor::loadParams(){
	if (isLatestFram()) {
		// loadParamsFromFram();
		loadParamsFromFlash();
		/// @todo 元に戻す
	} else {
		loadParamsFromFlash();
		Fram* fram = Fram::getInstance();
		uint32_t hash = getParamsHash();
		fram->writeUInt32(hash, 1880);
	}
}

void WallSensor::refleshStraigntValue(){
	thr_straight_value[0] = valid_val_thr_fleft;
	thr_straight_value[1] = valid_val_thr_left;
	thr_straight_value[2] = valid_val_thr_front;
	thr_straight_value[3] = valid_val_thr_right;
	thr_straight_value[4] = valid_val_thr_fright;
	ref_straight_value[0] = valid_val_ref_fleft;
	ref_straight_value[1] = valid_val_ref_left;
	ref_straight_value[2] = valid_val_ref_front;
	ref_straight_value[3] = valid_val_ref_right;
	ref_straight_value[4] = valid_val_ref_fright;
}


WallSensor *WallSensor::getInstance(){
	static WallSensor instance;
	return &instance;
}


// void TIM7_IRQHandler() {
// 	static WallSensor* s = WallSensor::getInstance();
// 	static uint8_t c = 0;
// 	static uint8_t d = 0;

// 	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET){
// 		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
		
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
