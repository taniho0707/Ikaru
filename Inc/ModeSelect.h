/*
 * @file ModeSelect.h
 */

#pragma once

#include "stm32f4xx_hal.h"

#include "Led.h"
#include "Speaker.h"
#include "Encoder.h"
#include "Gyro.h"
#include "WallSensor.h"

namespace mode {
	enum class MODE_PRIME : uint8_t {
		TURNADJUST = 0x0,
		SENSORLOG,
		RUNLOG,
		EXPR,
		SHRT,
		HIDARITE,
		LAST // コレより下に定義しない
	};

	enum class MODE_TURNADJUST : uint8_t {
		STRAIGHT_6,
		LAST // コレより下に定義しない
	};

	enum class MODE_SENSORLOG : uint8_t {
		HOGE,
		LAST // コレより下に定義しない
	};

	enum class MODE_RUNLOG : uint8_t {
		MAZE1,
		LAST // コレより下に定義しない
	};

	enum class MODE_EXPR : uint8_t {
		WITHOUT_MAE,
		WITH_MAE,
		LAST // コレより下に定義しない
	};

	enum class MODE_SHRT : uint8_t {
		HOGE,
		LAST // コレより下に定義しない
	};

	enum class MODE_HIDARITE : uint8_t {
		WITHOUT_MAE,
		WITH_MAE,
		LAST // コレより下に定義しない
	};

	struct StructMode {
		uint8_t prime;
		uint8_t sub;
	};
};

class ModeSelect {
private:
	ModeSelect();

	const uint16_t WAITTIME_GYRO;
	const uint16_t WAITTIME_ACCEL;
	const uint16_t WAITTIME_WALL;
	const float THR_UNDER_GYRO_X; //下限値
	const float THR_OVER_GYRO_X;  //上限値
	const float THR_UNDER_GYRO_Y; //下限値
	const float THR_OVER_GYRO_Y;  //上限値
	const float THR_UNDER_GYRO_Z; //下限値
	const float THR_OVER_GYRO_Z;  //上限値
	const float THR_UNDER_ACCEL_X; //下限値
	const float THR_OVER_ACCEL_X;  //上限値
	const float THR_UNDER_ACCEL_Y; //下限値
	const float THR_OVER_ACCEL_Y;  //上限値
	const float THR_UNDER_ACCEL_Z; //下限値
	const float THR_OVER_ACCEL_Z;  //上限値
	const float THR_WALL_FRONT;
	const float THR_WALL_FLEFT;
	const float THR_WALL_FRIGHT;

	uint16_t count_gyro_x;
	uint16_t count_gyro_y;
	uint16_t count_gyro_z;
	uint16_t count_accel_x;
	uint16_t count_accel_y;
	uint16_t count_accel_z;
	uint16_t count_wall_front;
	uint16_t count_wall_fleft;
	uint16_t count_wall_fright;
	uint16_t count_timeout;

	bool enabled;

	uint8_t mode_prime;
	uint8_t mode_sub;

	WallSensor* wallsensor = WallSensor::getInstance();
	Led* led = Led::getInstance();
	Speaker* speaker = Speaker::getInstance();
	Gyro* gyro = Gyro::getInstance();

	uint8_t getModeSubLast(uint8_t prime);

public:
	mode::StructMode select();

	bool startcheck(uint16_t timeout = 0);

	void interrupt();

	static ModeSelect* getInstance();
};
