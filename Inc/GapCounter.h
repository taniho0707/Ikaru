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
	// 直進用
	NOTHING,    // 壁切れ無し
	CONCURRENT, // 進行方向に並行    ｜
	VERTICAL,   // 進行方向に垂直のみ ー
	// 斜め用
	FRONT, // 進行方向から見て手前側のみ ＼ (右手)
	BOTH,  // 進行方向から見て両側      ＜ (右手)
	BACK,  // 進行方向から見て奥側のみ   ／ (右手)
};

enum class DiagoInout : uint8_t {
	LL, // 右ターン→右ターン 直進は奇数区画
	RR, // 左ターン→左ターン 直進は奇数区画
	LR, // 左ターン→右ターン 直進は偶数区画
	RL, // 右ターン→左ターン 直進は偶数区画
	UNDEFINED,
};

class GapCounter {
private:
	const float GAP_DISTANCE_LEFT;
	const float GAP_DISTANCE_RIGHT;
	const float GAP_DISTANCE_DIAGO_LEFT;
	const float GAP_DISTANCE_DIAGO_RIGHT;

	void start();
	void stop();

	void analyzeGap(int8_t x, int8_t y, MouseDirection angle, uint8_t num);
	void analyzeGapDiago(int8_t x, int8_t y, MousePosition side, MouseDirection angle, DiagoInout inout, uint8_t num);

	GapCounter();

	bool running;
	bool is_diago;

	DiagoInout diagoinout;

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
	// 斜め最短で直進するのは区画中心から区画中心のみであるためMousePositionの引数は取らない
	void startShrtTrapezoid(int8_t x, int8_t y, MouseDirection angle, uint8_t num);

	// num は斜めの区画数なので最短の直進回数と等しい
	void startShrtTrapezoidDiago(int8_t x, int8_t y, MousePosition side, MouseDirection angle, DiagoInout inout, uint8_t num);

	void interrupt();

	static GapCounter* getInstance();
};
