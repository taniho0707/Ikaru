/**
 * @file ModeSelect.cpp
 */


#include "ModeSelect.h"

using namespace mode;

const static MODE_PRIME      first_mode_prime      = MODE_PRIME::EXPR;
const static MODE_TURNADJUST first_mode_turnadjust = MODE_TURNADJUST::STRAIGHT_6;
const static MODE_SENSORLOG  first_mode_sensorlog  = MODE_SENSORLOG::SHOW;
const static MODE_RUNLOG     first_mode_runlog     = MODE_RUNLOG::MAZE1;
const static MODE_EXPR       first_mode_expr       = MODE_EXPR::NEW_GRAPH;
const static MODE_SHRT       first_mode_shrt       = MODE_SHRT::SMALL1;
const static MODE_HIDARITE   first_mode_hidarite   = MODE_HIDARITE::WITHOUT_MAE;


ModeSelect::ModeSelect() :
	WAITTIME_GYRO(50),
	WAITTIME_ACCEL(100),
	WAITTIME_WALL(500),
	THR_UNDER_GYRO_X(500),
	THR_OVER_GYRO_X(15000), //Pitch
	THR_UNDER_GYRO_Y(500),
	THR_OVER_GYRO_Y(15000), //Roll
	THR_UNDER_GYRO_Z(500),
	THR_OVER_GYRO_Z(10000), //Yaw
	THR_UNDER_ACCEL_X(100),
	THR_OVER_ACCEL_X(10000), //左右
	THR_UNDER_ACCEL_Y(100),
	THR_OVER_ACCEL_Y(10000), //前後
	THR_UNDER_ACCEL_Z(100),
	THR_OVER_ACCEL_Z(0), //G
	THR_WALL_FRONT(500),
	THR_WALL_FLEFT(500),
	THR_WALL_FRIGHT(500)
{
	count_gyro_x = 0;
	count_gyro_y = 0;
	count_gyro_z = 0;
	count_accel_x = 0;
	count_accel_y = 0;
	count_accel_z = 0;
	count_wall_front = 0;
	count_wall_fleft = 0;
	count_wall_fright = 0;

	enabled = false;
}


uint8_t ModeSelect::getModeSubLast(uint8_t prime){
	switch(static_cast<MODE_PRIME>(prime)){
	case MODE_PRIME::TURNADJUST:
		return static_cast<uint8_t>(MODE_TURNADJUST::LAST);
		break;
	case MODE_PRIME::SENSORLOG:
		return static_cast<uint8_t>(MODE_SENSORLOG::LAST);
		break;
	case MODE_PRIME::RUNLOG:
		return static_cast<uint8_t>(MODE_RUNLOG::LAST);
		break;
	case MODE_PRIME::EXPR:
		return static_cast<uint8_t>(MODE_EXPR::LAST);
		break;
	case MODE_PRIME::SHRT:
		return static_cast<uint8_t>(MODE_SHRT::LAST);
		break;
	case MODE_PRIME::HIDARITE:
		return static_cast<uint8_t>(MODE_HIDARITE::LAST);
		break;
	}
}

