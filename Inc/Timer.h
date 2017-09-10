/**
 * @file Timer.hpp
 * 正確に秒数を計ることを担ったクラス
 */
#pragma once

#include "stm32f4xx.h"

/**
 * 正確に秒数を計ることを担ったクラス
 */
class Timer{
private:
	static __IO int32_t total;

	Timer();

public:
	static int32_t getTime();

	/**
	 * @brief タイマーの加算を行う。<br>
	 * 正確に1msごとに呼ばれる必要がある。
	 */
	static void interrupt();


};
