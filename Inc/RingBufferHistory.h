/**
 * @file RingBufferHistory.h
 */
#pragma once

#include <array>

template <typename T, uint16_t N>
class RingBufferHistory{
private:
	std::array<T, N> buf;
	uint16_t ite;
public:
	explicit RingBufferHistory(){ }

	void push(const T& data){
		buf.at(ite) = data;
		if(++ite >= N) ite = 0;
		return;
	}

	T read(){
		return buf[ite];
	}

	T getAverage(){
		float sum = 0.0f;
		for (auto i: buf) {
			sum += i;
		}
		sum /= N;
		return sum;
	}
};