mode::StructMode ModeSelect::select(){
	uint16_t stable_time1 = 0, stable_time2 = 0;
	int16_t ax, ay, az, gx, gy, gz;
	mode_prime = static_cast<uint8_t>(first_mode_prime);
	mode_sub = 0; /// @todo 初期値代入する
	mode_number = 0; /// @todo 初期値代入する
	count_gyro_x = 0;
	count_gyro_y = 0;
	count_gyro_z = 0;
	enabled = true;
	while(true){
		gx = gyro->readGyroX();
		gy = gyro->readGyroY();
		gz = gyro->readGyroZ();
		if(abs(count_gyro_y) > WAITTIME_GYRO){
			int16_t ad = 0;
			if(count_gyro_y > 0){
				ad = -1;
			} else {
				ad = 1;
			}
			if(ad == -1 && mode_prime != 0){
				speaker->playMusic(MusicNumber::KIRBY3_ORIG_DESELECT1);
				-- mode_prime;
				mode_sub = 0;
			} else if(ad == 1 && mode_prime != static_cast<uint8_t>(MODE_PRIME::LAST)-1){
				speaker->playMusic(MusicNumber::KIRBY3_SELECT1);
				++ mode_prime;
				mode_sub = 0;
			} else {
				speaker->playMusic(MusicNumber::KIRBY_DYING);
			}
			HAL_Delay(300);
			count_gyro_y = 0;
		}

		if(abs(count_gyro_x) > WAITTIME_GYRO){
			int16_t ad = 0;
			if(count_gyro_x > 0){
				ad = 1;
			} else {
				ad = -1;
			}
			if(ad == -1 && mode_sub != 0){
				speaker->playMusic(MusicNumber::KIRBY3_ORIG_DESELECT2);
				-- mode_sub;
			} else if(ad == 1 && mode_sub != getModeSubLast(mode_prime)-1){
				speaker->playMusic(MusicNumber::KIRBY3_SELECT2);
				++ mode_sub;
			} else {
				speaker->playMusic(MusicNumber::KIRBY_DYING);
			}
			HAL_Delay(300);
			count_gyro_x = 0;
		}

		if(abs(count_gyro_z) > WAITTIME_GYRO){
			int16_t ad = 0;
			if(count_gyro_z > 0){
				ad = -1;
			} else {
				ad = 1;
			}
			if(ad == -1 && mode_number != 0){
				speaker->playMusic(MusicNumber::KIRBY3_ORIG_DESELECT3);
				-- mode_number;
			} else if(ad == 1 && mode_number != 2){ /// @todo fix max number
				speaker->playMusic(MusicNumber::KIRBY3_SELECT3);
				++ mode_number;
			} else {
				speaker->playMusic(MusicNumber::KIRBY_DYING);
			}
			HAL_Delay(300);
			count_gyro_z = 0;
		}
		
		if(abs(gx) < THR_UNDER_GYRO_X && abs(gy) < THR_UNDER_GYRO_Y && abs(gz) < THR_UNDER_GYRO_Z){
			led->on(LedNumbers::FRONT);
		} else {
			stable_time1 = 0;
			led->off(LedNumbers::FRONT);
		}
		if(stable_time1++ > 100){
			break;
		}
		HAL_Delay(10);
	}

	enabled = false;
	StructMode ret_mode;
	ret_mode.prime = mode_prime;
	ret_mode.sub = mode_sub;
	ret_mode.number = mode_number;
	return ret_mode;
}

bool ModeSelect::startcheck(uint16_t timeout){
	count_timeout = 0;
	count_wall_fleft = 0;
	count_wall_fright = 0;
	enabled = true;
	while(1){
		if(count_wall_fleft > WAITTIME_WALL && count_wall_fright > WAITTIME_WALL) {
			enabled = false;
			return true;
		} else if(timeout != 0 && count_timeout > timeout){
			enabled = false;
			return false;
		}
		HAL_Delay(1);
	}
}

void ModeSelect::interrupt(){
	if (!enabled) return;
	if (wallsensor->getValue(SensorPosition::FLeft) > THR_WALL_FLEFT) {
		++ count_wall_fleft;
	} else {
		count_wall_fleft = 0;
	}
	if (wallsensor->getValue(SensorPosition::FRight) > THR_WALL_FRIGHT) {
		++ count_wall_fright;
	} else {
		count_wall_fright = 0;
	}
	static int16_t gx, gy, gz;
	gx = gyro->readGyroX();
	gy = gyro->readGyroY();
	gz = gyro->readGyroZ();
	if (abs(gx) > THR_OVER_GYRO_X) {
		if (gx > 0) ++ count_gyro_x;
		else -- count_gyro_x;
	} else {
		count_gyro_x = 0;
	}
	if (abs(gy) > THR_OVER_GYRO_Y) {
		if (gy > 0) ++ count_gyro_y;
		else -- count_gyro_y;
	} else {
		count_gyro_y = 0;
	}
	if (abs(gz) > THR_OVER_GYRO_Z) {
		if (gz > 0) ++ count_gyro_z;
		else -- count_gyro_z;
	} else {
		count_gyro_z = 0;
	}
}

ModeSelect* ModeSelect::getInstance(){
	static ModeSelect instance;
	return &instance;
}

