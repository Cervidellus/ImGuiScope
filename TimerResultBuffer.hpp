#ifndef TIMER_RESULT_BUFFER_HPP
#define TIMER_RESULT_BUFFER_HPP	

#include "TimerResult.hpp"

#include <array>
#include <chrono>
#include <iostream>
#include <vector>

//A circular buffer for storing timer results.
class TimerResultBuffer
{
public:
	//maxTime is the period of time the buffer is meant to hold.
	//initSize is the initial size of the underlying vector. It will resize itself periodically. 
	TimerResultBuffer
	(
		const std::chrono::microseconds& maxTime = std::chrono::microseconds(10000000),
		const int& initSize = 100
	);

	void setMaxTime(const std::chrono::microseconds& maxTime);
	
	void put(const TimerResult& result);

	bool isEmpty();

	void cullExpired();

	//Doubles buffer by inserting before the tail, frequency of vector resizing.
	void expandBuffer();

	int count();

	int last();

	float mean();

	std::chrono::nanoseconds sum();
	
	//for setting length of time buffer from ImGui combo box.
	static const std::array<const char*, 4> timeBufferSizeNames;

	//for setting units from ImGui combo box
	static const std::array<const char*, 3> unitNames;

public:
	//True when the buffer is collecting results for the timer.
	bool isActive = false;
	//Index of ImGuiScope::Internal::timeBufferSizeNames used in ImGui widget
	int timeBufferSizeIndex = 0;
	//Index of ImGuiScope::Internal::unitNames used in ImGui widget
	int unitNameIndex = 1;

private:
	std::chrono::microseconds maxTime_;
	std::vector<TimerResult> ringBuffer_;
	int head_ = 0;
	int tail_ = 0;
	std::chrono::microseconds sum_ = std::chrono::microseconds(0);
};

#endif //TIMER_RESULT_BUFFER_HPP
