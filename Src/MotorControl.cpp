/**
 * @file MotorControl.cpp
 */

#include "MotorControl.h"

MotorControl::MotorControl() : 
	GAIN_LIN_P(1600),
	GAIN_LIN_I(2.8),
	GAIN_LIN_D(0.01), //要パラメータ調整
	GAIN_RAD_P(-0.4f),
	GAIN_RAD_I(-0.03f),
	GAIN_RAD_D(0.0f),
	GAIN_WALL_P(-1.0f),
	GAIN_WALL_SHRT_P(-1.5f),
	GAIN_WALL_I(0.0f),
	GAIN_WALL_D(0.0f),
	TREAD(380.0f)
{
	cur_lin_x = 0.0;
	cur_lin_vel = 0.0;
	cur_lin_acc = 0.0;
	tar_lin_vel = 0.0;
	enabled_wall_control = true;
	is_shrt_wall_control = false;
	is_comb_wall_control = false;
	is_failsafe = false;
	integral_rad_gyro = 0.0f;
	integral_lin_encoder = 0.0f;
	is_left_gap = false;
	is_left_gap_diago = false;
}

void MotorControl::setVelocity(float vel){
	tar_lin_vel = vel;
}

void MotorControl::setRadVelocity(float rad){
	tar_rad_vel = rad;
}

void MotorControl::stay(){
	motor->enable();
	setVelocity(0.0);
	setRadVelocity(0.0);
}

void MotorControl::resetWallIntegral(){
	integral_wall = 0.0f;
}

void MotorControl::resetRadIntegral(){
	integral_rad_gyro = 0.0f;
}
void MotorControl::resetLinIntegral(){
	integral_lin_encoder = 0.0f;
}

void MotorControl::calcIntegral(){
	cur_lin_vel = (encoder->getVelocityCorrect(EncoderSide::LEFT)+encoder->getVelocityCorrect(EncoderSide::RIGHT))/2.0f;
	cur_lin_x += cur_lin_vel * 0.001f;
}

void MotorControl::setIntegralEncoder(float t){
	cur_lin_x = t;
}
float MotorControl::getIntegralEncoder(){
	return cur_lin_x;
}

void MotorControl::resetDistanceFromGap(){
	dist_from_gap = -10.0f;
}

float MotorControl::getDistanceFromGap(){
	return dist_from_gap;
}

bool MotorControl::isLeftGap(){
	return is_left_gap;
}


void MotorControl::resetDistanceFromGapDiago(){
	dist_from_gap_diago = -10.0f;
}

float MotorControl::getDistanceFromGapDiago(){
	return dist_from_gap_diago;
}

bool MotorControl::isLeftGapDiago(){
	return is_left_gap_diago;
}


void MotorControl::controlX(){
	
}


void MotorControl::enableWallControl(){
	enabled_wall_control = true;
	is_comb_wall_control = false;
}
void MotorControl::disableWallControl(){
	enabled_wall_control = false;
}

void MotorControl::setExprWallControl(){
	is_shrt_wall_control = false;
}
void MotorControl::setShrtWallControl(){
	is_shrt_wall_control = true;
}

void MotorControl::setCombWallControl(){
	is_comb_wall_control = true;
}
void MotorControl::resetCombWallControl(){
	is_comb_wall_control = false;
}

