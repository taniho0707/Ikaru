/*
 * @file main.cpp
 */

#include "main.h"
#include "stm32f4xx_hal.h"

#include "SlalomParams.h"

#include "Led.h"
#include "ComPc.h"
#include "Battery.h"
#include "Speaker.h"
#include "Gyro.h"
#include "Fram.h"
#include "Datalog.h"

#include "Motor.h"
#include "Encoder.h"
#include "WallSensor.h"

#include "MotorControl.h"
#include "VelocityControl.h"
#include "ModeSelect.h"
#include "MotorCollection.h"

#include "Walldata.h"

#include "Position.h"
#include "Map.h"
#include "Footmap.h"

#include "MethodAdachi.h"

#include "Path.h"
#include "libpathbasic1.hpp"

#include "Graph.h"

using namespace slalomparams;
using namespace std;

void SystemClock_Config(void);
static void MX_NVIC_Init(void);

const uint16_t GOAL_X = 6;
const uint16_t GOAL_Y = 7;


void frontcorrection(){
	Led* led = Led::getInstance();
	MotorControl* mc = MotorControl::getInstance();
	MotorCollection* motorcollection = MotorCollection::getInstance();
	led->flickAsync(LedNumbers::FRONT, 5.0f, 1500);
	mc->disableWallControl();
	motorcollection->collectionByFrontDuringStop(0.02f);
	mc->stay();
	mc->resetRadIntegral();
	mc->resetLinIntegral();
	led->flickStop(LedNumbers::FRONT);
	HAL_Delay(300);
}

void calibratewallsensor(){
	WallSensor* wallsensor = WallSensor::getInstance();
	ModeSelect* mode = ModeSelect::getInstance();
	Speaker* speaker = Speaker::getInstance();
	Datalog* log = Datalog::getInstance();

	array<uint16_t, 100> tmp_avg1;
	array<uint16_t, 100> tmp_avg2;
	array<uint16_t, 100> tmp_avg3;
	uint16_t thr_fleft, thr_left, thr_front, thr_right, thr_fright;
	uint16_t ref_fleft, ref_left, ref_front, ref_right, ref_fright;
	float tmp_sum;

	// 1. 両横壁のみ
	mode->startcheck(0);
	speaker->playMusic(MusicNumber::HIRAPA);
	for (int i=0; i<100; ++i) {
		tmp_avg1.at(i) = wallsensor->getValue(SensorPosition::Left);
		tmp_avg2.at(i) = wallsensor->getValue(SensorPosition::Right);
		// log->writeFloat(tmp_avg1.at(i));
		// log->writeFloat(tmp_avg2.at(i));
		// log->writeFloat(0.0f);
		// log->writeFloat(0.0f);
		// log->writeFloat(0.0f);
		// log->writeFloat(0.0f);
		// log->writeFloat(0.0f);
		// log->writeFloat(0.0f);
		// log->writeFloat(0.0f);
		// log->writeFloat(0.0f);
		HAL_Delay(1);
	}
	tmp_sum = 0;
	for (auto el : tmp_avg1)
		tmp_sum += el;
	ref_left = tmp_sum / 100;
	// log->writeFloat(0.0f);
	// log->writeFloat(0.0f);
	// log->writeFloat(0.0f);
	// log->writeFloat(0.0f);
	// log->writeFloat(0.0f);
	// log->writeFloat(0.0f);
	// log->writeFloat(0.0f);
	// log->writeFloat(0.0f);
	// log->writeFloat(ref_left);
	// log->writeFloat(tmp_sum);
	tmp_sum = 0;
	for (auto el : tmp_avg2)
		tmp_sum += el;
	ref_right = tmp_sum / 100;
	// log->writeFloat(0.0f);
	// log->writeFloat(0.0f);
	// log->writeFloat(0.0f);
	// log->writeFloat(0.0f);
	// log->writeFloat(0.0f);
	// log->writeFloat(0.0f);
	// log->writeFloat(0.0f);
	// log->writeFloat(0.0f);
	// log->writeFloat(ref_right);
	// log->writeFloat(tmp_sum);
	speaker->playMusic(MusicNumber::KIRBY_DYING);

	// 2. 前壁制御用
	mode->startcheck(0);
	speaker->playMusic(MusicNumber::HIRAPA);
	for (int i=0; i<100; ++i) {
		tmp_avg1.at(i) = wallsensor->getValue(SensorPosition::Front);
		HAL_Delay(1);
	}
	tmp_sum = 0;
	for (auto el : tmp_avg1)
		tmp_sum += el;
	ref_front = tmp_sum / 100;
	speaker->playMusic(MusicNumber::KIRBY_DYING);

	// 3. 横・前に壁なし
	mode->startcheck(0);
	speaker->playMusic(MusicNumber::HIRAPA);
	for (int i=0; i<100; ++i) {
		tmp_avg1.at(i) = wallsensor->getValue(SensorPosition::Left);
		tmp_avg2.at(i) = wallsensor->getValue(SensorPosition::Right);
		tmp_avg3.at(i) = wallsensor->getValue(SensorPosition::Front);
		HAL_Delay(1);
	}
	tmp_sum = 0;
	for (auto el : tmp_avg1)
		tmp_sum += el;
	thr_left = (ref_left + (tmp_sum / 100)) / 2;
	tmp_sum = 0;
	for (auto el : tmp_avg2)
		tmp_sum += el;
	thr_right = (ref_right + (tmp_sum / 100)) / 2;
	tmp_sum = 0;
	for (auto el : tmp_avg3)
		tmp_sum += el;
	thr_front = (tmp_sum / 100);
	speaker->playMusic(MusicNumber::KIRBY_DYING);

	// 4. 前壁存在
	mode->startcheck(0);
	speaker->playMusic(MusicNumber::HIRAPA);
	for (int i=0; i<100; ++i) {
		tmp_avg1.at(i) = wallsensor->getValue(SensorPosition::Left);
		HAL_Delay(1);
	}
	tmp_sum = 0;
	for (auto el : tmp_avg1)
		tmp_sum += el;
	thr_front = ((tmp_sum / 100) + thr_front) / 2.0f;
	speaker->playMusic(MusicNumber::KIRBY_DYING);

	// // 5. 左壁切れ（キツイパターン）
	// mode->startcheck(0);
	// speaker->playMusic(MusicNumber::HIRAPA);
	// for (int i=0; i<100; ++i) {
	// 	tmp_avg1.at(i) = wallsensor->getValue(SensorPosition::Left);
	// 	HAL_Delay(1);
	// }
	// tmp_sum = 0;
	// for (auto el : tmp_avg1)
	// 	tmp_sum += el;
	//  = tmp_sum /= 100;
	// speaker->playMusic(MusicNumber::KIRBY_DYING);

	// // 6. 右壁切れ（キツイパターン）
	// mode->startcheck(0);
	// speaker->playMusic(MusicNumber::HIRAPA);
	// for (int i=0; i<100; ++i) {
	// 	tmp_avg1.at(i) = wallsensor->getValue(SensorPosition::Left);
	// 	HAL_Delay(1);
	// }
	// tmp_sum = 0;
	// for (auto el : tmp_avg1)
	// 	tmp_sum += el;
	//  = tmp_sum /= 100;
	// speaker->playMusic(MusicNumber::KIRBY_DYING);

	// 代入
	wallsensor->valid_val_ref_left = ref_left;
	wallsensor->valid_val_ref_right = ref_right;
	wallsensor->valid_val_ref_front = ref_front;
	wallsensor->valid_val_thr_left = thr_left;
	wallsensor->valid_val_thr_right = thr_right;
	wallsensor->valid_val_thr_front = thr_front;
	wallsensor->refleshStraigntValue();
	wallsensor->saveParamsToFram();
}

