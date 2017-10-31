/*
 * @file MotorCollection.cpp
 */
#include "MotorCollection.h"

MotorCollection::MotorCollection() :
	TIMEOUT(10000),
	GAIN_LIN(0.002f),
	GAIN_RAD(0.6f)
{
	is_running = false;
}

bool MotorCollection::collectionByFrontDuringStop(float lim){
	start_time = Timer::getTime();
	is_running = true;
	lin_limit = lim;
	mc->setIntegralEncoder(0.0f);
	return true;
}

bool MotorCollection::isRunning() {
	return is_running;
}


void MotorCollection::interrupt() {
	if (Timer::getTime() > start_time + TIMEOUT) {
		is_running = false;
	}
	if (!is_running) return;

	int16_t dif_front = wall->getDiffValue(SensorPosition::Front);
	float dif_lin = dif_front;
	float tmp = (dif_lin > 0) ? dif_lin*GAIN_LIN : -1*dif_lin*GAIN_LIN;
	float tmp2 = ((dif_lin > 0) ? -1.0f : 1.0f) * tmp;
	if(tmp2 > 0.1f) tmp2 = 0.1f;
	else if(tmp2 < -0.05f) tmp2 = -0.05f;
	mc->setVelocity(tmp2);
	mc->setRadVelocity(0.0f);

	if(abs(GAIN_LIN * dif_lin) < 0.01f || abs(mc->getIntegralEncoder()) > lin_limit) {
		is_running = false;
		return;
	}
}


MotorCollection* MotorCollection::getInstance(){
	static MotorCollection instance;
	return &instance;
}
