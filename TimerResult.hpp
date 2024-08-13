#ifndef TIMERRESULT_HPP
#define TIMERRESULT_HPP

#include <chrono>
#include <string>

struct TimerResult
{
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::microseconds elapsed;
};

#endif //TIMERRESULT_HPP