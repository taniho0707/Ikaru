/*
 * @file MethodAdachi.h
 */
#pragma once

#include <queue>

#include "SlalomParams.h"
#include "Map.h"
#include "Walldata.h"
#include "Footmap.h"
#include "Position.h"

class MethodAdachi{
private:

	std::queue< std::pair<int8_t, int8_t> > que;

	/* int8_t goal_x; */
	/* int8_t goal_y; */

	int8_t cur_x;
	int8_t cur_y;

public:
	Footmap fm;

	Map map;

	MethodAdachi();

	/* void setGoal(int8_t x, int8_t y); */
	void setCurrent(int8_t, int8_t);

	void setMap(Map&);

	void renewFootmap();

	Footmap getFootmap();

	slalomparams::RunType getNextMotion(int8_t x, int8_t y, MazeAngle angle, Walldata walldata);
};
