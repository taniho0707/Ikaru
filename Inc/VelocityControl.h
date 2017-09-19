#pragma once

#include "SlalomParams.h"

#include <cmath>
#define ARM_MATH_CM4
#define __FPU_PRESENT 1U
#include "arm_const_structs.h"
#include "arm_math.h"

#include "Speaker.h"
#include "Motor.h"
#include "MotorControl.h"
#include "WallSensor.h"
#include "Gyro.h"
/* #include "Map.h" */
#include "Timer.h"

/* #include "ComPc.h" */

class VelocityControl{
private:
	MotorControl* mc = MotorControl::getInstance();
	WallSensor* sens = WallSensor::getInstance();
	Gyro* gyro = Gyro::getInstance();
	Led* led = Led::getInstance();
	Speaker* speaker = Speaker::getInstance();
	/* Map map; */

	const float DIST_GAP_FROM_R;
	const float DIST_GAP_FROM_L;

	int32_t time;
	bool end_flag;
	bool is_started;
	bool has_done_slalom;

	bool enabled_wallgap;
	bool is_left_gap;

	enum slalomparams::RunType reg_type;

	// for Trapezoid and Pivot ( and Slalom)
	float reg_start_vel;
	float reg_max_vel;
	float reg_end_vel;
	float reg_distance;
	float reg_accel;

	// for Slalom
	float reg_d_before;
	float reg_d_after;
	int32_t reg_slalom_pos;
	float reg_min_vel;

	float target_linvel;
	float target_radvel;

	void calcTrapAccel(int32_t t);
	void calcTrapDiago(int32_t t);
	void calcPivotTurn(int32_t t);
	void calcSlalom(int32_t t);

	float x1, x2, x3, v, r;
	int32_t t1, t2, t3, t4;

	VelocityControl();

public:
	void setLinVel();
	void setRadVel();

	bool isRunning();
	bool hasDoneSlalom();

	void enableWallgap();
	void disableWallgap();

	// 加速もしくは等速のみ
	// 指定距離が来た場合走り続けるため，オーバーランの可能性あり
	// 他の走行を指定することで距離を残したまま継続
	void startTrapAccel(
		float start_vel,
		float max_vel,
		float distance,
		float accel
		);

	void runTrapAccel(
		float start_vel,
		float max_vel,
		float end_vel,
		float distance,
		float accel
		);
	void runTrapDiago(
		float start_vel,
		float max_vel,
		float end_vel,
		float distance,
		float accel
		);

	void runPivotTurn(
		float max_vel,
		float degree,
		float accel
		);

	bool runSlalom(
		slalomparams::RunType type,
		float vel
		);

	/// @brief 一定周期(周期はT<制御周期)ごとに呼び出してMotorControlへ適切な速度・角速度を出力する
	void interrupt();

	static VelocityControl* getInstance();
};
