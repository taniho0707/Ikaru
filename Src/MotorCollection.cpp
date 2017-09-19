/*
 * @file MotorCollection.cpp
 */
#include "MotorCollection.h"

MotorCollection::MotorCollection() :
	TIMEOUT(10000),
	GAIN_LIN(0.0004f),
	GAIN_RAD(0.6f)
{
	
}

bool MotorCollection::collectionByFrontDuringStop(float lin_limit){
	auto start_time = Timer::getTime();
	int16_t dif_left = wall->getDiffValue(SensorPosition::FLeft);
	int16_t dif_right = wall->getDiffValue(SensorPosition::FRight);
	int16_t dif_front = wall->getDiffValue(SensorPosition::Front);
	float dif_lin = dif_front;
	float dif_rad = 0;
	float tmp;
	mc->setIntegralEncoder(0.0f);
	while(Timer::getTime() < start_time + TIMEOUT){
		dif_front = wall->getDiffValue(SensorPosition::Front);
		dif_lin = dif_front;
		tmp = (dif_lin > 0) ? dif_lin*GAIN_LIN : -1*dif_lin*GAIN_LIN;
		mc->setVelocity(((dif_lin > 0) ? -1.0f : 1.0f) * tmp);
		mc->setRadVelocity(0.0f);
		if(abs(GAIN_LIN * dif_lin) < 0.01f) break;
		HAL_Delay(1);
		if(abs(mc->getIntegralEncoder()) > lin_limit) break;
	}
	return false;
}

MotorCollection* MotorCollection::getInstance(){
	static MotorCollection instance;
	return &instance;
}
