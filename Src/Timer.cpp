/*
 * @file Timer.cpp
 */

#include "Timer.h"
#include "Encoder.h"
#include "MotorControl.h"
#include "VelocityControl.h"
#include "Led.h"
#include "Battery.h"
#include "Speaker.h"
#include "Gyro.h"
#include "WallSensor.h"
#include "ModeSelect.h"
#include "GapCounter.h"
#include "MotorCollection.h"

__IO int32_t Timer::total = 0;

int32_t Timer::getTime(){
	return total;
}


void Timer::interrupt(){
	++ total;
	static Led* led = Led::getInstance();
	static Battery* battery = Battery::getInstance();
	static Speaker* speaker = Speaker::getInstance();
	static Encoder* encoder = Encoder::getInstance();
	static Gyro* gy = Gyro::getInstance();
	static MotorControl* motor = MotorControl::getInstance();
	static WallSensor* ws = WallSensor::getInstance();
	static VelocityControl* vc = VelocityControl::getInstance();
	static ModeSelect* mode = ModeSelect::getInstance();
	static GapCounter* gap = GapCounter::getInstance();
	static MotorCollection* correction = MotorCollection::getInstance();
	led->interrupt();
	battery->interrupt();
	speaker->interrupt();
	encoder->interrupt();
	gy->interrupt();
	motor->interrupt();
	ws->interrupt();
	vc->interrupt();
	mode->interrupt();
	gap->interrupt();
	correction->interrupt();
}
