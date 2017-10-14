/**
 * @file MotorControl.h
 */
#pragma once

#include "stm32f4xx_hal.h"

#include <vector>

#include "WallSensor.h"
#include "Encoder.h"
#include "Motor.h"
#include "Gyro.h"
#include "Led.h"
#include "Speaker.h"

#include "Datalog.h"
#include "GapCounter.h"

class MotorControl{
private:
	explicit MotorControl();

	Motor* motor = Motor::getInstance();
	Encoder* encoder = Encoder::getInstance();
	WallSensor* wall = WallSensor::getInstance();
	Gyro* gyro = Gyro::getInstance();
	Led* led = Led::getInstance();

	Datalog* log = Datalog::getInstance();

	const float GAIN_LIN_P;
	const float GAIN_LIN_I;
	const float GAIN_LIN_D;
	const float GAIN_RAD_P;
	const float GAIN_RAD_I;
	const float GAIN_RAD_D;
	const float GAIN_WALL_P;
	const float GAIN_WALL_SHRT_P;
	const float GAIN_WALL_I;
	const float GAIN_WALL_D;

	const float TREAD;

	float cur_lin_x;
	float cur_lin_vel;
	float cur_lin_acc;

	float tar_lin_vel;
	float tar_rad_vel;

	int16_t integral_wall;
	float integral_rad_gyro;
	float integral_lin_encoder;

	float difference_lin_encoder;
	float last_lin_encoder;

	float dist_from_gap;
	bool is_left_gap;
	float dist_from_gap_diago;
	bool is_left_gap_diago;
	bool is_expr_gap;

	bool enabled_wall_control;
	bool is_shrt_wall_control;
	bool is_comb_wall_control;

	bool is_failsafe;

	void calcIntegral();
	void controlX();
	void controlVel();

public:
	void resetWallIntegral();
	void resetRadIntegral();
	void resetLinIntegral();

	void setIntegralEncoder(float);
	float getIntegralEncoder();

	void resetDistanceFromGap();
	float getDistanceFromGap();
	bool isLeftGap();
	void resetDistanceFromGapDiago();
	float getDistanceFromGapDiago();
	bool isLeftGapDiago();
	void setExprGap();
	void setShrtGap();

	void setVelocity(float);
	void setRadVelocity(float);

	void enableWallControl();
	void disableWallControl();

	void setExprWallControl();
	void setShrtWallControl();

	void setCombWallControl();
	void resetCombWallControl();

	void clearGap();

	void stay();

	void interrupt();

	static MotorControl *getInstance();
};
