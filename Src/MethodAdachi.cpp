/**
 * @file MethodAdachi.cpp
 */
#include "MethodAdachi.h"

MethodAdachi::MethodAdachi(){
	fm.resetFootmap();
	map.format();
	// renewFootmap();
}

// void MethodAdachi::setGoal(int8_t x, int8_t y){
// 	goal_x = x;
// 	goal_y = y;
// }

void MethodAdachi::setCurrent(int8_t x, int8_t y){
	cur_x = x;
	cur_y = y;
}


void MethodAdachi::setMap(Map& m){
	map.copyFrom(m);
}

void MethodAdachi::renewFootmap(){
	std::pair<int8_t, int8_t> buf;
	std::pair<int8_t, int8_t> tmp;

	bool is_end = false;

	fm.resetFootmap();
	std::queue< std::pair<int8_t, int8_t> >().swap(que);

	for (auto ite : map.goals.curs) {
		tmp.first = ite.first;
		tmp.second = ite.second;
		que.push(tmp);
		fm.setFootmap(ite.first, ite.second, 0);
	}

	while(!que.empty()){
		buf = que.front();
		que.pop();

		if((map.isExistWall(buf.first, buf.second, MazeAngle::NORTH) == false)
		   && (fm.getFootmap(buf.first, buf.second + 1, 0) == 1024))
		{
			tmp.first = buf.first;
			tmp.second = buf.second + 1;
			if(!is_end) que.push(tmp);
			fm.setFootmap(buf.first, buf.second + 1, fm.getMinNextTo(buf.first, buf.second + 1, map.getWalldata(buf.first, buf.second + 1)) + 1);
		}
		if((map.isExistWall(buf.first, buf.second, MazeAngle::EAST) == false)
		   && (fm.getFootmap(buf.first + 1, buf.second, 0) == 1024))
		{
			tmp.first = buf.first + 1;
			tmp.second = buf.second;
			if(!is_end) que.push(tmp);
			fm.setFootmap(buf.first + 1, buf.second, fm.getMinNextTo(buf.first + 1, buf.second, map.getWalldata(buf.first + 1, buf.second)) + 1);
		}
		if((map.isExistWall(buf.first, buf.second, MazeAngle::SOUTH) == false)
		   && (fm.getFootmap(buf.first, buf.second - 1, 0) == 1024))
		{
			tmp.first = buf.first;
			tmp.second = buf.second - 1;
			if(!is_end) que.push(tmp);
			fm.setFootmap(buf.first, buf.second - 1, fm.getMinNextTo(buf.first, buf.second - 1, map.getWalldata(buf.first, buf.second - 1)) + 1);
		}
		if((map.isExistWall(buf.first, buf.second, MazeAngle::WEST) == false)
		   && (fm.getFootmap(buf.first - 1, buf.second, 0) == 1024))
		{
			tmp.first = buf.first - 1;
			tmp.second = buf.second;
			if(!is_end) que.push(tmp);
			fm.setFootmap(buf.first - 1, buf.second, fm.getMinNextTo(buf.first - 1, buf.second, map.getWalldata(buf.first - 1, buf.second)) + 1);
		}

		if(buf.first == cur_x && buf.second == cur_y) {
			is_end = true;
			return;
		}
	}
}

Footmap MethodAdachi::getFootmap(){
	return fm;
}

slalomparams::RunType MethodAdachi::getNextMotion(int8_t x, int8_t y, MazeAngle angle, Walldata walldata){
	uint16_t min = fm.getMinNextTo(x, y, Walldata::rotateWallToAbsolute(walldata, angle));
	if(angle == MazeAngle::NORTH){
		if(fm.getFootmap(x, y+1, 1024) == min && walldata.isExistWall(MouseAngle::FRONT)==false) return slalomparams::RunType::TRAPACCEL;
		else if(fm.getFootmap(x+1, y, 1024) == min && walldata.isExistWall(MouseAngle::RIGHT)==false) return slalomparams::RunType::SLALOM90SML_RIGHT;
		else if(fm.getFootmap(x-1, y, 1024) == min && walldata.isExistWall(MouseAngle::LEFT)==false) return slalomparams::RunType::SLALOM90SML_LEFT;
		else return slalomparams::RunType::PIVOTTURN;
	} else if(angle == MazeAngle::EAST){
		if(fm.getFootmap(x+1, y, 1024) == min && walldata.isExistWall(MouseAngle::FRONT)==false) return slalomparams::RunType::TRAPACCEL;
		else if(fm.getFootmap(x, y-1, 1024) == min && walldata.isExistWall(MouseAngle::RIGHT)==false) return slalomparams::RunType::SLALOM90SML_RIGHT;
		else if(fm.getFootmap(x, y+1, 1024) == min && walldata.isExistWall(MouseAngle::LEFT)==false) return slalomparams::RunType::SLALOM90SML_LEFT;
		else return slalomparams::RunType::PIVOTTURN;
	} else if(angle == MazeAngle::SOUTH){
		if(fm.getFootmap(x, y-1, 1024) == min && walldata.isExistWall(MouseAngle::FRONT)==false) return slalomparams::RunType::TRAPACCEL;
		else if(fm.getFootmap(x-1, y, 1024) == min && walldata.isExistWall(MouseAngle::RIGHT)==false) return slalomparams::RunType::SLALOM90SML_RIGHT;
		else if(fm.getFootmap(x+1, y, 1024) == min && walldata.isExistWall(MouseAngle::LEFT)==false) return slalomparams::RunType::SLALOM90SML_LEFT;
		else return slalomparams::RunType::PIVOTTURN;
	} else if(angle == MazeAngle::WEST){
		if(fm.getFootmap(x-1, y, 1024) == min && walldata.isExistWall(MouseAngle::FRONT)==false) return slalomparams::RunType::TRAPACCEL;
		else if(fm.getFootmap(x, y+1, 1024) == min && walldata.isExistWall(MouseAngle::RIGHT)==false) return slalomparams::RunType::SLALOM90SML_RIGHT;
		else if(fm.getFootmap(x, y-1, 1024) == min && walldata.isExistWall(MouseAngle::LEFT)==false) return slalomparams::RunType::SLALOM90SML_LEFT;
		else return slalomparams::RunType::PIVOTTURN;
	}
}