int main(void) {
	HAL_Init();
	SystemClock_Config();

	Led* led = Led::getInstance();
	led->on(LedNumbers::FRONT);
	HAL_Delay(10);
	led->flickAsync(LedNumbers::FRONT, 5, 1000);
	Speaker* speaker = Speaker::getInstance();
	speaker->playMusic(MusicNumber::KIRBY3_POWERON);
	ComPc* compc = ComPc::getInstance();
	compc->printf("Hello Ikaru!\n");

	Battery* battery = Battery::getInstance();
	battery->scan();
	compc->printf("%f [V]\n", battery->getVoltage());

	Gyro* gyro = Gyro::getInstance();
	HAL_Delay(10);
	if (gyro->whoami()) {
		compc->printf("Gyro WhoAmI was SUCCEEDED!!!\n");
	} else {
		compc->printf("Gyro WhoAmI was failed...\n");
	}

	Fram* fram = Fram::getInstance();
	HAL_Delay(10);
	if (fram->readDeviceId()) {
		compc->printf("Fram RDID was SUCCEEDED!!!\n");
	} else {
		compc->printf("Fram RDID was failed...\n");
	}
	fram->writeStatusResister(0x2);
	HAL_Delay(1);
	fram->writeEnable();
	HAL_Delay(1);

	HAL_Delay(1000);
	led->flickSync(LedNumbers::FRONT, 10, 1000);
	led->flickAsync(LedNumbers::FRONT, 4, 2000);
	gyro->resetCalibration();

	Encoder* encoder = Encoder::getInstance();

	WallSensor* wallsensor = WallSensor::getInstance();
	wallsensor->start();

	if (wallsensor->isLatestFram()) compc->printf("[ FRAM] ");
	else compc->printf("[FLASH] ");
	compc->printf("parameters hash: [%8X]\n", fram->readUInt32(1880));
	wallsensor->loadParams();

	compc->printf("* Wall Sensor Parameters\n");
	compc->printf("valid_val_ref_fleft:            %4d\n", wallsensor->valid_val_ref_fleft);
	compc->printf("valid_val_ref_left:             %4d\n", wallsensor->valid_val_ref_left);
	compc->printf("valid_val_ref_front:            %4d\n", wallsensor->valid_val_ref_front);
	compc->printf("valid_val_ref_right:            %4d\n", wallsensor->valid_val_ref_right);
	compc->printf("valid_val_ref_fright:           %4d\n", wallsensor->valid_val_ref_fright);
	compc->printf("valid_val_thr_fleft:            %4d\n", wallsensor->valid_val_thr_fleft);
	compc->printf("valid_val_thr_left:             %4d\n", wallsensor->valid_val_thr_left);
	compc->printf("valid_val_thr_front:            %4d\n", wallsensor->valid_val_thr_front);
	compc->printf("valid_val_thr_right:            %4d\n", wallsensor->valid_val_thr_right);
	compc->printf("valid_val_thr_fright:           %4d\n", wallsensor->valid_val_thr_fright);
	compc->printf("valid_thr_wall_disappear:       %4d\n", wallsensor->valid_thr_wall_disappear);
	compc->printf("valid_val_thr_control_left:     %4d\n", wallsensor->valid_val_thr_control_left);
	compc->printf("valid_val_thr_control_right:    %4d\n", wallsensor->valid_val_thr_control_right);
	compc->printf("valid_val_thr_gap_fleft:        %4d\n", wallsensor->valid_val_thr_gap_fleft);
	compc->printf("valid_val_thr_gap_left:         %4d\n", wallsensor->valid_val_thr_gap_left);
	compc->printf("valid_val_thr_gap_right:        %4d\n", wallsensor->valid_val_thr_gap_right);
	compc->printf("valid_val_thr_gap_fright:       %4d\n", wallsensor->valid_val_thr_gap_fright);
	compc->printf("valid_val_thr_gap_diago_fleft:  %4d\n", wallsensor->valid_val_thr_gap_diago_fleft);
	compc->printf("valid_val_thr_gap_diago_left:   %4d\n", wallsensor->valid_val_thr_gap_diago_left);
	compc->printf("valid_val_thr_gap_diago_right:  %4d\n", wallsensor->valid_val_thr_gap_diago_right);
	compc->printf("valid_val_thr_gap_diago_fright: %4d\n", wallsensor->valid_val_thr_gap_diago_fright);
	compc->printf("valid_val_thr_slalom_fleft:     %4d\n", wallsensor->valid_val_thr_slalom_fleft);
	compc->printf("valid_val_thr_slalom_left:      %4d\n", wallsensor->valid_val_thr_slalom_left);
	compc->printf("valid_val_thr_slalom_right:     %4d\n", wallsensor->valid_val_thr_slalom_right);
	compc->printf("valid_val_thr_slalom_fright:    %4d\n", wallsensor->valid_val_thr_slalom_fright);


	HAL_Delay(50);
	if (wallsensor->getValue(SensorPosition::Front) > 500) {
		Datalog *log = Datalog::getInstance();
		constexpr auto num = 10;
		for(auto i=0; i<log->getSize()/num; ++i){
			compc->printf("%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", i,
						  log->readFloat(num*i),
						  log->readFloat(num*i+1),
						  log->readFloat(num*i+2),
						  log->readFloat(num*i+3),
						  log->readFloat(num*i+4),
						  log->readFloat(num*i+5),
						  log->readFloat(num*i+6),
						  log->readFloat(num*i+7),
						  log->readFloat(num*i+8),
						  log->readFloat(num*i+9)
				);
		}
	}

	while(true){
		ModeSelect* mode = ModeSelect::getInstance();
		speaker->playMusic(MusicNumber::KIRBYKAGAMI_OP);
		mode::StructMode decided_mode = mode->select();
		bool enabled_mae = false;
	
		switch(decided_mode.prime){
		case static_cast<uint8_t>(mode::MODE_PRIME::HIDARITE):
		{
			switch(decided_mode.sub){
			case static_cast<uint8_t>(mode::MODE_HIDARITE::WITHOUT_MAE):
				enabled_mae = false;
				break;
			case static_cast<uint8_t>(mode::MODE_HIDARITE::WITH_MAE):
				enabled_mae = true;
				break;
			default:
				break;
			}
			float gvel = 0.10f;
			float gaccel = 1.5f;
			float section = 0.045f;
			float turn_coefficient = 0.4f;
			// ここから左手法で走る
			mode->startcheck(0);
			led->initPort(LedNumbers::TOP1);
			led->flickAsync(LedNumbers::TOP1, 4.0f, 2000);
			speaker->playMusic(MusicNumber::HIRAPA);
			HAL_Delay(1000);
			Map map;
			Position pos;
			Walldata walldata;
			map.setReached(0, 0);
			Datalog* log = Datalog::getInstance();

			MotorControl* mc = MotorControl::getInstance();
			mc->stay();
			led->on(LedNumbers::FRONT);
			VelocityControl* vc = VelocityControl::getInstance();
			vc->disableWallgap();

			MotorCollection* motorcollection = MotorCollection::getInstance();
			vc->runTrapAccel(0.0f, gvel, gvel, section*0.5f, gaccel);
			// vc->enableWallgap();
			mc->enableWallControl();
			while(vc->isRunning());

			while (1) {
				walldata = wallsensor->getWall();
				map.addWall(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
				map.setReached(pos.getPositionX(), pos.getPositionY());
				// fram->saveMap(map, 0);

				// vc->runTrapAccel(0.25f, 0.25f, 0.25f, 0.09f, 5.0f);
				// // vc->disableWallgap();
				// while(vc->isRunning());
				if(wallsensor->isExistWall(SensorPosition::Left) == false){
					vc->runSlalom(RunType::SLALOM90SML_LEFT, gvel);
					// vc->disableWallgap();
					mc->disableWallControl();
					while(vc->isRunning());
					pos.setNextPosition(RunType::SLALOM90SML_LEFT);
				} else if(wallsensor->isExistWall(SensorPosition::Front) == false){
					vc->runTrapAccel(gvel, gvel, gvel, section, gaccel);
					// vc->disableWallgap();
					mc->enableWallControl();
					while(vc->isRunning());
					pos.setNextPosition(RunType::TRAPACCEL);
				} else if(wallsensor->isExistWall(SensorPosition::Right) == false){
					vc->runSlalom(RunType::SLALOM90SML_RIGHT, gvel);
					// vc->disableWallgap();
					mc->disableWallControl();
					while(vc->isRunning());
					pos.setNextPosition(RunType::SLALOM90SML_RIGHT);
				} else {
					vc->runTrapAccel(gvel, gvel, 0.0f, section*0.5f, gaccel);
					// vc->disableWallgap();
					mc->disableWallControl();
					while(vc->isRunning());
					pos.setNextPosition(RunType::PIVOTTURN);

					if(enabled_mae){
						led->flickAsync(LedNumbers::FRONT, 5.0f, 1500);
						mc->disableWallControl();
						motorcollection->collectionByFrontDuringStop(0.02f);
						mc->stay();
						mc->resetRadIntegral();
						mc->resetLinIntegral();
						led->flickStop(LedNumbers::FRONT);
						HAL_Delay(300);
						vc->runPivotTurn(500, 90, 1000);
						mc->disableWallControl();
						while(vc->isRunning());
						led->flickAsync(LedNumbers::FRONT, 5.0f, 1500);
						mc->disableWallControl();
						motorcollection->collectionByFrontDuringStop(0.02f);
						mc->stay();
						mc->resetRadIntegral();
						mc->resetLinIntegral();
						led->flickStop(LedNumbers::FRONT);
						led->on(LedNumbers::FRONT);
						HAL_Delay(300);
						vc->runPivotTurn(500, 90, 1000);
						mc->disableWallControl();
						while(vc->isRunning());
						HAL_Delay(300);
					} else {
						vc->runPivotTurn(500*turn_coefficient, 180, 5000*turn_coefficient);
						mc->disableWallControl();
						while(vc->isRunning());
					}
			
					mc->resetRadIntegral();
					mc->resetLinIntegral();
					vc->runTrapAccel(0.0f, gvel, gvel, section*0.5f, gaccel);
					// vc->disableWallgap();
					mc->disableWallControl();
					while(vc->isRunning());
				}
			}
		}
		break;
		case static_cast<uint8_t>(mode::MODE_PRIME::EXPR):
		{
			Map map;

			switch(decided_mode.sub){
			case static_cast<uint8_t>(mode::MODE_EXPR::NEW):
				enabled_mae = true;
				break;
			case static_cast<uint8_t>(mode::MODE_EXPR::LOAD):
				enabled_mae = true;
				fram->loadMap(map, decided_mode.number);
				break;
			default:
				break;
			}

			mode->startcheck(0);
			led->initPort(LedNumbers::TOP1);
			led->flickAsync(LedNumbers::TOP1, 4.0f, 2000);
			speaker->playMusic(MusicNumber::KIRBY64_BEGINNER_1);
			HAL_Delay(1000);

			MotorControl* mc = MotorControl::getInstance();
			mc->stay();
			led->on(LedNumbers::FRONT);
			VelocityControl* vc = VelocityControl::getInstance();
			vc->enableWallgap();
			vc->setExprGap();
			mc->enableWallControl();
			mc->setExprGap();
			MotorCollection* motorcollection = MotorCollection::getInstance();

			mc->stay();
			vc->runTrapAccel(0.0f, 0.25f, 0.25f, 0.045f, 5.0f);
			while(vc->isRunning());
			vc->startTrapAccel(0.25f, 0.25f, 0.09f, 5.0f);

			Walldata walldata;
			Position pos;
			MethodAdachi adachi;
			map.setReached(0, 0);
			adachi.setGoal(GOAL_X, GOAL_Y);
			uint16_t savenumber = 0;

			Datalog* log = Datalog::getInstance();

			while(true){
				led->off(LedNumbers::FRONT);
				walldata = wallsensor->getWall();
				map.addWall(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
				map.setReached(pos.getPositionX(), pos.getPositionY());
				adachi.setCurrent(pos.getPositionX(), pos.getPositionY());
				adachi.setMap(map);
				adachi.renewFootmap();
				RunType runtype = adachi.getNextMotion(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);

				led->on(LedNumbers::FRONT);
		
				if(runtype == slalomparams::RunType::TRAPACCEL){
					bool flag_can_straight = true;
					vc->runTrapAccel(0.25f, 0.25f, 0.25f, 0.025f, 3.0f);
					mc->enableWallControl();
					while(vc->isRunning()) {
						if (wallsensor->getValue(SensorPosition::Front) > (160)) { // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
							flag_can_straight = false;
						}
					}
					if (flag_can_straight) {
						vc->runTrapAccel(0.25f, 0.25f, 0.25f, 0.065f, 3.0f);
						while(vc->isRunning());
					} else {
						vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.02f, 3.0f);
						while(vc->isRunning());
						/// @todo ここ修正
						if (!wallsensor->isExistWall(SensorPosition::Right)) {
							runtype = slalomparams::RunType::SLALOM90SML_RIGHT;
							frontcorrection();
							vc->runPivotTurn(500, -90, 1000);
							while(vc->isRunning());
							vc->runTrapAccel(0.0f, 0.25f, 0.25f, 0.045f, 3.0f);
							mc->disableWallControl();
							// vc->disableWallgap();
							while(vc->isRunning());
						} else if (!wallsensor->isExistWall(SensorPosition::Left)) {
							runtype = slalomparams::RunType::SLALOM90SML_LEFT;
							frontcorrection();
							vc->runPivotTurn(500, 90, 1000);
							while(vc->isRunning());
							vc->runTrapAccel(0.0f, 0.25f, 0.25f, 0.045f, 3.0f);
							mc->disableWallControl();
							// vc->disableWallgap();
							while(vc->isRunning());
						} else {
							runtype = slalomparams::RunType::PIVOTTURN;
							vc->runPivotTurn(500, 180, 1000);
							while(vc->isRunning());
							vc->runTrapAccel(0.0f, 0.25f, 0.25f, 0.045f, 3.0f);
							mc->disableWallControl();
							// vc->disableWallgap();
							while(vc->isRunning());
						}
					}
					vc->enableWallgap();
				} else if(runtype == slalomparams::RunType::PIVOTTURN){
					fram->saveMap(map, savenumber);
					vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.045f, 3.0f);
					while(vc->isRunning());
					if(walldata.isExistWall(MouseAngle::FRONT)){
						frontcorrection();
					}
					if(walldata.isExistWall(MouseAngle::LEFT)){
						vc->runPivotTurn(500, 90, 1000);
						while(vc->isRunning());
						frontcorrection();
						vc->runPivotTurn(500, 90, 1000);
						while(vc->isRunning());
						mc->resetRadIntegral();
						mc->resetLinIntegral();
					} else {
						vc->runPivotTurn(500, 180, 1000);
						/// @todo -180にする
						while(vc->isRunning());
						mc->resetRadIntegral();
						mc->resetLinIntegral();
					}

					vc->runTrapAccel(0.0f, 0.25f, 0.0f, -0.03f, 3.0f);
					mc->enableWallControl();
					while(vc->isRunning());

					mc->resetDistanceFromGap();
					mc->resetDistanceFromGapDiago();
					vc->runTrapAccel(0.0f, 0.25f, 0.25f, 0.075f, 3.0f);
					mc->enableWallControl();
					while(vc->isRunning());
				} else if(runtype == slalomparams::RunType::SLALOM90SML_RIGHT){
					vc->runSlalom(RunType::SLALOM90SML_RIGHT, 0.25f);
					while(vc->isRunning());
				} else if(runtype == slalomparams::RunType::SLALOM90SML_LEFT){
					vc->runSlalom(RunType::SLALOM90SML_LEFT, 0.25f);
					while(vc->isRunning());
				} else {
					vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.045f, 4.0f);
					mc->enableWallControl();
					while(vc->isRunning());
					break;
				}
		
				pos.setNextPosition(runtype);
				vc->startTrapAccel(0.25f, 0.25f, 0.09f, 5.0f);
		
				static bool is_first_goal = true; // = has_goal
				if(pos.getPositionX() == GOAL_X && pos.getPositionY() == GOAL_Y && is_first_goal){
					is_first_goal = false;
					led->on(LedNumbers::TOP1);
					speaker->playSound(1200, 500, false);
					walldata = wallsensor->getWall();
					map.setWall(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
					map.setReached(pos.getPositionX(), pos.getPositionY());
					led->off(LedNumbers::TOP1);

					led->on(LedNumbers::TOP1);
					adachi.setGoal(0, 0);
					fram->saveMap(map, 1);
				} else if(pos.getPositionX() == 0 && pos.getPositionY() == 0){
					vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.045f, 5.0f);
					while(vc->isRunning());
					mc->disableWallControl();

					fram->saveMap(map, 2);

					break;
				}
			}
		}
		case static_cast<uint8_t>(mode::MODE_PRIME::SHRT):
		{
			PathType type;

			float param_max_straight = 0.3f;
			float param_max_diago = 0.3f;
			float param_max_turn = 0.3f;
			float param_accel = 3.0f;
			
			switch(decided_mode.sub){
			case static_cast<uint8_t>(mode::MODE_SHRT::SMALL1):
				type = PathType::SMALL;
				break;
			case static_cast<uint8_t>(mode::MODE_SHRT::SMALL2):
				type = PathType::SMALL;
				param_max_straight = 0.5f;
				param_accel = 5.0f;
				break;
			case static_cast<uint8_t>(mode::MODE_SHRT::BIG):
				type = PathType::BIG;
				break;
			case static_cast<uint8_t>(mode::MODE_SHRT::DIAGO):
				type = PathType::DIAGO;
				break;
			default:
				break;
			}
			
			mode->startcheck(0);
			led->initPort(LedNumbers::TOP1);
			led->flickAsync(LedNumbers::TOP1, 4.0f, 2000);
			speaker->playMusic(MusicNumber::KIRBY64_BEGINNER_2);
			// HAL_Delay(1000);

			Walldata walldata;
			Map map;
			Footmap footmap;
			Position pos;
			DerivingPathBasic1 padachi;

			fram->loadMap(map, decided_mode.number);
			padachi.setGoal(GOAL_X, GOAL_Y);
			padachi.setMap(map);
			
			Path path = padachi.getPath(type);
			if(path.getMotion(0).type == RunType::PIVOTTURN){
				led->flickAsync(LedNumbers::FRONT, 4.0f, 0);
				led->flickAsync(LedNumbers::TOP1, 4.0f, 0);
				while(true);
			}

			led->flickSync(LedNumbers::FRONT, 3.0f, 1000);

			MotorControl* mc = MotorControl::getInstance();
			mc->stay();
			led->on(LedNumbers::FRONT);
			VelocityControl* vc = VelocityControl::getInstance();
			vc->enableWallgap();
			mc->enableWallControl();
			mc->setShrtWallControl();

			GapCounter* gapcounter = GapCounter::getInstance();
			gapcounter->setMap(map);
			path.updatePositions();

			// type == PathType::SMALL の場合のみ横センサを使い，
			// そうでなければ前横センサで壁切れを読めるようにする
			if (type == PathType::SMALL) {
				mc->setExprGap();
			} else {
				mc->setShrtGap();
			}

			if (type == PathType::DIAGO) {
				vc->setShrtGap();
			}

			struct Motion motion;
			int i=0;
			vc->enableWallgap();
			mc->resetDistanceFromGap();
			mc->resetDistanceFromGapDiago();
			vc->startTrapAccel(0.0f, param_max_turn, 0.09f, param_accel);

			while(true){
				motion = path.getMotion(i);
				if(i == 0){
					pair<int8_t, int8_t> cur_pos = path.getPositionCoordinate(i);
					vc->setPosition(cur_pos.first, cur_pos.second, path.getAngleCoordinate(i));
					vc->runTrapAccel(0.0f, param_max_straight, param_max_turn, 0.045f*(motion.length-1) +0.02f, param_accel);
				} else {
					if(path.getMotion(i+1).type == RunType::PIVOTTURN){
						vc->runTrapAccel(param_max_turn, param_max_straight, 0.0f, 0.045f*(motion.length+1), param_accel);
						while(vc->isRunning());
						break;
					}
			
					if(motion.type == RunType::TRAPACCEL){
						pair<int8_t, int8_t> cur_pos = path.getPositionCoordinate(i);
						vc->setPosition(cur_pos.first, cur_pos.second, path.getAngleCoordinate(i));
						led->on(LedNumbers::FRONT);
						if(path.getMotion(i+1).type == RunType::SLALOM90SML_RIGHT || path.getMotion(i+1).type == RunType::SLALOM90SML_LEFT)
							vc->runTrapAccel(param_max_turn, param_max_straight, 0.3f, 0.045f*motion.length, param_accel);
						else
							vc->runTrapAccel(param_max_turn, param_max_straight, param_max_turn, 0.045f*motion.length, param_accel);
						led->off(LedNumbers::FRONT);
					} else if(motion.type == RunType::TRAPDIAGO){
						vc->runTrapDiago(param_max_turn, param_max_diago, param_max_turn, 0.06364f*motion.length, param_accel);
					} else if(motion.type == RunType::SLALOM90SML_RIGHT || motion.type == RunType::SLALOM90SML_LEFT){
						vc->runSlalom(motion.type, 0.3f);
					} else {
						vc->runSlalom(motion.type, param_max_turn);
					}
				}
				while(vc->isRunning());
				++i;
				vc->startTrapAccel(param_max_turn, param_max_turn, 0.09f, param_accel);
			}

			while(true);

			break;
		}
		case static_cast<uint8_t>(mode::MODE_PRIME::TURNADJUST):
		{
			mode->startcheck(0);
			led->initPort(LedNumbers::TOP1);
			led->flickAsync(LedNumbers::TOP1, 4.0f, 2000);
			speaker->playMusic(MusicNumber::HIRAPA);
			HAL_Delay(1000);

			MotorControl* mc = MotorControl::getInstance();
			mc->stay();
			led->on(LedNumbers::FRONT);
			VelocityControl* vc = VelocityControl::getInstance();

			switch(decided_mode.sub){
			case static_cast<uint8_t>(mode::MODE_TURNADJUST::STRAIGHT_6):
			{
				vc->disableWallgap();
				mc->disableWallControl();
				mc->stay();
				vc->runTrapAccel(0.0f, 0.25f, 0.0f, 0.72f, 3.0f);
				while(vc->isRunning());
				while(1);
				break;
			}
			case static_cast<uint8_t>(mode::MODE_TURNADJUST::PIVOTTURN):
			{
				vc->disableWallgap();
				mc->disableWallControl();
				for (int i=0; i<10; ++i) {
					vc->runPivotTurn(500, 180, 1000);
					while(vc->isRunning());
					HAL_Delay(800);
				}
				while(1);
				break;
			}
			case static_cast<uint8_t>(mode::MODE_TURNADJUST::SLALOM90SML_LEFT1):
			{
				vc->disableWallgap();
				mc->enableWallControl();
				vc->runTrapAccel(0.0f, 0.25f, 0.25f, 0.09f+0.045f, 3.0f);
				while(vc->isRunning());
				for (int i=0; i<40; ++i) {
					vc->runTrapAccel(0.25f, 0.25f, 0.25f, 0.09f, 3.0f);
					while(vc->isRunning());
					vc->runSlalom(RunType::SLALOM90SML_LEFT, 0.25f);
					mc->disableWallControl();
					while(vc->isRunning());
				}
				vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.45f, 3.0f);
				while(true);
				break;
			}
			case static_cast<uint8_t>(mode::MODE_TURNADJUST::SLALOM90SML_LEFT2):
			{
				vc->disableWallgap();
				mc->enableWallControl();
				vc->runTrapAccel(0.0f, 0.25f, 0.25f, 0.09f+0.045f, 3.0f);
				while(vc->isRunning());
				vc->runSlalom(RunType::SLALOM90SML_LEFT, 0.25f);
				mc->disableWallControl();
				while(vc->isRunning());
				vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.09f+0.045f, 3.0f);
				while(vc->isRunning());
				mc->stay();
				while(true);
				break;
			}
			case static_cast<uint8_t>(mode::MODE_TURNADJUST::SLALOM90SML_RIGHT1):
			{
				vc->disableWallgap();
				mc->enableWallControl();
				vc->runTrapAccel(0.0f, 0.25f, 0.25f, 0.09f+0.045f, 3.0f);
				while(vc->isRunning());
				for (int i=0; i<40; ++i) {
					vc->runTrapAccel(0.25f, 0.25f, 0.25f, 0.09f, 3.0f);
					while(vc->isRunning());
					vc->runSlalom(RunType::SLALOM90SML_RIGHT, 0.25f);
					mc->disableWallControl();
					while(vc->isRunning());
				}
				vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.45f, 3.0f);
				while(true);
				break;
			}
			case static_cast<uint8_t>(mode::MODE_TURNADJUST::SLALOM90SML_RIGHT2):
			{
				vc->disableWallgap();
				mc->enableWallControl();
				vc->runTrapAccel(0.0f, 0.25f, 0.25f, 0.09f+0.045f, 3.0f);
				while(vc->isRunning());
				vc->runSlalom(RunType::SLALOM90SML_RIGHT, 0.25f);
				mc->disableWallControl();
				while(vc->isRunning());
				vc->runTrapAccel(0.25f, 0.25f, 0.0f, 0.09f+0.045f, 3.0f);
				while(vc->isRunning());
				mc->stay();
				while(true);
				break;
			}
			default:
				break;
			}
		}
		case static_cast<uint8_t>(mode::MODE_PRIME::SENSORLOG):
		{
			switch(decided_mode.sub){
			case static_cast<uint8_t>(mode::MODE_SENSORLOG::SHOW):
			{
				while(1){
					led->off(LedNumbers::FRONT);
					HAL_Delay(99);
					led->on(LedNumbers::FRONT);
					HAL_Delay(1);
					compc->printf("[%4d %4d %4d %4d %4d ] %+7d %+7d %+7d : %+7d %+7d %+7d, %f %f [%f %f]\n", wallsensor->getValue(SensorPosition::FLeft), wallsensor->getValue(SensorPosition::Left), wallsensor->getValue(SensorPosition::Front), wallsensor->getValue(SensorPosition::Right), wallsensor->getValue(SensorPosition::FRight), gyro->readGyroX(), gyro->readGyroY(), gyro->readGyroZ(), gyro->readAccelX(), gyro->readAccelY(), gyro->readAccelZ(), encoder->getVelocity(EncoderSide::LEFT), encoder->getVelocity(EncoderSide::RIGHT), wallsensor->getDistance(SensorPosition::Left), wallsensor->getDistance(SensorPosition::Right));
				}
				break;
			}
			case static_cast<uint8_t>(mode::MODE_SENSORLOG::CALIBRATE_SIDE):
			{
				speaker->playMusic(MusicNumber::OIRABOKODAZE1);
				mode->startcheck(0);
				speaker->playMusic(MusicNumber::HIRAPA);
				float value11 = wallsensor->getValue(SensorPosition::Left);
				float value21 = wallsensor->getValue(SensorPosition::Right);
				speaker->playMusic(MusicNumber::KIRBY_DYING);
				mode->startcheck(0);
				speaker->playMusic(MusicNumber::HIRAPA);
				float value12 = wallsensor->getValue(SensorPosition::Left);
				float value22 = wallsensor->getValue(SensorPosition::Right);
				wallsensor->calibrate(SensorPosition::Left, 10.0f, value11, 54.8f, value12);
				wallsensor->calibrate(SensorPosition::Right, 54.8f, value21, 10.0f, value22);
				speaker->playMusic(MusicNumber::KIRBY_DYING);
				break;
			}
			case static_cast<uint8_t>(mode::MODE_SENSORLOG::CALIBRATE_REFTHR):
			{
				speaker->playMusic(MusicNumber::OIRABOKODAZE1);
				calibratewallsensor();
				break;
			}
			}
		}
		break;
		case static_cast<uint8_t>(mode::MODE_PRIME::RUNLOG):
		{
			led->off(LedNumbers::FRONT);
			HAL_Delay(1000);
			for (int i=0; i<decided_mode.number; ++i) {
				led->off(LedNumbers::FRONT);
				HAL_Delay(500);
				led->on(LedNumbers::FRONT);
				HAL_Delay(500);
			}

			switch(decided_mode.sub){
			case static_cast<uint8_t>(mode::MODE_RUNLOG::MAZE1):
			{
				Map map;
				fram->loadMap(map, 0);
				compc->printf("* 0");
				for(int i=0; i<32; ++i) compc->printf("+----");
				compc->printf("+\n");
				for(int j=0; j<32; ++j){
					for(int i=0; i<32; ++i){
						if(map.isExistWall(i, 31-j, MazeAngle::WEST)) compc->printf("|");
						else compc->printf(" ");
						if(map.hasReached(i, 31-j)) compc->printf(" () ");
						else compc->printf("    ");
					}
					compc->printf("|\n");
					for(int i=0; i<32; ++i){
						if(map.isExistWall(i, 31-j, MazeAngle::SOUTH)) compc->printf("+----");
						else compc->printf("+    ");
					}
					compc->printf("+\n");
				}
				fram->loadMap(map, 1);
				compc->printf("* 1");
				for(int i=0; i<32; ++i) compc->printf("+----");
				compc->printf("+\n");
				for(int j=0; j<32; ++j){
					for(int i=0; i<32; ++i){
						if(map.isExistWall(i, 31-j, MazeAngle::WEST)) compc->printf("|");
						else compc->printf(" ");
						if(map.hasReached(i, 31-j)) compc->printf(" () ");
						else compc->printf("    ");
					}
					compc->printf("|\n");
					for(int i=0; i<32; ++i){
						if(map.isExistWall(i, 31-j, MazeAngle::SOUTH)) compc->printf("+----");
						else compc->printf("+    ");
					}
					compc->printf("+\n");
				}

				Graph graph;
				compc->printf("GRAPH\n");
				graph.connectWithMap(map);
				compc->printf("CONNECTED WITH MAP\n");
				vector<uint16_t> result = graph.dijkstra(Graph::cnvCoordinateToNum(0, 0, MazeAngle::SOUTH), Graph::cnvCoordinateToNum(6, 6, MazeAngle::NORTH));
				compc->printf("DIJKSTRA COMPLETED\n");
				compc->printf("%d\n", result.size());
				int16_t result_x, result_y;
				MazeAngle result_angle;
				for (auto ite : result) {
					Graph::cnvNumToCoordinate(ite, result_x, result_y, result_angle);
					compc->printf("%4d (%2d,%2d : %1d)\n", ite, result_x, result_y, result_angle);
				}
				compc->printf("END\n");

				fram->loadMap(map, 2);
				compc->printf("* 2");
				for(int i=0; i<32; ++i) compc->printf("+----");
				compc->printf("+\n");
				for(int j=0; j<32; ++j){
					for(int i=0; i<32; ++i){
						if(map.isExistWall(i, 31-j, MazeAngle::WEST)) compc->printf("|");
						else compc->printf(" ");
						if(map.hasReached(i, 31-j)) compc->printf(" () ");
						else compc->printf("    ");
					}
					compc->printf("|\n");
					for(int i=0; i<32; ++i){
						if(map.isExistWall(i, 31-j, MazeAngle::SOUTH)) compc->printf("+----");
						else compc->printf("+    ");
					}
					compc->printf("+\n");
				}
				break;
			}
			case static_cast<uint8_t>(mode::MODE_RUNLOG::ERASE):
			{
				Datalog* log = Datalog::getInstance();
				log->cleanFlash();
				speaker->playMusic(MusicNumber::KIRBY_DYING);
				break;
			}
			}
			break;
		}
		}
	}

	
	// // vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.135f, 5.0f);
	// // while(vc->isRunning());
	// while(1){
	// 	// vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.09f, 5.0f);
	// 	// while(vc->isRunning());
	// 	// vc->runSlalom(RunType::SLALOM90SML_RIGHT, 0.3f);
	// 	// while(vc->isRunning());
	// 	vc->runPivotTurn(500, 180, 2000);
	// 	while(vc->isRunning());
	// 	HAL_Delay(1000);
	// }

	// vc->runTrapAccel(0.0f, 0.1f, 0.0f, 0.27f, 5.0f);
	// while(vc->isRunning());
	// HAL_Delay(10000);
	// vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.135f, 5.0f);
	// while(vc->isRunning());
	// vc->runSlalom(RunType::SLALOM90SML_LEFT, 0.3f);
	// while(vc->isRunning());
	// vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.135f, 5.0f);
	// mc->disableWallControl();
	// while(vc->isRunning());

	// vc->runTrapAccel(0.0f, 0.3f, 0.0f, 1.26f, 3.0f);
	// vc->disableWallgap();
	// mc->disableWallControl();
	// mc->stay();
	// Motor* motor = Motor::getInstance();
	// motor->disable();
	// HAL_Delay(10000);
	// speaker->playMusic(MusicNumber::KIRBY_GOURMETRACE);
	// Datalog *log = Datalog::getInstance();
	// constexpr auto num = 10;
	// for(auto i=0; i<log->getSize()/num; ++i){
	// 	compc->printf("%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", i,
	// 				  log->readFloat(num*i),
	// 				  log->readFloat(num*i+1),
	// 				  log->readFloat(num*i+2),
	// 				  log->readFloat(num*i+3),
	// 				  log->readFloat(num*i+4),
	// 				  log->readFloat(num*i+5),
	// 				  log->readFloat(num*i+6),
	// 				  log->readFloat(num*i+7),
	// 				  log->readFloat(num*i+8),
	// 				  log->readFloat(num*i+9)
	// 		);
	// }
	


	if (gyro->readAccelZ() < 0) {
		while(1){
			led->off(LedNumbers::FRONT);
			HAL_Delay(99);
			led->on(LedNumbers::FRONT);
			HAL_Delay(1);
			compc->printf("[%4d %4d %4d %4d %4d ] %+7d %+7d %+7d : %+7d %+7d %+7d, %f %f\n", wallsensor->getValue(SensorPosition::FLeft), wallsensor->getValue(SensorPosition::Left), wallsensor->getValue(SensorPosition::Front), wallsensor->getValue(SensorPosition::Right), wallsensor->getValue(SensorPosition::FRight), gyro->readGyroX(), gyro->readGyroY(), gyro->readGyroZ(), gyro->readAccelX(), gyro->readAccelY(), gyro->readAccelZ(), encoder->getVelocity(EncoderSide::LEFT), encoder->getVelocity(EncoderSide::RIGHT));
		}
	}
}




