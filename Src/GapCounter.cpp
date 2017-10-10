/**
 * @file GapCounter.cpp
 */

#include "GapCounter.h"


GapCounter::GapCounter() :
	GAP_DISTANCE_LEFT(0.012f),
	GAP_DISTANCE_RIGHT(0.012f)
{
	running = false;
}

void GapCounter::start(){
	running = true;
}

void GapCounter::stop(){
	running = false;
}

void GapCounter::analyzeGap(int8_t x, int8_t y, MazeAngle angle, uint8_t num){
	for (int i=0; i<num; ++i) {
		// 左を確認
		if (map.isExistWallFromMouse(0, 1, MazeAngle::WEST, x, y, angle)){
			gap_left.at(i) = GAPSTATUS::NOTHING;
		} else if (map.isExistWallFromMouse(0, 0, MazeAngle::WEST, x, y, angle)){
			gap_left.at(i) = GAPSTATUS::CONCURRENT;
		} else {
			gap_left.at(i) = GAPSTATUS::VERTICAL;
		}
		// 右を確認
		if (map.isExistWallFromMouse(0, 1, MazeAngle::EAST, x, y, angle)){
			gap_right.at(i) = GAPSTATUS::NOTHING;
		} else if (map.isExistWallFromMouse(0, 0, MazeAngle::EAST, x, y, angle)){
			gap_right.at(i) = GAPSTATUS::CONCURRENT;
		} else {
			gap_right.at(i) = GAPSTATUS::VERTICAL;
		}
	}
}


void GapCounter::setMap(Map& map_p){
	map.copyFrom(map_p);
}

float GapCounter::getDistance(){
	return passed_distance;
}

void GapCounter::setDistance(float dist){
	passed_distance = dist;
}


void GapCounter::startShrtTrapezoid(int8_t x, int8_t y, MazeAngle angle, uint8_t num){
	analyzeGap(x, y, angle, num);
	start();
	target_distance = num * 0.09f;
	passed_distance = 0.0f;
}

void GapCounter::interrupt(){
	if (!running) return;
	uint8_t ite = 0;
	passed_distance += 0.0005f * (encoder->getVelocity(EncoderSide::LEFT) + encoder->getVelocity(EncoderSide::RIGHT));
	if (wallsensor->hadGap(SensorPosition::Left)) {
		ite = static_cast<uint8_t>(floor((passed_distance/0.09f)+0.04f));
		if (gap_left.at(ite) != GAPSTATUS::NOTHING) {
			passed_distance = ite * 0.09f + 0.045f - GAP_DISTANCE_LEFT;
			speaker->playSound(440, 50, false);
		}
	}
	if (wallsensor->hadGap(SensorPosition::Right)) {
		ite = static_cast<uint8_t>(floor((passed_distance/0.09f)+0.04f));
		if (gap_left.at(ite) != GAPSTATUS::NOTHING) {
			passed_distance = ite * 0.09f + 0.045f - GAP_DISTANCE_RIGHT;
			speaker->playSound(440, 50, false);
		}
	}
	if (passed_distance >= target_distance) {
		stop();
	}
}

GapCounter* GapCounter::getInstance(){
	static GapCounter instance;
	return &instance;
}