void MotorControl::controlVel(){
	static float tar_motor_lin_power = 0;
	static float tar_motor_rad_power = 0;
	static int16_t tar_motor_r_power = 0;
	static int16_t tar_motor_l_power = 0;
	static float tar_vel_rev = 0;
	static float tar_rad_rev = 0;
	static float integral_r = 0.0f;
	static float integral_l = 0.0f;

	static float d_rad_gyro = 0.0;

	static int16_t lastwall = 0;
	static int16_t current_wall_correction = 0;

	if(integral_lin_encoder > 100 || integral_rad_gyro > 50000){
		led->initPort(LedNumbers::TOP1);
		led->flickAsync(LedNumbers::FRONT, 4.0f, 0);
		led->flickAsync(LedNumbers::TOP1, 4.0f, 0);
		is_failsafe = true;
	}

	if(is_failsafe){
		motor->setDuty(MotorSide::LEFT, 0);
		motor->setDuty(MotorSide::RIGHT, 0);
		motor->disable();
		return;
	}

	if(enabled_wall_control){
		if(is_comb_wall_control){
			current_wall_correction = wall->getCorrectionComb(500);
		} else {
			current_wall_correction = wall->getCorrection(20);
		}
	} else {
		current_wall_correction = 0;
	}
	if(tar_lin_vel < 0.025f){
		current_wall_correction = 0;
	}

	// // 壁積分値の計算
	// integral_wall += wall->getCorrection(10000) * enabled_wall_control;

	// rotation成分の計算
	tar_rad_rev = ((tar_rad_vel - (is_shrt_wall_control ? GAIN_WALL_SHRT_P : GAIN_WALL_P) * ((tar_lin_vel < 0.001f) ? 3 : 1) * current_wall_correction) - gyro->getGyroYaw());
	// tar_rad_rev = ((tar_rad_vel - enabled_wall_control * GAIN_WALL_P * wall->getCorrection(10000) - enabled_wall_control * GAIN_WALL_D * (wall->getCorrection(10000)-lastwall)) - gyro->getGyroYaw());
	// d_rad_gyro = (tar_rad_vel - gyro->getGyroYaw()) - tar_rad_rev;
	// integral_rad_gyro += (tar_rad_vel - gyro->getGyroYaw());
	// if(wall->isExistWall(SensorPosition::Left) || wall->isExistWall(SensorPosition::Right)) integral_rad_gyro = 0.0f;
	integral_rad_gyro += tar_rad_rev;
	tar_motor_rad_power = GAIN_RAD_P * tar_rad_rev + GAIN_RAD_I * integral_rad_gyro;

	// linear成分の計算
	tar_vel_rev = (tar_lin_vel) - ((encoder->getVelocity(EncoderSide::RIGHT)+encoder->getVelocity(EncoderSide::LEFT))/2/* + gyro->getAccelFront() * 0.0005*/);
	integral_lin_encoder += tar_vel_rev;
	difference_lin_encoder = last_lin_encoder - tar_vel_rev;
	last_lin_encoder = tar_vel_rev;
	tar_motor_lin_power = GAIN_LIN_P * tar_vel_rev + GAIN_LIN_I * integral_lin_encoder + GAIN_LIN_D * difference_lin_encoder;

	// 壁切れ用の計算
	dist_from_gap += 0.001f * cur_lin_vel;
	if(wall->hadGap(SensorPosition::Left)){
		dist_from_gap = 0.0f;
		is_left_gap = true;
	}
	if(wall->hadGap(SensorPosition::Right)){
		dist_from_gap = 0.0f;
		is_left_gap = false;
	}
	dist_from_gap_diago += 0.001f * cur_lin_vel;
	if(wall->hadGapDiago(SensorPosition::Left)){
		dist_from_gap_diago = 0.0f;
		is_left_gap_diago = true;
	}
	if(wall->hadGapDiago(SensorPosition::Right)){
		dist_from_gap_diago = 0.0f;
		is_left_gap_diago = false;
	}

	// モーター出力
	tar_motor_r_power = static_cast<int16_t>(tar_motor_lin_power - tar_motor_rad_power);
	tar_motor_l_power = static_cast<int16_t>(tar_motor_lin_power + tar_motor_rad_power);

	motor->setDuty(MotorSide::LEFT, tar_motor_l_power);
	motor->setDuty(MotorSide::RIGHT, tar_motor_r_power);

	// log->writeFloat(tar_lin_vel);
	// log->writeFloat(encoder->getVelocity(EncoderSide::LEFT));
	// log->writeFloat(encoder->getVelocity(EncoderSide::RIGHT));
	// log->writeFloat((encoder->getVelocity(EncoderSide::LEFT)+encoder->getVelocity(EncoderSide::RIGHT))/2);
	// log->writeFloat(cur_lin_x);
	// log->writeFloat(gyro->getGyroYaw());
	// log->writeFloat(tar_rad_vel);
	// log->writeFloat(wall->getValue(SensorPosition::Left));
	// log->writeFloat(wall->getValue(SensorPosition::Right));
	// // log->writeFloat(current_wall_correction);
	// // log->writeFloat(tar_motor_l_power);
	// // log->writeFloat(tar_motor_r_power);
	// log->writeFloat(getIntegralEncoder());
	// // log->writeFloat(getDistanceFromGap());
	// // log->writeFloat(getDistanceFromGapDiago());

	// lastwall = wall->getCorrection(10000);

	if(enabled_wall_control) led->on(LedNumbers::TOP1);
	else led->off(LedNumbers::TOP1);
}


void MotorControl::interrupt(){
	if(motor->isEnabled()){
		calcIntegral();
		controlVel();
	}
}



MotorControl* MotorControl::getInstance(){
	static MotorControl instance;
	return &instance;
}