/** System Clock Configuration */
	void SystemClock_Config(void) {
		RCC_OscInitTypeDef RCC_OscInitStruct;
		RCC_ClkInitTypeDef RCC_ClkInitStruct;

		/**Configure the main internal regulator output voltage */
		__HAL_RCC_PWR_CLK_ENABLE();
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

		/**Initializes the CPU, AHB and APB busses clocks */
		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
		RCC_OscInitStruct.HSIState = RCC_HSI_ON;
		RCC_OscInitStruct.HSICalibrationValue = 16;
		RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
		RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
		RCC_OscInitStruct.PLL.PLLM = 8;
		RCC_OscInitStruct.PLL.PLLN = 100;
		RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
		RCC_OscInitStruct.PLL.PLLQ = 2;
		RCC_OscInitStruct.PLL.PLLR = 2;
		if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		/**Initializes the CPU, AHB and APB busses clocks */
		RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
		RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
		RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

		if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		/**Configure the Systick interrupt time */
		HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

		/**Configure the Systick */
		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

		/* SysTick_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
	}



/** NVIC Configuration
 */
	static void MX_NVIC_Init(void) {
		/* DMA1_Stream3_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
		/* DMA1_Stream4_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
		/* ADC_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(ADC_IRQn);
		/* SPI2_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(SPI2_IRQn);
		/* SPI3_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(SPI3_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(SPI3_IRQn);
		/* DMA2_Stream0_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
		/* DMA2_Stream7_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
	}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
	void _Error_Handler(char * file, int line) {
		/* USER CODE BEGIN Error_Handler_Debug */
		/* User can add his own implementation to report the HAL error return state */
		while(1) {
		}
		/* USER CODE END Error_Handler_Debug */ 
	}

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
	void assert_failed(uint8_t* file, uint32_t line) {
		/* USER CODE BEGIN 6 */
		/* User can add his own implementation to report the file name and line number,
		   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
		/* USER CODE END 6 */
	}

#endif

// // http://mohammedari.blogspot.jp/2013/11/arm-none-eabi-gcc.html
// extern void (**__init_array_start)();
// extern void (**__init_array_end)();
 
// inline void static_init()
// {
//   for (void (**p)() = __init_array_start; p < __init_array_end; ++p)
//     (*p)();
// }
