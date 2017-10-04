#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"

#include <array>
#include <cmath>
#include "RingBufferHistory.h"

#include "Walldata.h"

using namespace std;


enum class SensorPosition : uint8_t{
	FLeft,
	Left,
	Front,
	Right,
	FRight,
};

class WallSensor{
private:
	const uint16_t VAL_REF_FLEFT;
	const uint16_t VAL_REF_LEFT;
	const uint16_t VAL_REF_FRONT;
	const uint16_t VAL_REF_RIGHT;
	const uint16_t VAL_REF_FRIGHT;
	const uint16_t VAL_THR_FLEFT;
	const uint16_t VAL_THR_LEFT;
	const uint16_t VAL_THR_FRONT;
	const uint16_t VAL_THR_RIGHT;
	const uint16_t VAL_THR_FRIGHT;
	const int16_t THR_WALL_DISAPPEAR;
	const int16_t VAL_THR_CONTROL_LEFT;
	const int16_t VAL_THR_CONTROL_RIGHT;
	const uint16_t VAL_THR_GAP_FLEFT;
	const uint16_t VAL_THR_GAP_LEFT;
	const uint16_t VAL_THR_GAP_RIGHT;
	const uint16_t VAL_THR_GAP_FRIGHT;
	const uint16_t VAL_THR_GAP_DIAGO_FLEFT;
	const uint16_t VAL_THR_GAP_DIAGO_LEFT;
	const uint16_t VAL_THR_GAP_DIAGO_RIGHT;
	const uint16_t VAL_THR_GAP_DIAGO_FRIGHT;
	const uint16_t VAL_THR_SLALOM_FLEFT;
	const uint16_t VAL_THR_SLALOM_LEFT;
	const uint16_t VAL_THR_SLALOM_RIGHT;
	const uint16_t VAL_THR_SLALOM_FRIGHT;

	ADC_HandleTypeDef hadc;
	ADC_ChannelConfTypeDef s_config;

	RingBufferHistory< array<float, 5>, 10 > buf;

	bool is_working;
	bool enabled;
	array<uint16_t, 5> ref_straight_value;
	array<uint16_t, 5> thr_straight_value;

	// 値<->距離の変換式用パラメータ y=a/ln(x)+b
	array<float, 5> param_a;
	array<float, 5> param_b;

	// <float, 4>, 4 だった
	array< array<float, 5>, 5 > log_value;

	// 壁切れ用
	bool had_gap[4];
	bool is_waiting_gap[4];
	bool had_gap_diago[4];
	bool is_waiting_gap_diago[4];

	WallSensor();

public:
	array<float, 5> dark_value;
	array<float, 5> bright_value;
	array<float, 5> current_value;
	array<float, 5> last_value;

	bool isWorking();

	void enable();
	void disable();

	void start();
	void stop();

	void interrupt();

	inline void onLed();
	inline void onLed(SensorPosition);
	inline void offLed();
	inline void offLed(SensorPosition);

	void setBrightValue(SensorPosition);
	void setDarkValue(SensorPosition);
	void calcValue(uint8_t num);
	void setAvgValue();

	uint16_t getValue(SensorPosition);
	int16_t getDiffValue(SensorPosition);
	uint16_t getLastValue(SensorPosition);
	bool isExistWall(SensorPosition);

	float getDistance(SensorPosition);

	void calibrate(SensorPosition, float x1, float value1, float x2, float value2);
	void getCalibrationParams(array<float, 5>& a, array<float, 5>& b);

	bool canSlalom();

	Walldata getWall();

	void waitGap(SensorPosition);
	bool hadGap(SensorPosition);
	void checkGap();

	void waitGapDiago(SensorPosition);
	bool hadGapDiago(SensorPosition);
	void checkGapDiago();

	int16_t getCorrection(uint16_t max);
	int16_t getCorrectionComb(uint16_t max);

	static WallSensor* getInstance();
};

