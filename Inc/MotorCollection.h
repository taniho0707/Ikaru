/*
 * @file MotorCollection.h
 */
#pragma once

#include "MotorControl.h"
#include "WallSensor.h"
#include <cmath>
#include "Timer.h"

class MotorCollection{
private:
	const uint32_t TIMEOUT;
	const float GAIN_LIN;
	const float GAIN_RAD;
	
	MotorControl* mc = MotorControl::getInstance();
	WallSensor* wall = WallSensor::getInstance();

	bool is_running;
	int32_t start_time;
	float lin_limit;

	explicit MotorCollection();

public:
	bool collectionByFrontDuringStop(float lin_limit);
	bool isRunning();

	void interrupt();

	static MotorCollection *getInstance();
};

