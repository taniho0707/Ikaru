/**
 * @file ModeSelect.cpp
 */


#include "ModeSelect.h"

using namespace mode;

MODE_PRIME      first_mode_prime      = MODE_PRIME::EXPR;
MODE_TURNADJUST first_mode_turnadjust = MODE_TURNADJUST::LAST;
MODE_SENSORLOG  first_mode_sensorlog  = MODE_SENSORLOG::LAST;
MODE_RUNLOG     first_mode_runlog     = MODE_RUNLOG::LAST;
MODE_EXPR       first_mode_expr       = MODE_EXPR::LAST;
MODE_SHRT       first_mode_shrt       = MODE_SHRT::LAST;
MODE_HIDARITE   first_mode_hidarite   = MODE_HIDARITE::LAST;


ModeSelect::ModeSelect() :
	WAITTIME_GYRO(10),
	WAITTIME_ACCEL(100),
	WAITTIME_WALL(1000),
	THR_UNDER_GYRO_X(100),
	THR_OVER_GYRO_X(15000), //Pitch
	THR_UNDER_GYRO_Y(100),
	THR_OVER_GYRO_Y(15000), //Roll
	THR_UNDER_GYRO_Z(100),
	THR_OVER_GYRO_Z(10000), //Yaw
	THR_UNDER_ACCEL_X(0),
	THR_OVER_ACCEL_X(10000), //左右
	THR_UNDER_ACCEL_Y(0),
	THR_OVER_ACCEL_Y(10000), //前後
	THR_UNDER_ACCEL_Z(0),
	THR_OVER_ACCEL_Z(0), //G
	THR_WALL_FRONT(1000),
	THR_WALL_FLEFT(1000),
	THR_WALL_FRIGHT(1000)
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


mode::StructMode ModeSelect::select(){
	
}

bool ModeSelect::startcheck(uint16_t timeout){
	count_timeout = 0;
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
}

ModeSelect* ModeSelect::getInstance(){
	static ModeSelect instance;
	return &instance;
}

