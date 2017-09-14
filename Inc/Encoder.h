/**
 * @file Encoder.h
 */
#pragma once

#include "stm32f4xx_hal.h"

enum class EncoderSide : uint8_t{
	LEFT,
	RIGHT,
};

class Encoder{
private:
	explicit Encoder();

	// BUFFER SIZE of history of velocity
	constexpr static uint16_t BUFSIZE = 10;

	float hist_l[BUFSIZE];
	float hist_r[BUFSIZE];
	uint8_t ite_hist;

	const static uint16_t MEDIAN;
	const static float PULSE_L;
	const static float PULSE_R;
	float velocity_l;
	float velocity_r;
	float last_l;
	float last_r;
	
public:
	float getVelocity(EncoderSide);
	float getVelocityCorrect(EncoderSide);
	void interrupt();

	static Encoder* getInstance();
};

