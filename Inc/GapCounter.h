/*
 * @file GapCounter.h
 */

#pragma once

#include "Encoder.h"
/* #include "VelocityControl.h" */
#include "Map.h"
#include "WallSensor.h"
#include "Speaker.h"

#include "Datalog.h"

enum class GAPSTATUS : uint8_t {
	NOTHING,    // 壁切れ無し
	CONCURRENT, // 進行方向に並行    ｜
	VERTICAL,   // 進行方向に垂直のみ ー
};

class GapCounter {
private:
	const float GAP_DISTANCE_LEFT;
	const float GAP_DISTANCE_RIGHT;

	void start();
	void stop();

	void analyzeGap(int8_t x, int8_t y, MazeAngle angle, uint8_t num);

	GapCounter();

	bool running;

	Map map;
	float passed_distance;
	float target_distance;

public:
	std::array<GAPSTATUS, 30> gap_left;
	std::array<GAPSTATUS, 30> gap_right;
private:

	WallSensor* wallsensor = WallSensor::getInstance();
	Encoder* encoder = Encoder::getInstance();
	Speaker* speaker = Speaker::getInstance();

public:
	bool isRunning();

	void setMap(Map& map_p);

	float getDistance();
	void setDistance(float dist);

	// num は区画数なので最短の直進回数/2
	void startShrtTrapezoid(int8_t x, int8_t y, MazeAngle angle, uint8_t num);
	// 斜め最短で直進するのは区画中心から区画中心のみである

	void interrupt();

	static GapCounter* getInstance();
};
