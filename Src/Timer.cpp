/*
 * @file Timer.cpp
 */

#include "Timer.h"
// #include "Encoder.h"
// #include "MotorControl.h"
// #include "VelocityControl.h"
#include "Led.h"
#include "Battery.h"

__IO int32_t Timer::total = 0;

int32_t Timer::getTime(){
	return total;
}


void Timer::interrupt(){
	++ total;
	// static Encoder *encoder = Encoder::getInstance();
	// static MotorControl *motor = MotorControl::getInstance();
	// static VelocityControl* vc = VelocityControl::getInstance();
	// static Gyro* gy = Gyro::getInstance();
	static Led* led = Led::getInstance();
	static Battery* battery = Battery::getInstance();
	// encoder->interrupt();
	// gy->readGyroYaw();
	// gy->readAccelFront();
	// vc->interrupt();
	// motor->interrupt();
	led->interrupt();
	battery->interrupt();
	// Speaker::interrupt();
}
