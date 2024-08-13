#ifndef TIMER_RESULT_BUFFER_HPP
#define TIMER_RESULT_BUFFER_HPP	

#include "TimerResult.hpp"

#include <chrono>
#include <vector>

class TimerResultBuffer
{
	//A circular buffer for storing timer results.
	//It is meant to store results collected over a specified period of time.
	//I need to store both the start and end points so I can tie the buffer to time.
public:
	//maxTime is the period of time the buffer is meant to hold.
	//initSize is the initial size of the underlying vector. It will resize itself periodically. 
	TimerResultBuffer(
		const std::chrono::microseconds& maxTime = std::chrono::microseconds(1000), 
		const int& initSize = 100) :
		maxTime_(maxTime),
		ringBuffer_(std::vector<TimerResult>(initSize, { std::chrono::high_resolution_clock::time_point(), std::chrono::microseconds(0)}))
	{
	}

	//True when the buffer is collecting results for the timer.
	bool isActive = false;

	void setMaxTime(const std::chrono::microseconds& maxTime) { maxTime_ = maxTime; }
	
	void put(const TimerResult& result)
	{
		sum_ += result.elapsed;
		ringBuffer_[head_] = result;
		int nextHead_ = (head_ + 1) % ringBuffer_.size();
		if (nextHead_ == tail_)
		{
			//First cull. If that doesn't work, resize and reset the head and tail.
			cullExpired();
			if (nextHead_ == tail_) expandBuffer();
		}
		head_ = nextHead_;
	}

	bool isEmpty() { return (head_ == tail_); }

	void cullExpired() {
		while (std::chrono::high_resolution_clock::now() - ringBuffer_[tail_].start > maxTime_ && tail_ != head_)
		{
			sum_ -= ringBuffer_[tail_].elapsed;
			tail_ = (tail_ + 1) % ringBuffer_.size();
		}
	}

	//Doubles the size of the buffer by inserting before the tail, making room for more head insertions.
	void expandBuffer()
	{
		int originalBufferSize = ringBuffer_.size();
		ringBuffer_.insert(ringBuffer_.begin() + tail_, originalBufferSize, TimerResult());
		if (head_ >= tail_) head_ += originalBufferSize;
		tail_ += originalBufferSize;
	}

	int count() {
		if (head_ >= tail_) return head_ - tail_;
		else return ringBuffer_.size() - tail_ + head_;
	}

	std::chrono::nanoseconds sum()
	{
		cullExpired();
		return sum_;
	}


private:
	std::chrono::microseconds maxTime_ = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds(1));
	std::vector<TimerResult> ringBuffer_;
	int head_ = 0;
	int tail_ = 0;
	std::chrono::nanoseconds sum_ = std::chrono::nanoseconds(0);
};

#endif //TIMER_RESULT_BUFFER_HPP
